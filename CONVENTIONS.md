# Math Conventions

## TL;DR
* Left-handed coordinate system: +X right, +Y up, +Z forward
* Matrices are row-major
* `Vector * Matrix` transforms the vector by the matrix
* Rotation helpers use radians
* Quaternions are counter-clockwise
* `vec3 * quat` rotates the vector by the quaternion

## Coordinate System

- The library uses a left-handed coordinate system.
- Positive X is right.
- Positive Y is up.
- Positive Z is forward.

## Matrix Conventions

- Matrices are row-major.
- `Vector * Matrix` applies the matrix transform to the vector.
- `transform_point(...)` uses homogeneous `w = 1`.
- `transform_vector(...)` uses homogeneous `w = 0`, so translation does not
  affect the result.

## Quaternion Conventions

- Quaternion rotation helpers use radians. Use `deg2rad(...)` when working from
  degree values.
- Positive rotation is counter-clockwise under the library's handedness rules.
- The supported vector rotation operator is `vec3 * quat`.
- Quaternion `operator==` compares components exactly. Use
  `approx_equal(...)` for approximate component equality and
  `same_rotation(...)` when `q` and `-q` should be considered equivalent.
- Quaternion `ln()` and `exp()` implement the general quaternion logarithm and
  exponential. The zero quaternion logarithm returns zero as a finite fallback.
- Normalizing or inverting a zero quaternion returns zero.
- `look_rotation(forward, up)` returns identity for a zero forward vector and
  chooses a fallback up axis when the supplied directions are collinear.

## Comparison Semantics

- Vector comparison operators are component-wise "all lanes must satisfy the
  relation" checks.
- Those comparison operators are not a total ordering and should not be treated
  as one.
- Approximate equality is provided separately through `approx_equal(...)`.
- NaNs are never approximately equal. Identically signed infinities compare
  equal.

## Numeric Domains

- Integral vectors support component arithmetic, dot/cross products, and
  squared-length operations.
- Integral `length()` and `distance()` results are promoted to `double`.
- Normalization and angle operations are available only for floating-point
  vectors.
- Normalizing a zero floating-point vector returns zero. Tiny non-zero vectors
  are normalized rather than discarded.
- The angle between a zero vector and any vector returns zero as a finite
  fallback.

## Scalar Helper Semantics

- `sign(x)` returns `-1` for negative values, `0` for zero, and `1` for
  positive values.
- `refract(incident, normal, ior)` treats `ior` as the material index of
  refraction relative to air and returns the zero vector on total internal
  reflection.

## Backend Notes

- `vec2` is currently scalar-only.
- Floating-point `vec3`, `vec4`, matrices, and quaternions use the RTM-backed
  implementation where applicable.
- Integral vector types use the scalar implementation.
- The scalar field aliases (`x`, `y`, `z`, `w`) and contiguous `data` views use
  anonymous unions supported by MSVC, GCC, and Clang. Builds that reject all
  compiler extensions with strict pedantic errors are not supported without a
  field-access compatibility break.
