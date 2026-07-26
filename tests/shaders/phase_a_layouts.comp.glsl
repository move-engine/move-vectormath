#version 450

struct PositionRadius16
{
    vec3 Position;
    float Radius;
};

struct Aabb3fSlots16
{
    vec3 Minimum;
    float MinimumPadding;
    vec3 Maximum;
    float MaximumPadding;
};

layout(std430, set = 0, binding = 0) readonly buffer PositionBuffer
{
    PositionRadius16 Positions[];
};

layout(std430, set = 0, binding = 1) readonly buffer BoundsBuffer
{
    Aabb3fSlots16 Bounds[];
};

layout(std430, set = 0, binding = 2) writeonly buffer OutputBuffer
{
    vec4 OutputValues[];
};

layout(local_size_x = 64) in;

void
main()
{
    const uint index = gl_GlobalInvocationID.x;
    const vec3 center =
        (Bounds[index].Minimum + Bounds[index].Maximum) * 0.5;
    OutputValues[index] =
        vec4(Positions[index].Position + center, Positions[index].Radius);
}
