# Error and invariant policy

## General policy

The math library does not throw exceptions for numeric or geometric failure.
It uses value results, `bool`, `std::optional`, and small status enums.
No operation allocates unless an explicitly higher-level container API says so.

## Construction patterns

Use one of four patterns:

1. infallible constructor for values that cannot violate an invariant;
2. `TryFrom...` returning `std::optional<T>` for arbitrary runtime data;
3. `FromOr` when a caller supplies an explicit fallback;
4. internal or conspicuously tagged unchecked construction for values proven by
   an audited algorithm.

Examples:

```cpp
auto direction = Direction3f::TryFrom(input);
auto rotation = Rotation3f::TryFrom(rawQuaternion);
auto box = Aabb3f::TryFromMinMax(minimum, maximum);
```

Factories reject NaN/infinity unless the type explicitly supports a canonical
empty/infinite sentinel.

## Normalization

Generic vector normalization must expose failure:

```cpp
std::optional<Direction3f> TryNormalize(Vec3f value);
Vec3f NormalizedOrZero(Vec3f value);
bool NormalizeInPlace(Vec3f& value);
```

The convenience fallback is named `OrZero`; it is not hidden in an ambiguous
`Normalized` contract.

Direction/normal creation defines a minimum usable magnitude based on numeric
range and a documented tolerance. Valid tiny nonzero vectors should not be
discarded merely because their components are below a generic epsilon; scaled
normalization avoids avoidable underflow/overflow.

## Invariant drift

`Direction3`, `Normal3`, and `Rotation3` have private storage and no arbitrary
component mutation.

Operations returning invariant types must preserve or restore the invariant:

- rotation composition renormalizes according to a measured stable strategy;
- rotating a direction by a valid rotation may use the mathematically closed
  path plus debug drift checks;
- interpolation always normalizes as required;
- affine normal transformation normalizes and reports singular failure.

The permitted unit-length error is part of `Tolerance<T>` and is tested over
long operation chains.

## Tolerance

Avoid one global epsilon. Provide a lightweight policy:

```cpp
template <Floating T>
struct Tolerance
{
    T Absolute;
    T Relative;
    T UnitLength;
    T Parallel;
};
```

Functions accept only the relevant scalar/policy and have documented defaults.
Scale-sensitive comparisons combine absolute and relative tolerance.
Geometric boundary tolerance is interpreted in the units of the query.

Exact equality remains exact. NaNs never compare nearly equal; identically
signed infinities can compare equal only in scalar approximate comparison, not
in invariant construction.

## Singular and degenerate inputs

- matrix/affine inverse is fallible;
- look rotation is fallible when a usable basis cannot be formed, with a
  separate `FromOr` convenience path;
- triangle normal/barycentric operations define degenerate behavior;
- zero-length segment remains a valid point-like segment;
- negative radius/half-extents are rejected unless an explicit absolute-value
  factory is used;
- AABB has one canonical empty state;
- project/unproject is fallible when homogeneous W prevents a valid result.

## Query boundaries

By default:

- touching counts as intersection;
- ray origin (`t == 0`) and both segment endpoints are included;
- a ray starting inside a volume reports `StartsInside`;
- coplanar ray/plane has no unique point hit and returns a distinct status if
  the detailed API needs to distinguish it;
- back faces are included unless culling is explicitly requested.

## Assertions and diagnostics

Validation builds may assert invariant assumptions inside unchecked/internal
paths. Release behavior cannot depend on assertions.

Optional diagnostics provide:

- `IsFinite`;
- `IsValid` on invariant/geometry types;
- invariant error measurements;
- status-returning construction overloads for tools.

These live in focused headers or compile-time validation modes and do not add
fields to value types.

## Determinism

Cross-platform bitwise determinism is not implied by ordinary SIMD math.
If deterministic math is added later, it should be an explicit policy/module
with separate benchmarks and contracts rather than weakening or silently
changing the default optimized path.

