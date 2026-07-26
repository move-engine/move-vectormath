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

Naming this limitation is preferable to Unreal/Godot-style transform APIs that
look general while scale composition has caveats.

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

## Projective matrices

Perspective projection is not affine. It remains a `Mat4` or a dedicated
projection value. Project/unproject functions must make homogeneous division
and clip convention explicit and can fail when W is unsuitable.

Do not put projective behavior into `AffineTransform3`.

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

