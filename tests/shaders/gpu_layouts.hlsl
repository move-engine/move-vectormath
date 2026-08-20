struct PositionRadius16
{
    float3 Position;
    float Radius;
};

struct Aabb3fSlots16
{
    float3 Minimum;
    float MinimumPadding;
    float3 Maximum;
    float MaximumPadding;
};

StructuredBuffer<PositionRadius16> Positions : register(t0);
StructuredBuffer<Aabb3fSlots16> Bounds : register(t1);

cbuffer MathConstants : register(b0)
{
    float3 CameraPosition;
    float Exposure;
    row_major float3x3 NormalMatrix;
};

float4
main(uint vertexId : SV_VertexID) : SV_Position
{
    const PositionRadius16 item = Positions[vertexId];
    const Aabb3fSlots16 bounds = Bounds[vertexId];
    const float3 center = (bounds.Minimum + bounds.Maximum) * 0.5F;
    return float4(
        mul(item.Position - CameraPosition + center, NormalMatrix),
        item.Radius + Exposure);
}
