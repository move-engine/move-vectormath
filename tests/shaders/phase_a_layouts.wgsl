struct PositionRadius16 {
    Position: vec3<f32>,
    Radius: f32,
}

struct Aabb3fSlots16 {
    Minimum: vec3<f32>,
    MinimumPadding: f32,
    Maximum: vec3<f32>,
    MaximumPadding: f32,
}

@group(0) @binding(0)
var<storage, read> Positions: array<PositionRadius16>;

@group(0) @binding(1)
var<storage, read> Bounds: array<Aabb3fSlots16>;

@group(0) @binding(2)
var<storage, read_write> OutputValues: array<vec4<f32>>;

@compute @workgroup_size(64)
fn Main(@builtin(global_invocation_id) id: vec3<u32>) {
    let index = id.x;
    let center = (Bounds[index].Minimum + Bounds[index].Maximum) * 0.5;
    OutputValues[index] =
        vec4<f32>(Positions[index].Position + center, Positions[index].Radius);
}
