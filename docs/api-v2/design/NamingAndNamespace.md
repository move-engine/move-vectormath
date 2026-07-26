# Naming and namespace proposal

## Namespace

All primary facilities live under:

```cpp
namespace mv::math {}
```

Subnamespaces are reserved for capabilities rather than public type identity:

- `mv::math::packed` only if packed-format names become too numerous;
- `mv::math::interop::rtm` for explicit RTM conversion;
- `mv::math::interop::directx` for DirectX conversion;
- `mv::math::literals` for angle literals;
- `mv::math::detail` for non-public backend machinery.

Ordinary public types must not contain `rtm`, `simd`, `scalar`, or a backend
wrapper in their fully qualified identity.

## Identifier style

- Types, concepts, functions, methods, constants, and enum values use
  PascalCase.
- Template parameters use descriptive PascalCase names.
- Private data uses a trailing underscore.
- Preprocessor configuration uses `MV_MATH_*`.

Examples:

```cpp
mv::math::Vec3f velocity;
mv::math::Point3d worldOrigin;
mv::math::Direction3f forward = mv::math::Direction3f::Forward();

auto distanceSquared = mv::math::DistanceSquared(a, b);
auto hit = mv::math::Intersect(ray, triangle);
```

This is intentionally different from the v1 snake_case surface. No legacy
aliases are proposed.

## Type names

Dimension precedes scalar suffix:

- `Vec2f`, `Vec3f`, `Vec4f`
- `Vec2d`, `Vec3d`, `Vec4d`
- `Vec2i`, `Vec3i`, `Vec4i`
- `Point2f`, `Point3f`
- `Direction2f`, `Direction3f`
- `Normal3f`
- `Quatf`, `Rotation3f`
- `Mat3f`, `Mat4f`
- `RigidTransform3f`, `AffineTransform3f`
- `Ray3f`, `Segment3f`, `Aabb3f`, `Obb3f`, `Frustum3f`

Primary templates retain readable names such as `Vec3<T>` and `Point3<T>`.
The common aliases are the normal user-facing spellings.

Packed/storage types say so:

- `PackedVec3f`
- `PackedMat4f`
- `PackedQuatf`
- `OctahedralDirection32`

`Vec3f` must never be an alias for a packed 12-byte type on one platform and a
16-byte compute type on another.

## Operation naming

Use consistent pairs:

| Meaning | Name |
|---|---|
| exact predicate | `IsZero`, `IsFinite`, `IsUnit` |
| tolerance predicate | `IsNearlyZero`, `IsNearlyEqual` |
| explicit normalization fallback | `NormalizedOrZero` |
| mutating fallible normalization | `NormalizeInPlace` |
| non-mutating value | `ClampedLength` |
| mutating operation | `ClampLength` |
| squared quantity | `LengthSquared`, `DistanceSquared` |
| one-shot boolean query | `Intersects` |
| three-state query | `Classify` |
| detailed query | `Intersect` |
| fallible creation | `TryFrom...`, `TryNormalize` |
| explicit fallback | `FromOr` |
| preconditioned operation | `...Unchecked` only when public and justified |

Use full words where they materially improve discovery. `LengthSquared` is
preferred over competing `LengthSqr`, `SizeSquared`, and `Mag2` synonyms.

## Members versus free functions

- component access and inexpensive observations are members;
- named construction and canonical constants are static members;
- arithmetic operators are non-member friends where appropriate;
- symmetric/cross-type algorithms are free functions;
- detailed geometry queries are free functions;
- backend and external-library conversions live in adapter namespaces.

Examples:

```cpp
v.LengthSquared();
auto direction = Direction3f::TryFrom(v);
auto fallbackValue = NormalizedOrZero(v);
Rotation3f::FromAxisAngle(axis, 90.0_deg);
Distance(pointA, pointB);
TransformPoint(transform, point);
Intersect(ray, triangle);
```

## Familiar names and explicit contracts

Established Unity, Unreal, Godot, Source, DirectXMath, and shader terminology
is useful for discoverability, but is not a compatibility contract. Retain
familiar names such as `Dot`, `Cross`, `MoveTowards`, `TransformPoint`, and
`PointAt` when they accurately describe the operation.

Add a qualifier when it exposes behavior that would otherwise be surprising:

- `Lerp` is the unclamped mathematical operation; `LerpClamped` opts into
  clamping;
- exact `operator==` is separate from `IsNearlyEqual`;
- `TryNormalize`/`TryFrom` reports failure and `NormalizedOrZero` names its
  fallback;
- `TryProjectPoint` exposes projective homogeneous division and failure;
- `InflatedBy` distinguishes a per-side margin from a total size change.

Do not copy a familiar name when it hides which transform components are
applied. Overload sets for point, vector, direction, and normal transforms are
constrained by semantic type. The detailed policy lives in
[`Approachability.md`](Approachability.md).

## Angle naming

Raw scalar angle parameters are avoided in primary APIs:

```cpp
Radiansf
Degreesf
Rotation3f::FromAxisAngle(Direction3f axis, Radiansf angle);
Rotation3f::FromEuler(EulerAngles3f angles);
```

`EulerAngles3f` includes an explicit order. Convenience literals live in
`mv::math::literals`.

## Coordinate-specific names

Functions that depend on handedness or clip policy must state or accept it:

```cpp
LookAtLH(...)
LookAtRH(...)
PerspectiveFov(..., Handedness::Left, ClipDepth::ZeroToOne)
Frustum3f::FromViewProjection(matrix, ClipConvention::DirectX)
```

The default direction constants retain Move's established +X right, +Y up,
+Z forward convention, but projection/view construction does not rely on an
unstated default.
