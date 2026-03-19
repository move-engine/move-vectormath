# move-vectormath

`move-vectormath` is the Move math library for vectors, matrices, quaternions,
and scalar math helpers.

## Layout

- Public umbrella header: [packages/move/math/include/move/vectormath.hpp](/home/claire/src/move-engine/move-vectormath/packages/move/math/include/move/vectormath.hpp)
- Core public headers: [packages/move/math/include/move/math](/home/claire/src/move-engine/move-vectormath/packages/move/math/include/move/math)
- Tests: [packages/move/math/tests](/home/claire/src/move-engine/move-vectormath/packages/move/math/tests)
- Math conventions: [CONVENTIONS.md](/home/claire/src/move-engine/move-vectormath/CONVENTIONS.md)

## Public Surface

The umbrella header exports:

- `vec2`, `vec3`, `vec4`
- `quat`
- `mat3x3`, `mat4x4`
- common math helpers from `move::math`

Backend behavior is intentionally mixed:

- `vec2` is currently scalar-only.
- Floating-point `vec3`, `vec4`, matrices, and quaternions use the RTM-backed
  path where applicable.
- Integral vector types use the scalar path.

## Usage

```cpp
#include <move/vectormath.hpp>

using namespace move::math;

float3 a(1.0f, 2.0f, 3.0f);
float3 b(4.0f, 5.0f, 6.0f);
float3 sum = a + b;

quatf rotation = quatf::rotation_y(deg2rad(90.0f));
float3 rotated = float3::forward() * rotation;
```

See [CONVENTIONS.md](/home/claire/src/move-engine/move-vectormath/CONVENTIONS.md)
for coordinate-system, transform, and rotation semantics.

## Testing

From the repository root:

```bash
move-cli test
```

The test sources live under
[packages/move/math/tests](/home/claire/src/move-engine/move-vectormath/packages/move/math/tests).

## License

This repository is licensed under the MIT license. See
[LICENSE.md](/home/claire/src/move-engine/move-vectormath/LICENSE.md).
