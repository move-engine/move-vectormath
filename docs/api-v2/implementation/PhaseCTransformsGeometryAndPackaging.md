# Phase C transforms, geometry, and packaging

## Scope

This slice completes the Move 1.x transform foundation and the first practical
high-level culling/graphics surface:

- general quaternion algebra and scaling-safe normalization/inversion;
- explicit-order Euler construction, look-towards, and shortest-arc Nlerp and
  Slerp for `Rotation3<T>`;
- affine composition/inverse, rigid/affine/TRS matrix conversion, and typed TRS
  decomposition with deterministic reflection and explicit shear status;
- separate inverse-transpose half-space and ordered-tangent surface-normal
  semantics under negative determinant transforms;
- float/double OBB values and ray, sphere, AABB, and OBB queries;
- explicit clip-depth/reverse-Z frustum extraction, infinite-far plane masks,
  prepared culling, and hierarchical plane-mask propagation;
- NDC/viewport project/unproject and viewport-ray construction; and
- canonical `mv::math` CMake consumption plus first-class XMake installation
  and RTM/scalar consumer tests.

## Contract choices

`TrsTransform3` is an authoring/decomposition value. It converts losslessly to
affine, but arbitrary composition remains affine because rotated nonuniform
scales can shear. Decomposition assigns the sign of a reflection to X
deterministically and reports shear rather than silently discarding it.

`TryTransformNormal` treats a normal as a plane covector and preserves its
positive half-space. `TryTransformOrientedSurfaceNormal` instead transforms the
cross product of an ordered tangent pair. The two intentionally differ by a
sign under reflection.

Frustum planes point inward. Infinite-far projections deactivate the far plane
with a mask instead of storing an invalid sentinel. Prepared culling preserves
planes that intersected a parent so child traversal can skip planes already
proved to contain it.

Viewport APIs require clip-depth, depth-direction, and screen-Y choices where
they affect interpretation. The general ray begins on the near plane and works
for perspective and orthographic matrices; a separately named perspective
helper creates an eye-origin ray.

## Inline provenance

The implementation keeps sources beside the algorithms:

- Hamilton products and conventions are cross-checked against RTM 2.3.1
  (MIT, commit `745bd25673d93b46941eda55e0993327dbc12b53b`).
- quaternion matrix conversion and spherical interpolation cite Ken Shoemake's
  original work and are cross-checked against DirectXMath (MIT).
- normal cofactors cite Eric Lengyel's derivation.
- OBB SAT cites Christer Ericson, *Real-Time Collision Detection*, section
  4.4.1; the representation is cross-checked against DirectXCollision (MIT).
- frustum plane extraction cites Gribb and Hartmann (2001).
- project/unproject behavior is cross-checked against GLM and DirectXMath
  (both MIT).
- overflow-safe quaternion norm scaling follows LAPACK's xLASSQ pattern.

No opaque provenance identifier is required to understand a kernel; the exact
reference or compatible implementation is named in its local comment.

## Verification

The fresh GCC release configuration builds all current and retained legacy
targets with project warnings treated as errors and passes 23 of 23 CTest
tests. Both new culling/graphics executables run against RTM and forced-scalar
backends. The downstream CMake consumer links the canonical `mv::math` target.

XMake independently builds and passes RTM and forced-scalar header consumers,
and a production configuration installs 56 current `mv::math` headers without
test binaries. A fresh Clang 22 build with AddressSanitizer and
UndefinedBehaviorSanitizer also passes 23 of 23 tests. Hosted compiler/OS
results remain commit-checkpoint evidence rather than being claimed in
advance.

## Sequencing

Primary current-API behavioral tests now run under Catch2 for both RTM and
forced-scalar backends. Dependency-light executables remain for smoke,
consumer, and code-generation checks. The remaining legacy-only contract audit
and benchmark migration still precede named-module work. Headers are the
implementation source of truth, allowing the module to be a thin export
wrapper once the public surface and performance evidence are stable.
