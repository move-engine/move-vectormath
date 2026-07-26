# V1 type-layout baseline

Measurements were taken from the permanent v1 baseline on x86-64 using the
default RTM-enabled configuration.

| Type | Size (bytes) | Alignment (bytes) | Intended role |
|---|---:|---:|---|
| `float2` | 8 | 4 | two-component vector |
| `float3` | 16 | 16 | fast three-component float vector |
| `vec3f` | 12 | 4 | scalar/storage three-component float vector |
| `float4` | 16 | 16 | fast four-component float vector |
| `quatf` | 16 | 16 | float quaternion |
| `float4x4` | 64 | 16 | fast float matrix |
| `storage_float4x4` | 64 | 4 | packed/alignment-light float matrix |

Representative vector, quaternion, compute-matrix, and storage-matrix types
were not trivially copyable. V2 should make value and storage types trivially
copyable unless a specific invariant requires controlled construction.

## V2 acceptance questions

- Does the default `Vec3f` have one unambiguous representation?
- Are packed 12-byte vectors named distinctly from SIMD compute vectors?
- Are sizes and alignments stable and asserted in tests?
- Can compute values cross API/ABI or serialization boundaries only through
  deliberate, documented types?
- Do invariant types such as `Direction3f` have the same runtime footprint as
  their underlying compute representation?
- Are component offsets, array stride, and matrix major order documented for
  every transfer type?
- Can a 16-byte compute array be uploaded directly to a compatible shader
  layout without repacking?
- Are 12-byte packed, 16-byte compute, and GPU-layout types kept distinct when
  their array/structure contracts differ?
- Can interleaved storage be processed through strided/fused kernels without a
  gather into a temporary compute array?
