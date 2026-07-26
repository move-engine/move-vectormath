# Semantic type proposal

## Goal and guarantee

Semantic types distinguish affine and unit-length roles that generic vectors
cannot express. They carry no runtime tag and are intended to have the same
size/alignment as their underlying compute value.

For floating-point invariants, "guaranteed" means:

1. arbitrary runtime data enters only through validated/fallible construction;
2. public operations cannot mutate the value into an invalid state;
3. closed operations return the invariant type only when their implementation
   preserves/re-establishes the invariant within the documented tolerance;
4. debug validation can detect drift at subsystem boundaries.

It does not mean the compiler proves `x*x + y*y + z*z == 1` as exact real
arithmetic.

## Point

```cpp
template <Floating T>
class Point3;
```

`Point3` represents a location in affine space. Its legal algebra is:

| Expression | Result |
|---|---|
| `Point - Point` | `Vec` displacement |
| `Point + Vec` | `Point` |
| `Point - Vec` | `Point` |
| `Lerp(Point, Point, t)` | `Point` |
| `Distance(Point, Point)` | scalar |

Point plus point, point scaling, normalization, and dot/cross are omitted.
Explicit conversion to/from `Vec3` exists for interop/authoring boundaries.

This makes matrix translation semantics structural and prevents accidental
normalization of world positions.

## Direction

```cpp
template <Floating T>
class Direction3;
```

`Direction3` stores a unit vector. It has no public component setters.
Construction paths:

```cpp
std::optional<Direction3f> Direction3f::TryFrom(Vec3f value);
Direction3f Direction3f::FromOr(Vec3f value, Direction3f fallback);
Direction3f Direction3f::AxisX();
Direction3f Direction3f::Forward();
```

An internal `AssumeNormalized` constructor is available to audited algorithms;
if a public unchecked constructor exists, it requires an explicit tag and
asserts in validation builds.

Closed operations include negation and rotation by `Rotation3`. Arbitrary
component arithmetic returns `Vec3`, not `Direction3`. Cross product of
directions returns a vector because parallel inputs produce zero; a separate
`TryPerpendicularDirection` can normalize and validate.

Using `Direction3`:

- ray parameter is physical distance;
- projecting onto it avoids division by squared length;
- reflection and plane operations do not normalize again;
- basis and look-rotation construction can state precise preconditions.

## Normal

```cpp
template <Floating T>
class Normal3;
```

`Normal3` is also unit length but is distinct because transformation differs
under non-uniform scale. A rigid transform treats it like a direction. An
affine transform uses inverse-transpose linear transformation followed by
normalization and can fail for singular transforms.

Normal construction mirrors direction construction. Explicit conversion
between `Direction3` and `Normal3` is cheap but named, making semantic intent
visible.

Operations such as reflection and plane construction take `Normal3`; travel
and orientation APIs take `Direction3`.

## Rotation

```cpp
template <Floating T>
class Rotation3;
```

`Rotation3` stores a normalized quaternion with private components. Identity
is the default. Construction includes:

- axis-angle from `Direction3` plus `Radians`;
- explicit Euler order;
- look rotation from forward and up directions;
- validated conversion from `Quat` or orthonormal `Mat3`;
- shortest-arc rotation between directions.

Closed operations:

- composition;
- inverse via conjugate;
- rotate `Vec3`, `Point3` about origin, `Direction3`, and `Normal3`;
- normalized interpolation (`Nlerp`, `Slerp`);
- conversion to matrix.

Equality meanings remain separate:

- exact component equality for representation tests;
- `IsNearlyEqual` for component tolerance;
- `RepresentsSameRotation` for the `q`/`-q` equivalence.

Composition order is documented with executable examples.

## Extents and nonnegative measures

Several geometry types require nonnegative radii/half-extents. A generic public
`NonNegative<T>` wrapper is likely too noisy. Instead:

- geometry factories validate/canonicalize scalar radii and extents;
- fields are private;
- accessors return plain scalars/vectors;
- `TryFrom...` rejects invalid external data;
- explicit `FromAbsolute...` factories canonicalize when that is desired.

If repeated implementation proves awkward, an internal `Extent3<T>` invariant
type can be introduced without making it part of ordinary call sites.

## Optional coordinate-space tags

Compile-time world/local/view tags can catch an additional class of errors:

```cpp
TaggedPoint3f<WorldSpace>
TaggedPoint3f<LocalSpace>
```

They also multiply template instantiations and complicate engine integration.
They are proposed as a later opt-in header built on untagged semantic types,
not as a template parameter on every initial v2 type.

## Expected cost

`Point3f`, `Direction3f`, and `Normal3f` should each be one `Vec3f`.
`Rotation3f` should be one `Quatf`. Copy, load, store, and passing convention
should be identical to the underlying value.

Construction/validation has a cost at untrusted boundaries. Repeated consumers
then become cheaper because the invariant is reused. Benchmarks must include
both one-shot construction and amortized game-loop use.

