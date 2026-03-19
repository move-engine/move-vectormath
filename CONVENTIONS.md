# Math Conventions

## TL;DR
* Left-handed coordinate system, Y-up
* Matrices are row-major
* `Vector * Matrix` transforms the vector by the matrix
* Rotation helpers use radians
* Quaternions are counter-clockwise
* `vec3 * quat` rotates the vector by the quaternion

## Coordinate System

- The library uses a left-handed coordinate system.
- Positive Y is up.

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

## Comparison Semantics

- Vector comparison operators are component-wise "all lanes must satisfy the
  relation" checks.
- Those comparison operators are not a total ordering and should not be treated
  as one.
- Approximate equality is provided separately through `approx_equal(...)`.

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
