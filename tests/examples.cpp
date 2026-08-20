#include <cstdlib>

#include "../packages/move/math_examples/src/camera_component.hpp"
#include "../packages/move/math_examples/src/character_controller.hpp"

namespace
{
    void Require(bool condition)
    {
        if (!condition)
        {
            std::abort();
        }
    }

    class EmptyWorld final : public examples::CharacterPhysicsWorld<float>
    {
    public:
        bool sweep_capsule(const vector_type&,
                           float,
                           float,
                           const vector_type&,
                           examples::CharacterSweepHit<float>&) const override
        {
            return false;
        }

        bool depenetrate_capsule(const vector_type&,
                                 float,
                                 float,
                                 vector_type&) const override
        {
            return false;
        }

        bool probe_ground(const vector_type&,
                          float,
                          float,
                          float,
                          vector_type&) const override
        {
            return false;
        }
    };
}  // namespace

int main()
{
    using mv::math::Vec2f;
    using mv::math::Vec3f;

    examples::TransformComponent<float> parent;
    parent.local_position = Vec3f(2.0F, 0.0F, 0.0F);
    examples::TransformComponent<float> child;
    child.parent = &parent;
    child.local_position = Vec3f(1.0F, 0.0F, 0.0F);
    Require(child.world_position() == Vec3f(3.0F, 0.0F, 0.0F));
    Require(child.try_set_world_position(Vec3f(5.0F, 0.0F, 0.0F)));
    Require(child.local_position == Vec3f(3.0F, 0.0F, 0.0F));

    examples::CameraComponent<float> camera;
    camera.transform.local_position = Vec3f(0.0F, 0.0F, -4.0F);
    Require(camera.look_at(Vec3f::Zero()));
    Require(camera.view_matrix().has_value());
    Require(camera.projection_matrix().has_value());
    Require(camera.view_projection_matrix().has_value());
    Require(camera.ndc_to_world_ray(Vec2f::Zero()).has_value());

    EmptyWorld world;
    examples::CollideAndSlideCharacterController<float> character;
    Require(
        character.integrate(world, 1.0F / 60.0F, Vec3f(0.0F, -9.81F, 0.0F)));
}
