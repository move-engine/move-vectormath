#include <algorithm>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

#include "camera_component.hpp"

namespace
{
    using scalar_type = float;
    using vector_type = mv::math::Vec3f;
    using point_type = mv::math::Point3f;
    using color_type = vector_type;
    using ray_type = mv::math::Ray3f;
    using camera_type = examples::CameraComponent<scalar_type>;

    struct Material
    {
        color_type albedo = color_type::One();
        color_type emission = color_type::Zero();
        scalar_type roughness = 1.0F;
        scalar_type metallic = 0.0F;
    };

    struct Sphere
    {
        point_type center = point_type::Origin();
        scalar_type radius = 1.0F;
        Material material;
    };

    struct Hit
    {
        scalar_type distance = std::numeric_limits<scalar_type>::max();
        point_type position = point_type::Origin();
        mv::math::Normal3f normal = mv::math::Normal3f::AxisY();
        Material material;
        bool front_face = true;
    };

    struct Rng
    {
        std::uint32_t state = 0x12345678U;

        explicit Rng(std::uint32_t seed) : state(seed)
        {
        }

        scalar_type next()
        {
            state ^= state << 13U;
            state ^= state >> 17U;
            state ^= state << 5U;
            return static_cast<scalar_type>(state & 0x00FFFFFFU) /
                   static_cast<scalar_type>(0x01000000U);
        }
    };

    [[nodiscard]] vector_type random_in_unit_sphere(Rng& rng)
    {
        for (;;)
        {
            const vector_type sample(rng.next() * 2.0F - 1.0F,
                                     rng.next() * 2.0F - 1.0F,
                                     rng.next() * 2.0F - 1.0F);
            if (mv::math::LengthSquared(sample) < 1.0F)
            {
                return sample;
            }
        }
    }

    [[nodiscard]] bool intersect_sphere(const ray_type& ray,
                                        const Sphere& sphere,
                                        scalar_type minimum_distance,
                                        scalar_type maximum_distance,
                                        Hit& out_hit)
    {
        const auto geometry = mv::math::Sphere3f::TryFromCenterRadius(
            sphere.center, sphere.radius);
        const auto intersection =
            geometry ? mv::math::Intersect(ray, *geometry) : std::nullopt;
        if (!intersection)
        {
            return false;
        }

        scalar_type distance = intersection->EntryDistance;
        std::optional<mv::math::Normal3f> outward = intersection->EntryNormal;
        if (distance < minimum_distance)
        {
            distance = intersection->ExitDistance;
            outward = intersection->ExitNormal;
        }
        if (!outward || distance < minimum_distance ||
            distance > maximum_distance)
        {
            return false;
        }

        out_hit.distance = distance;
        out_hit.position = ray.PointAt(distance);
        out_hit.front_face =
            mv::math::Dot(ray.Direction().Vector(), outward->Vector()) < 0.0F;
        out_hit.normal = out_hit.front_face ? *outward : -*outward;
        out_hit.material = sphere.material;
        return true;
    }

    [[nodiscard]] color_type sky_color(const mv::math::Direction3f& direction)
    {
        const scalar_type amount =
            std::clamp(direction.Vector().Y() * 0.5F + 0.5F, 0.0F, 1.0F);
        return color_type(0.85F, 0.92F, 1.0F) * (1.0F - amount) +
               color_type(0.35F, 0.55F, 0.95F) * amount;
    }

    [[nodiscard]] bool scatter(const ray_type& ray_in,
                               const Hit& hit,
                               Rng& rng,
                               color_type& out_attenuation,
                               ray_type& out_ray)
    {
        const scalar_type metallic =
            std::clamp(hit.material.metallic, 0.0F, 1.0F);
        const scalar_type roughness =
            std::clamp(hit.material.roughness, 0.0F, 1.0F);

        vector_type bounce =
            mv::math::Reflect(ray_in.Direction().Vector(), hit.normal);
        if (metallic < 1.0F)
        {
            bounce = hit.normal.Vector() + random_in_unit_sphere(rng);
        }
        if (roughness > 0.0F)
        {
            bounce += random_in_unit_sphere(rng) * roughness;
        }

        auto direction = mv::math::Direction3f::TryFrom(bounce);
        if (!direction ||
            mv::math::Dot(direction->Vector(), hit.normal.Vector()) <= 0.0F)
        {
            direction = hit.normal.ToDirection();
        }

        const auto scattered = ray_type::TryFromOriginDirection(
            hit.position + hit.normal.Vector() * 0.001F, *direction);
        if (!scattered)
        {
            return false;
        }
        out_attenuation = hit.material.albedo;
        out_ray = *scattered;
        return true;
    }

