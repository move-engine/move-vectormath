# Phase A shader layout fixtures

These fixtures mirror the C++ transfer types in
`mv/math/gpu/ShaderLayouts.hpp`.

Expected layout:

| Type/member | Offset | Size/stride |
| --- | ---: | ---: |
| `PositionRadius16.Position` | 0 | 12 |
| `PositionRadius16.Radius` | 12 | 4 |
| `PositionRadius16` array element | 0 | 16 |
| `Aabb3fSlots16.Minimum` | 0 | 16-byte slot |
| `Aabb3fSlots16.Maximum` | 16 | 16-byte slot |
| `Aabb3fSlots16` array element | 0 | 32 |
| standalone float3 array element | 0 | 16 |
| row-major float3x3 row | 0, 16, 32 | 16-byte stride |

The C++ tests always assert the host side. CI should compile and reflect the
HLSL, GLSL/SPIR-V, and WGSL fixtures when the corresponding shader compilers
are available. The math headers do not depend on those tools.
