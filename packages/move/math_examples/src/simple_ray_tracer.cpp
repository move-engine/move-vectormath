#include <cstdint>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

#include "camera_component.hpp"
#include "move/math/vec3.hpp"

namespace
{
    using scalar_type = float;
    using vector_type =
        move::math::vec3<scalar_type, move::math::Acceleration::Default>;
    using color_type = vector_type;
    using camera_type = examples::CameraComponent<scalar_type>;

    struct Ray
    {
        vector_type origin;
        vector_type direction;
    };

    struct Material
    {
        color_type albedo = color_type::one();
        color_type emission = color_type::zero();
        scalar_type roughness = 1.0f;
        scalar_type metallic = 0.0f;
    };

    struct Sphere
    {
        vector_type center = vector_type::zero();
        scalar_type radius = 1.0f;
        Material material;
    };

    struct Hit
    {
        scalar_type distance = std::numeric_limits<scalar_type>::max();
        vector_type position = vector_type::zero();
        vector_type normal = vector_type::up();
        Material material;
        bool front_face = true;
    };

    struct Rng
    {
        std::uint32_t state = 0x12345678u;

        explicit Rng(std::uint32_t seed) : state(seed)
        {
        }

        scalar_type next()
        {
            state ^= state << 13;
            state ^= state >> 17;
            state ^= state << 5;
            return scalar_type(state & 0x00FFFFFFu) /
                   scalar_type(0x01000000u);
        }
    };

    [[nodiscard]] vector_type random_in_unit_sphere(Rng& rng)
    {
        for (;;)
        {
            const vector_type sample(
                rng.next() * 2.0f - 1.0f,
                rng.next() * 2.0f - 1.0f,
                rng.next() * 2.0f - 1.0f);

            if (sample.length_squared() < 1.0f)
            {
                return sample;
            }
        }
    }

    [[nodiscard]] bool intersect_sphere(const Ray& ray,
                                        const Sphere& sphere,
                                        scalar_type t_min,
                                        scalar_type t_max,
                                        Hit& out_hit)
    {
        const vector_type oc = ray.origin - sphere.center;
        const scalar_type a = vector_type::dot(ray.direction, ray.direction);
        const scalar_type half_b = vector_type::dot(oc, ray.direction);
        const scalar_type c = vector_type::dot(oc, oc) -
                              sphere.radius * sphere.radius;
        const scalar_type discriminant = half_b * half_b - a * c;
        if (discriminant < 0.0f)
        {
            return false;
        }

        const scalar_type sqrt_discriminant = move::math::sqrt(discriminant);
        scalar_type root = (-half_b - sqrt_discriminant) / a;
        if (root < t_min || root > t_max)
        {
            root = (-half_b + sqrt_discriminant) / a;
            if (root < t_min || root > t_max)
            {
                return false;
            }
        }

        out_hit.distance = root;
        out_hit.position = ray.origin + ray.direction * root;
        const vector_type outward_normal =
            (out_hit.position - sphere.center) * (1.0f / sphere.radius);
        out_hit.front_face =
            vector_type::dot(ray.direction, outward_normal) < 0.0f;
        out_hit.normal = out_hit.front_face ? outward_normal
                                            : vector_type(-outward_normal);
        out_hit.material = sphere.material;
        return true;
    }

    [[nodiscard]] color_type sky_color(const vector_type& direction)
    {
        const scalar_type t =
            move::math::lerp(0.0f, 1.0f, direction.normalized().get_y() * 0.5f + 0.5f);
        return color_type(0.85f, 0.92f, 1.0f) * (1.0f - t) +
               color_type(0.35f, 0.55f, 0.95f) * t;
    }

    [[nodiscard]] bool scatter(const Ray& ray_in,
                               const Hit& hit,
                               Rng& rng,
                               color_type& out_attenuation,
                               Ray& out_ray)
    {
        const scalar_type metallic =
            move::math::clamp(hit.material.metallic, 0.0f, 1.0f);
        const scalar_type roughness =
            move::math::clamp(hit.material.roughness, 0.0f, 1.0f);

        vector_type bounce_direction = vector_type::reflect(
            ray_in.direction.normalized(), hit.normal.normalized());

        if (metallic < 1.0f)
        {
            bounce_direction =
                (hit.normal + random_in_unit_sphere(rng)).normalized();
        }

        if (roughness > 0.0f)
        {
            bounce_direction =
                (bounce_direction + random_in_unit_sphere(rng) * roughness)
                    .normalized();
        }

        if (vector_type::dot(bounce_direction, hit.normal) <= 0.0f)
        {
            bounce_direction = hit.normal;
        }

        out_attenuation = hit.material.albedo;
        out_ray.origin = hit.position + hit.normal * 0.001f;
        out_ray.direction = bounce_direction.normalized();
        return true;
    }

