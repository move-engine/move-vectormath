# Transform type proposal

## Why more than one transform

A general 4x4 matrix can represent every common transform but hides useful
structure. Conversely, a translation/rotation/scale tuple is convenient for
authoring but is not closed under composition when non-uniform scales with
different orientations introduce shear.

V2 should name the capabilities rather than offering one ambiguous
`Transform`.

## RigidTransform3

```cpp
template <Floating T>
class RigidTransform3
{
    Rotation3<T> Rotation;
    Vec3<T> Translation;
};
```

Represents rotation plus translation only. Properties:

- identity default;
- compact representation;
- closed under composition;
- inverse uses quaternion conjugate plus rotated translation;
- preserves distance, direction, normal, angles, and handedness;
- transforms points, vectors, directions, normals, rays, lines, segments,
  planes, spheres, OBBs, and frusta through specialized overloads.

Named calls make semantic behavior obvious:

```cpp
Point3f worldPoint = TransformPoint(localToWorld, localPoint);
Direction3f worldDirection = TransformDirection(localToWorld, localDirection);
Normal3f worldNormal = TransformNormal(localToWorld, localNormal);
```

Operator syntax may be provided only after composition order is unambiguous in
tests/documentation. Named operations remain canonical.

## SimilarityTransform3

An optional but useful middle type stores uniform scale, rotation, and
translation. It preserves angles and maps spheres to spheres. Composition and
inverse remain cheap and closed when scale is nonzero.

This type should be added if renderer/scene workloads show frequent uniform
scale. Otherwise it can wait; its semantic slot should be reserved.

## TrsTransform3

```cpp
template <Floating T>
class TrsTransform3
{
    Vec3<T> Translation;
    Rotation3<T> Rotation;
    Vec3<T> Scale;
};
```

This is an authoring/decomposition representation, not a closed transform
algebra. Composition returns `AffineTransform3` unless a specialized checked
operation proves the result remains TRS without shear.

Naming this limitation is preferable to Unity/Unreal/Godot-style transform
APIs that look general while scale composition has caveats.

## AffineTransform3

```cpp
template <Floating T>
class AffineTransform3
{
    Mat3<T> Linear;
    Vec3<T> Translation;
};
```

Represents rotation, non-uniform scale, shear, reflection, and translation.
Properties:

- compact 3x4-equivalent data;
- closed under composition;
- point/vector transform without a homogeneous divide;
- normal transform through inverse-transpose;
- fallible inverse for singular linear parts;
- explicit conversion to `Mat4`.

Transforming a `Direction3` through a general affine transform is fallible or
renormalizing because length may change. Transforming a `Normal3` is fallible
for singular transforms.

## No ambiguous transform helpers

Move retains familiar transform vocabulary, but overload availability is part
of the contract:

```cpp
Point3f TransformPoint(AffineTransform3f transform, Point3f point);
Vec3f TransformVector(AffineTransform3f transform, Vec3f vector);
Direction3f TransformDirection(RigidTransform3f transform,
                               Direction3f direction);
std::optional<Normal3f> TransformNormal(AffineTransform3f transform,
                                       Normal3f normal);
```

`TransformVector` applies the full linear part. It does not apply translation.
`TransformDirection` is provided when the transform structurally preserves the
unit-direction invariant. A general affine overload must not silently ignore
scale or shear merely to preserve length. The caller can transform to a vector
and explicitly normalize, or provide/extract a rotation.

Normals are never treated as ordinary directions under nonuniform scale.
`TransformNormal` uses the inverse transpose, restores the unit invariant, and
reports singular failure.

These rules deliberately refine Unity's familiar `TransformPoint`,
`TransformVector`, and `TransformDirection` names. The names provide an on-ramp;
the types prevent their documented caveats from becoming hidden policy.

## Projective matrices

Perspective projection is not affine. It remains a `Mat4` or a dedicated
projection value. Project/unproject functions must make homogeneous division
and clip convention explicit and can fail when W is unsuitable.

Do not put projective behavior into `AffineTransform3`.
Do not encode the distinction with an implementation-shaped name such as
`MultiplyPoint3x4`; use `TransformPoint` for affine values and an explicitly
fallible operation such as `TryProjectPoint` for projective matrices.

## Scene hierarchy is not a math value

These transform types contain no parent, cached world state, dirty flag, or
reparenting behavior. Unity's scene `Transform` makes these concerns feel like
one facility, but hierarchy mutation and its local/world preservation policy
belong in an engine scene API.

If such an API is added elsewhere, reparenting should require an explicit
policy such as `ReparentPolicy::PreserveWorld` or
`ReparentPolicy::PreserveLocal`. It should not use a defaulted boolean whose
effects include changing local translation, rotation, or scale.

## Composition order

The existing row-vector convention is retained. Documentation and compile-time
examples will define:

```cpp
Point3f world = TransformPoint(localToWorld, local);
RigidTransform3f localToClip = Compose(localToWorld, worldToClip);
```

The exact argument wording of `Compose` must be chosen to read in application
order. Ambiguous bare multiplication should not be the only composition API.

## Conversion policy

- rigid to similarity/affine/matrix is lossless and explicit or named;
- TRS to affine/matrix is lossless and named;
- affine/matrix to rigid or TRS is fallible and returns decomposition details;
- matrix to affine validates the projective row/column;
- narrowing between float and double is explicit.

Decomposition returns a typed object containing translation, rotation, scale,
shear/reflection information, and status rather than several output
parameters.
