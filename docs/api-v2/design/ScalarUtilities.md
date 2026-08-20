# Scalar utilities and constants

The current API keeps scalar math deliberately thin. It does not wrap the C++
standard library merely to change spelling:

- use `std::abs`, `std::min`, `std::max`, and `std::clamp`;
- use `std::lerp` for ordinary scalar interpolation;
- use `std::sqrt`, the trigonometric functions, exponentials, logarithms,
  rounding, and remainders from `<cmath>`; and
- use `Radians<T>` and `Degrees<T>` at APIs where angle units matter.

`Constants.hpp` exposes `Pi<T>`, `HalfPi<T>`, `QuarterPi<T>`, and
`Tau<T>` from the C++20 `std::numbers` constants. `Scalar.hpp` adds the
game/graphics-specific facilities that are not already clear standard-library
operations:

- `Saturate(value)`;
- fallible `TryInverseLerp(from, to, value)`;
- `TryInverseLerpClamped(from, to, value)`;
- `SmoothStep(amount)`; and
- `SmootherStep(amount)`.

Inverse lerp rejects equal endpoints and non-finite inputs instead of silently
manufacturing a value. Its scaled fallback preserves useful results when
finite endpoint subtraction would overflow. Smoothing accepts a normalized
amount and clamps it explicitly; callers use `std::lerp` or vector `Lerp`
when they also want endpoint interpolation.

The cubic Hermite and quintic Perlin polynomial sources are cited inline beside
their implementations. These helpers are header-only, allocation-free, and
backend-independent.