    [[nodiscard]] color_type trace(const Ray& ray,
                                   const std::vector<Sphere>& scene,
                                   Rng& rng,
                                   int max_bounces)
    {
        Ray current_ray = ray;
        color_type throughput = color_type::one();
        color_type radiance = color_type::zero();

        for (int bounce = 0; bounce < max_bounces; ++bounce)
        {
            Hit closest_hit;
            bool found_hit = false;
            scalar_type closest_distance =
                std::numeric_limits<scalar_type>::max();

            for (const Sphere& sphere : scene)
            {
                Hit hit;
                if (intersect_sphere(
                        current_ray, sphere, 0.001f, closest_distance, hit))
                {
                    found_hit = true;
                    closest_distance = hit.distance;
                    closest_hit = hit;
                }
            }

            if (!found_hit)
            {
                radiance += throughput * sky_color(current_ray.direction);
                break;
            }

            radiance += throughput * closest_hit.material.emission;

            color_type attenuation;
            Ray scattered;
            if (!scatter(current_ray, closest_hit, rng, attenuation, scattered))
            {
                break;
            }

            throughput = throughput * attenuation;
            current_ray = scattered;
        }

        return radiance;
    }

    [[nodiscard]] std::uint8_t to_byte(scalar_type value)
    {
        const scalar_type gamma_corrected =
            move::math::pow(move::math::clamp(value, 0.0f, 1.0f), 1.0f / 2.2f);
        return static_cast<std::uint8_t>(gamma_corrected * 255.0f + 0.5f);
    }
}  // namespace

int main(int argc, char** argv)
{
    const std::string output_path =
        argc > 1 ? argv[1] : "simple_ray_tracer.ppm";

    constexpr int width = 320;
    constexpr int height = 180;
    constexpr int samples_per_pixel = 16;
    constexpr int max_bounces = 4;

    camera_type camera;
    camera.aspect_ratio = static_cast<scalar_type>(width) /
                          static_cast<scalar_type>(height);
    camera.vertical_fov = move::math::deg2rad(55.0f);
    camera.transform.local_position = vector_type(0.0f, 1.4f, -6.0f);
    camera.look_at(vector_type(0.0f, 1.0f, 0.0f));

    std::vector<Sphere> scene = {
        {vector_type(0.0f, -1000.0f, 0.0f),
         1000.0f,
         {color_type(0.65f, 0.68f, 0.72f), color_type::zero(), 1.0f, 0.0f}},
        {vector_type(-1.6f, 1.0f, 1.0f),
         1.0f,
         {color_type(0.85f, 0.3f, 0.25f), color_type::zero(), 1.0f, 0.0f}},
        {vector_type(1.3f, 0.9f, 0.2f),
         0.9f,
         {color_type(0.9f, 0.92f, 0.95f), color_type::zero(), 0.08f, 1.0f}},
        {vector_type(0.0f, 3.5f, 1.5f),
         0.75f,
         {color_type::zero(), color_type(8.0f, 7.0f, 6.0f), 0.0f, 0.0f}},
    };

    std::ofstream out(output_path, std::ios::binary);
    if (!out)
    {
        std::cerr << "Failed to open output file: " << output_path << '\n';
        return 1;
    }

    out << "P3\n" << width << ' ' << height << "\n255\n";

    Rng rng(0xC0FFEEu);
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            color_type accumulated = color_type::zero();
            for (int sample = 0; sample < samples_per_pixel; ++sample)
            {
                const scalar_type u =
                    (static_cast<scalar_type>(x) + rng.next()) /
                    static_cast<scalar_type>(width);
                const scalar_type v =
                    (static_cast<scalar_type>(y) + rng.next()) /
                    static_cast<scalar_type>(height);

                const move::math::float2 ndc(
                    u * 2.0f - 1.0f, (1.0f - v) * 2.0f - 1.0f);
                const camera_type::Ray camera_ray = camera.ndc_to_world_ray(ndc);
                accumulated += trace(
                    {camera_ray.origin, camera_ray.direction.normalized()},
                    scene,
                    rng,
                    max_bounces);
            }

            const color_type pixel =
                accumulated * (1.0f / static_cast<scalar_type>(samples_per_pixel));
            out << static_cast<int>(to_byte(pixel.get_x())) << ' '
                << static_cast<int>(to_byte(pixel.get_y())) << ' '
                << static_cast<int>(to_byte(pixel.get_z())) << '\n';
        }
    }

    std::cout << "Wrote " << output_path << '\n';
    return 0;
}
