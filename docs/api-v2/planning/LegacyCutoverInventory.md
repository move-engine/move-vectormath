# Legacy-to-`mv::math` cutover inventory

This inventory is the deletion gate for the public `move::math` header tree.
It records capabilities, not spelling-compatible aliases: the replacement API
may deliberately change contracts, but Move 1.x must not silently lose required
game and graphics functionality.

Status meanings:

- **Ready:** the replacement API, behavior tests, and focused compilation
  coverage exist.
- **Partial:** a usable replacement exists, but important legacy capabilities
  or migration evidence are missing.
- **Missing:** the capability still depends on `move::math`.
- **Remove:** the legacy facility is intentionally not carried forward.

Performance is a separate gate. Every hot capability requires identical-input
`mv::math` versus `move::math` benchmarks on SSE4.2, AVX, and AVX2 before its
legacy implementation is deleted. Invariant validation and other boundary
construction costs are measured separately from prevalidated hot paths.

## Core values

| Capability | Status | Cutover requirement |
| --- | --- | --- |
| `Vec2f`, `Vec3f`, `Vec4f` storage and basic arithmetic | Ready | Direct legacy/new performance rows and complete operator consistency audit. |
| Double vectors | Ready | Float/double aliases share the public contracts; keep backend-specific performance expectations explicit. |
| Integral vectors | Ready | Fixed-width aliases and arithmetic-domain constraints are published and correctness-tested. |
| Dot, cross, squared length | Ready | Consistent 2D/3D/4D coverage and distance helpers exist. |
| Length, distance, normalization | Ready | Floating-point vector metrics and scaling-safe invariant-bearing normalization exist. |
| Interpolation, clamp, reflect/refract, angle helpers | Partial | Practical interpolation, clamp, reflect, refraction, and typed angles exist; add only workload-driven scalar helpers. |
| Scalar helpers and constants | Missing | Replace useful `common.hpp` facilities with PascalCase functions/constants or standard-library guidance. |
| Packed vectors and explicit load/store | Ready | Keep layout tests and batch conversion benchmarks. |

## Rotation and transforms

| Capability | Status | Cutover requirement |
| --- | --- | --- |
| General quaternion value | Partial | Multiplication, scaling-safe normalization/inverse, and comparison exist; add only workload-driven exponential/logarithmic functions. |
| Valid rotation value | Ready | Axis-angle, explicit-order Euler construction, look-towards, composition, inverse, shortest-arc Nlerp/Slerp, and vector/direction rotation exist. |
| Rigid transform | Ready | Float/double semantic operations and matrix/affine conversions exist; retain direct migration benchmarks. |
| Affine transform | Ready | Point/vector/normal operations, application-order composition, fallible inverse, and matrix conversion exist. |
| TRS authoring/decomposition | Ready | Typed float/double TRS, reflection-aware decomposition status, shear rejection, and affine/matrix conversion exist. |
| General `Mat3f` | Partial | Identity, access, multiplication, vector transform, transpose, determinant, fallible inverse, scale, and rotation conversion exist; add packed/GPU transfer and finish direct legacy performance rows. |
| General `Mat4f` | Ready | Identity, access, multiplication, homogeneous transform, transpose, determinant, fallible inverse, affine/rigid/TRS conversion, and direct legacy benchmarks exist. |
| View/projection construction | Ready | Fallible look-at, finite/infinite perspective, orthographic, handedness, clip-depth, and forward/reverse-Z policies are explicit and tested. |

## Geometry and queries

| Capability | Status | Cutover requirement |
| --- | --- | --- |
| Point, direction, normal semantics | Ready | Float/double aliases preserve invariant-bearing zero-overhead contracts. |
| Line, ray, segment, plane, triangle | Ready | Float/double aliases and query kernels are correctness-tested. |
| Sphere, capsule, AABB | Ready | Float/double continuous geometry and integer discrete AABBs exist; add utilities only when workloads require them. |
| Ray/plane, ray/triangle, ray/sphere, ray/AABB | Ready | Float/double kernels exist; retain matching-semantic external benchmark rows. |
| Closest-point queries | Ready | Float/double kernels exist; split broad result declarations if focused-header measurements justify it. |
| OBB and OBB queries | Ready | Float/double OBBs, ray/sphere/AABB/OBB tests, SAT, support/corners/closest point, and conservative affine AABB conversion exist. |
| Frustum extraction and culling | Ready | Explicit clip-depth/reverse-Z extraction, infinite-far masks, prepared point/sphere/AABB/OBB classification, and hierarchical plane masks exist. |
| Project/unproject and camera rays | Ready | NDC/viewport projection, inverse-aware unprojection, explicit Y/depth conventions, and near-plane/perspective camera rays exist. |

## Packaging and public surface

| Capability | Status | Cutover requirement |
| --- | --- | --- |
| Capability-oriented headers | Ready | Keep focused headers as the implementation source of truth. |
| `Math.hpp` umbrella | Ready | Track focused and umbrella compilation cost. |
| CMake consumption | Ready | `mv::math` is canonical for new consumers; remove transitional legacy aliases at final cutover. |
| XMake header consumption | Ready | The upstream XMake target installs headers, pins RTM 2.3.1, and tests RTM/scalar consumers. |
| `mv.math` C++20 module | Missing | Deliberately scheduled after legacy tests and cross-library benchmarks migrate (item 7); headers remain the source of truth. |
| User documentation | Ready | README and conventions teach the current namespace, semantic contracts, focused headers, and CMake/XMake usage. |
| Legacy test migration | Partial | Current behavior suites run under Catch2 for RTM and forced-scalar backends; audit remaining legacy-only contracts before deleting the old tests. |
| Cross-library benchmark migration | Partial | New capability suites exist, but the main suite still benchmarks the old surface. |

## Intentionally removed legacy design

- Backend identity in ordinary public type names (`fast_*`, scalar/RTM wrapper
  types, acceleration template arguments).
- Snake-case duplicate aliases and method spellings.
- Infallible matrix inverse for singular inputs.
- Projection/view constructors with implicit handedness or clip conventions.
- Ambiguous point/vector/direction transformation helpers.
- Direct extensions in the third-party `rtm` namespace.
- Disabled or prototype artifacts in the installed include tree.

## Deletion gate

Delete `packages/move/math/include/move/` only when:

1. required **Missing** items have replacements or an explicit removal
   decision;
2. legacy tests have equivalent replacement-contract coverage;
3. direct old/new runtime benchmarks have no unexplained regression;
4. focused-header, umbrella-header, and module compile-time measurements meet
   their acceptance criteria;
5. README, conventions, CMake/XMake consumers, and Move integration use the
   replacement surface; and
6. the benchmark repository no longer requires the live legacy implementation
   (historical results may remain archived).