    [[nodiscard]] color_type trace(const ray_type& ray,
                                   const std::vector<Sphere>& scene,
                                   Rng& rng,
                                   int max_bounces)
    {
        ray_type current_ray = ray;
        color_type throughput = color_type::One();
        color_type radiance = color_type::Zero();

        for (int bounce = 0; bounce < max_bounces; ++bounce)
        {
            Hit closest_hit;
            bool found_hit = false;
            scalar_type closest_distance =
                std::numeric_limits<scalar_type>::max();

            for (const Sphere& sphere : scene)
            {
                Hit hit;
                if (intersect_sphere(current_ray, sphere, 0.001F,
                                     closest_distance, hit))
                {
                    found_hit = true;
                    closest_distance = hit.distance;
                    closest_hit = hit;
                }
            }

            if (!found_hit)
            {
                radiance += throughput * sky_color(current_ray.Direction());
                break;
            }

            radiance += throughput * closest_hit.material.emission;
            color_type attenuation;
            ray_type scattered;
            if (!scatter(current_ray, closest_hit, rng, attenuation, scattered))
            {
                break;
            }
            throughput *= attenuation;
            current_ray = scattered;
        }
        return radiance;
    }

    [[nodiscard]] std::uint8_t to_byte(scalar_type value)
    {
        const scalar_type gamma_corrected =
            std::pow(std::clamp(value, 0.0F, 1.0F), 1.0F / 2.2F);
        return static_cast<std::uint8_t>(gamma_corrected * 255.0F + 0.5F);
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
    camera.aspect_ratio =
        static_cast<scalar_type>(width) / static_cast<scalar_type>(height);
    camera.vertical_fov = mv::math::ToRadians(mv::math::Degreesf(55.0F));
    camera.transform.local_position = vector_type(0.0F, 1.4F, -6.0F);
    if (!camera.look_at(vector_type(0.0F, 1.0F, 0.0F)))
    {
        std::cerr << "Could not orient the camera\n";
        return 1;
    }

    const std::vector<Sphere> scene = {
        {point_type(0.0F, -1000.0F, 0.0F),
         1000.0F,
         {color_type(0.65F, 0.68F, 0.72F), color_type::Zero(), 1.0F, 0.0F}},
        {point_type(-1.6F, 1.0F, 1.0F),
         1.0F,
         {color_type(0.85F, 0.3F, 0.25F), color_type::Zero(), 1.0F, 0.0F}},
        {point_type(1.3F, 0.9F, 0.2F),
         0.9F,
         {color_type(0.9F, 0.92F, 0.95F), color_type::Zero(), 0.08F, 1.0F}},
        {point_type(0.0F, 3.5F, 1.5F),
         0.75F,
         {color_type::Zero(), color_type(8.0F, 7.0F, 6.0F), 0.0F, 0.0F}},
    };

    std::ofstream out(output_path, std::ios::binary);
    if (!out)
    {
        std::cerr << "Failed to open output file: " << output_path << '\n';
        return 1;
    }
    out << "P3\n" << width << ' ' << height << "\n255\n";

    Rng rng(0xC0FFEEU);
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            color_type accumulated = color_type::Zero();
            for (int sample = 0; sample < samples_per_pixel; ++sample)
            {
                const scalar_type u =
                    (static_cast<scalar_type>(x) + rng.next()) /
                    static_cast<scalar_type>(width);
                const scalar_type v =
                    (static_cast<scalar_type>(y) + rng.next()) /
                    static_cast<scalar_type>(height);
                const auto camera_ray = camera.ndc_to_world_ray(
                    mv::math::Vec2f(u * 2.0F - 1.0F, (1.0F - v) * 2.0F - 1.0F));
                if (camera_ray)
                {
                    accumulated += trace(*camera_ray, scene, rng, max_bounces);
                }
            }

            const color_type pixel =
                accumulated / static_cast<scalar_type>(samples_per_pixel);
            out << static_cast<int>(to_byte(pixel.X())) << ' '
                << static_cast<int>(to_byte(pixel.Y())) << ' '
                << static_cast<int>(to_byte(pixel.Z())) << '\n';
        }
    }

    std::cout << "Wrote " << output_path << '\n';
    return 0;
}
