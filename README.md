# move-vectormath

`move-vectormath` is the Move math library for vectors, matrices, quaternions,
and scalar math helpers.

It can be consumed either:

- as a Move package (`move/math`)
- as a plain CMake dependency via `add_subdirectory`, `FetchContent`, `CPM`,
  or similar workflows

## Layout

- Public umbrella header: [packages/move/math/include/move/vectormath.hpp](packages/move/math/include/move/vectormath.hpp)
- Core public headers: [packages/move/math/include/move/math](packages/move/math/include/move/math)
- Tests: [packages/move/math/tests](packages/move/math/tests)
- Math conventions: [CONVENTIONS.md](CONVENTIONS.md)

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

See [CONVENTIONS.md](CONVENTIONS.md)
for coordinate-system, transform, and rotation semantics.

## Integration

### Move Package

In a Move project, depend on `move/math` and include the umbrella header:

```cpp
#include <move/vectormath.hpp>
```

Typical Move CLI workflows from the repository root are:

```bash
move-cli build move/math
move-cli test move/math
```

### CMake

The top-level CMake project exports an interface target named
`move-vectormath` with aliases:

- `move::vectormath`
- `move::math`
- `move-math`

The recommended link target for consumers is `move::vectormath`.

#### `add_subdirectory`

```cmake
add_subdirectory(path/to/move-vectormath)

target_link_libraries(your_target
  PRIVATE
    move::vectormath
)
```

#### `FetchContent`

```cmake
include(FetchContent)

FetchContent_Declare(
  move_vectormath
  GIT_REPOSITORY https://github.com/move-engine/move-vectormath.git
  GIT_TAG main
)

FetchContent_MakeAvailable(move_vectormath)

target_link_libraries(your_target
  PRIVATE
    move::vectormath
)
```

#### `CPM`

```cmake
include(cmake/CPM.cmake)

CPMAddPackage(
  NAME move-vectormath
  GITHUB_REPOSITORY move-engine/move-vectormath
  GIT_TAG main
)

target_link_libraries(your_target
  PRIVATE
    move::vectormath
)
```

After linking, include the public umbrella header:

```cpp
#include <move/vectormath.hpp>
```

or the specific headers under `move/math` if you prefer.

## Testing

From the repository root:

```bash
move-cli test
```

The test sources live under
[packages/move/math/tests](packages/move/math/tests).

## License

This repository is licensed under the MIT license. See
[LICENSE.md](LICENSE.md).
