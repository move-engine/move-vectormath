# move-vectormath

`move-vectormath` is Move's C++20 game and graphics math library. The current
API lives in `mv::math` and provides arithmetic vectors, invariant-bearing
semantic values, transforms, matrices, geometry, spatial queries, culling, and
explicit graphics-convention helpers.

## Use the API

Include the full public surface:

```cpp
#include <mv/math/Math.hpp>
```

or focused capability headers such as:

```cpp
#include <mv/math/Core.hpp>
#include <mv/math/Transforms.hpp>
#include <mv/math/Geometry.hpp>
#include <mv/math/Queries.hpp>
#include <mv/math/Culling.hpp>
#include <mv/math/Graphics.hpp>
```

Example:

```cpp
using namespace mv::math;

const auto direction = Direction3f::TryFrom(target - position);
if (!direction)
{
    return;
}

const RigidTransform3f localToWorld(rotation, translation);
const Point3f world = TransformPoint(localToWorld, local);

const auto bounds = Obb3f::TryFromCenterHalfExtents(
    world, Vec3f(1.0F, 2.0F, 1.0F), rotation);
```

Fallible construction is deliberate: normalized directions, normals, finite
geometry, singular inverses, projective homogeneous division, and invalid
camera bases do not silently manufacture fallback values.

## Conventions

- `+X` is right, `+Y` is up, and `+Z` is the conventional forward axis.
- Matrices use row-vector application: `value * matrix`.
- Composition functions read in application order: `Compose(first, second)`
  applies `first`, then `second`.
- Angle arguments use `Radians<T>` or `Degrees<T>` rather than untyped scalars.
- Projection construction requires explicit handedness, clip-depth range, and
  forward/reverse depth direction.
- `Point3`, `Direction3`, and `Normal3` are different semantic values.
- Packed CPU storage and exact GPU transfer layouts are explicit types rather
  than accidental properties of compute vectors.

See [CONVENTIONS.md](CONVENTIONS.md) and the organized
[API-v2 documentation](docs/api-v2/README.md).

## CMake

The canonical target for new consumers is `mv::math`:

```cmake
add_subdirectory(path/to/move-vectormath)
target_link_libraries(your_target PRIVATE mv::math)
```

The target publishes the C++20 requirement, public include directory, and the
pinned RTM dependency. `FetchContent` and CPM can add the repository in the
same way before linking `mv::math`.

## XMake

The repository owns a first-class header-only XMake target:

```lua
includes("path/to/move-vectormath")

target("your-game")
    add_deps("move-vectormath")
```

The upstream project pins the official `rtm v2.3.1` package and installs the
complete public include tree. A package repository can delegate installation
to this project and expose it through `add_requires("move-vectormath")`.

Validate the XMake consumption surface with:

```sh
xmake f -c --build_tests=y -m release -y
xmake build
xmake test -v
```

The `force_scalar` XMake option publishes `MV_MATH_FORCE_SCALAR=1` when a
portable scalar build is required.

C++ named-module work is intentionally deferred until the legacy correctness
tests and cross-library benchmarks have migrated to this API. Headers remain
the source of truth and supported public interface.

## Verification

The CMake suite exercises GCC, Clang, AppleClang, and MSVC; scalar and RTM
backends; sanitizers; generated-code fixtures; shader transfer layouts; and a
downstream consumer. XMake has independent RTM and forced-scalar header
consumer tests. Runtime benchmarks live in the separate `vectormathbench`
repository and are diagnostic rather than noisy hosted-runner regression
gates.

## License

This repository is licensed under the MIT license. See
[LICENSE.md](LICENSE.md).
