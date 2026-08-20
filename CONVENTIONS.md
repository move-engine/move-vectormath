# Math conventions

## Coordinate and matrix conventions

- `+X` is right, `+Y` is up, and `+Z` is the conventional forward axis.
- The core does not force every camera or projection to be left-handed.
  Projection and view builders require an explicit `Handedness`.
- Matrices are stored and presented by rows.
- `Vector * Matrix` applies a matrix using Move's row-vector convention.
- `Compose(first, second)` applies `first` and then `second`.
- Affine translation occupies the fourth row of `Mat4` and the homogeneous
  fourth column is `(0, 0, 0, 1)`.

## Semantic values

- `Point3` is a location. Point minus point is a displacement; adding two
  points is unavailable.
- `Direction3` is always finite, unit length, and nonzero.
- `Normal3` is a unit surface/plane covector and is distinct from direction.
- `Rotation3` stores a unit quaternion. General, possibly non-unit quaternion
  algebra uses `Quat`.
- Construction that cannot preserve an invariant uses `Try...` and reports
  failure.

## Transforms

- `RigidTransform3` is rotation plus translation and is closed under
  composition and inversion.
- `TrsTransform3` is an authoring/decomposition value. Arbitrary TRS
  composition can create shear, so its general composition result is affine.
- `AffineTransform3` represents a linear transformation plus translation and
  is closed under composition. Inversion can fail for singular inputs.
- `TransformPoint` applies translation. `TransformVector` does not.
- A general affine direction transformation is fallible because it must
  restore unit length.
- `TryTransformNormal` uses inverse-transpose/covector semantics and preserves
  the transformed positive half-space under reflections.
- `TryTransformOrientedSurfaceNormal` transforms the cross product of an
  ordered tangent pair and therefore differs by a sign under reflection.

## Rotations and angles

- Angles use `Radians<T>` and `Degrees<T>` wrappers.
- Euler construction requires an explicit `EulerOrder`; the order names the
  sequence applied to a local vector.
- `Nlerp` and `Slerp` take the shortest quaternion arc and clamp the amount to
  `[0,1]`. Their `Unclamped` variants expose extrapolation.
- Quaternion component equality is exact. `IsNearlyEquivalent(Rotation3,
  Rotation3)` treats `q` and `-q` as the same rotation.

## Projection, depth, and viewport conventions

- Perspective and orthographic constructors require `Handedness`,
  `ClipDepth`, and `DepthDirection`.
- Both `[0,1]` and `[-1,1]` normalized-device depth are supported.
- Forward and reverse Z, finite and infinite far perspective, and intentional
  orthographic axis reversal are explicit.
- `TryProjectPoint` returns normalized-device coordinates and exposes invalid
  homogeneous division.
- Viewport conversion requires an explicit clip-depth convention; screen Y can
  be declared up or down.
- `TryMakeViewportRay` begins at the near plane and works for perspective or
  orthographic projections. `TryMakePerspectiveViewportRay` accepts the camera
  origin when an eye-origin ray is desired.

## Geometry and culling

- Linear primitive parameters use physical distance when their direction is a
  `Direction3`.
- AABB and OBB half-extents are nonnegative. Touching boundaries intersect.
- Frustum planes point inward. Infinite-far frusta deactivate the far plane
  rather than storing an invalid sentinel.
- Culling is conservative. Plane-mask propagation retains only planes that
  intersected a parent bound.

## Representations and backends

- Compute types select scalar or RTM primitives privately; backend identity is
  not part of ordinary public type names.
- `Vec2f` stays compact and scalar-backed so array loops can vectorize across
  values. `Vec3f` and `Vec4f` may retain SIMD-native storage.
- Packed CPU and GPU transfer representations name their exact byte layout.
  A compute vector is not described as universally GPU-blittable.
- Define `MV_MATH_FORCE_SCALAR=1`, or use XMake's `force_scalar` option, to
  select the portable scalar backend consistently for a target graph.
