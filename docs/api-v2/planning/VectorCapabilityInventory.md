# Vector capability cutover inventory

The first API-v2 vector types proved representation and backend selection, but
they were not a complete replacement for the legacy vector API. Core vector
parity is now a prerequisite for further high-level API expansion.

## Scalar families

`Vec2<T>`, `Vec3<T>`, and `Vec4<T>` support arithmetic component types. Public
aliases use fixed-width names so their representation is portable:

- floating point: `VecNf`, `VecNd`;
- default game integers: `VecNi` and `VecNu` (32-bit);
- explicit integers: `VecNi8/u8`, `VecNi16/u16`, `VecNi32/u32`, and
  `VecNi64/u64`.

Matrices, normalized directions, normals, rotations, planes, rays, and most
continuous intersection queries require floating-point scalars. They should
offer `f` and `d` aliases, not misleading integer aliases. Points, grid bounds,
and discrete extents may support integers where their complete contract does.

## Capability matrix

| Capability | Scalar domain | Status / decision |
| --- | --- | --- |
| Component access and mutation | arithmetic | Ready |
| Vector and scalar `+`, `-`, `*`, `/` | arithmetic | Implemented; ordinary C++ overflow/divide rules apply to public components, while unused native lanes are never evaluated by dimensional division |
| Unary negation and compound assignment | arithmetic | Implemented; unsigned negation wraps by C++ rules |
| Dot and squared length | arithmetic | Implemented consistently for 2D/3D/4D |
| Cross product | arithmetic 3D | Implemented |
| Exact equality | arithmetic | Implemented |
| Ordering | arithmetic | Use explicitly named component predicates; do not restore ambiguous relational operators |
| Indexing | arithmetic | Read-only `operator[]` returns by value with a debug-checked precondition; `SetComponent` reports invalid runtime indices; `Get<I>`/`Set<I>` provide branch-free compile-time access |
| Length and distance | floating point | Implemented; squared forms also support arithmetic scalars |
| Fallible normalization | floating point | Implemented for 3D through `Direction3<T>::TryFrom`; zero/non-finite values fail explicitly |
| Interpolation | floating point | Implemented with distinct `Lerp` and `LerpUnclamped` names |
| Min/max/clamp/abs/sign | appropriate arithmetic subsets | Implemented; abs/sign are constrained to signed and floating-point scalars |
| Reflect/project | floating point | Implemented through `Normal3<T>` and `Direction3<T>` so normalized-input preconditions are encoded |
| Refract | floating point | Implemented for `Direction3<T>`/`Normal3<T>` with explicit success, total-internal-reflection, invalid-ratio, and numerical-failure states |
| Approximate equality | floating point | Implemented with explicit combined absolute/relative `Tolerance<T>` policy; exact equality remains separate |
| Constants/axes | arithmetic where representable | Implemented as PascalCase factories |

The migration benchmark audit also found that legacy RTM-backed `Vec3` and
`Vec4` scalar addition is declared but fails to compile when instantiated: it
passes a scalar to RTM's vector/vector `vector_add`. New scalar arithmetic is
therefore correctness-tested, while direct old/new performance rows use the
equivalent component-vector operation.

## Intentional differences from legacy

- No public backend/acceleration type parameter or `fast_` versus `storage_`
  alias split. Representation-specific packed and GPU types remain explicit.
- Compact `Vec2f` deliberately uses scalar per-value primitives so compilers
  can vectorize across adjacent 8-byte values without SIMD expand/extract work.
- No ambiguous componentwise `<`, `>`, `<=`, or `>=` returning one Boolean.
- No unchecked `Normalized()` that silently maps zero to zero; normalization
  will expose failure or require an explicit fallback.
- No raw mutable indexing reference when it would expose or constrain the
  selected native representation.
- Swizzles are limited to common dimensional projections (`XY`, `XZ`, `YZ`,
  `ZW`, and `XYZ`). Move does not generate every permutation: that surface
  increases parse/completion cost, and explicit construction is clearer for
  uncommon reorderings.
