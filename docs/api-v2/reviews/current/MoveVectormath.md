# Move Vectormath v1 API review

## Executive assessment

The v1 API has a useful low-level core and its RTM-backed wrappers can compile
away completely in representative hot loops. Its principal problem is not that
the backend abstraction necessarily costs runtime performance. The problem is
that backend selection, storage policy, numeric type, and public identity are
entangled in large public templates. That makes the API harder to explain,
increases the amount of implementation parsed by consumers, permits surprising
implicit conversions, and still leaves large game/graphics feature gaps.

The v2 API should retain the existing implementation strategy that works:
write algorithms once over a small backend vocabulary and specialize the
primitive operations. It should not retain backend inheritance or incidental
RTM wrapper types in public signatures.

## Reviewed baseline

- Repository commit:
  `d68818dd806bb5cbb5d23dbd5c4d0ef0a8570597`
- Public namespace: `move::math`
- Public headers: `packages/move/math/include/move/math/`
- Public-header size: 8,334 lines, excluding the disabled scalar matrix file
- C++ requirement: C++20
- Optimized backend: RTM 2.3.1

## Current surface

The public surface contains:

- scalar helpers for comparison, interpolation, trigonometry, logarithms,
  angle conversion, and related operations;
- `vec2`, `vec3`, and `vec4` templates;
- `quat`;
- `mat3x3`, a skeletal `mat3x4`, and `mat4x4`;
- packed-looking `storage_*` aliases and matrix storage types;
- explicit acceleration selection through `Acceleration::{Default,Scalar,RTM}`;
- traits for component type/count and vector/matrix recognition;
- direct RTM conversion hooks;
- optional serialization and formatting integration.

There are no first-class points, normalized directions, normals, angle types,
transform objects, bounds, geometric primitives, collision queries, frusta, or
batch/structure-of-arrays types.

## What works well

### Specialized primitive implementations can disappear

The benchmark audit found identical retired-instruction counts for Move and raw
RTM in ray/AABB and ray/triangle kernels. This is strong evidence that an
inline wrapper over an optimized backend can be zero-overhead when its contract
matches the backend operation. V2 should preserve this property.

### Compute and storage representations are already recognized as distinct

The library provides fast and storage aliases and separate storage matrices.
This is the correct performance concern: a three-component float value may
need either 12-byte packed storage or 16-byte SIMD-friendly compute storage.
The current naming and conversions need redesign, but the distinction itself
should remain.

### The audit established useful numerical contracts

The v1 audit made zero normalization finite, retained nonzero subnormal/tiny
vectors, promoted integral geometry results, separated exact quaternion
equality from approximate equality and rotational equivalence, and repaired
general quaternion logarithm/exponential behavior. Those explicit contracts
are valuable inputs to v2.

### Point and vector transforms are distinguished

`mat4x4::transform_point` and `transform_vector` correctly select homogeneous
W values of one and zero. V2 can make that distinction structural through
semantic types rather than relying only on method selection.

### Safe defaults are convenient

Vectors default to zero and matrices/quaternions default to identity-like
values. That is friendly for ordinary gameplay code. A measured, explicit
uninitialized construction path can coexist with safe defaults for hot
allocation paths.

## Public architecture issues

### Backend inheritance leaks into the type model

`vec3<T, RequestedAccel, Accel>` publicly inherits either a scalar or RTM base
implementation. The resolved backend is a third template parameter with a
default derived from the requested backend and component type. Consequences
include:

- implementation mechanics are visible in diagnostics and overload sets;
- the same conceptual value has several concrete public identities;
- common code must account for scalar, requested, and resolved acceleration;
- representation and capabilities are inherited rather than stated by the
  public contract;
- both scalar and RTM base implementations are included by the public vector
  header.

A public value should instead contain one implementation-selected
representation, with backend policy hidden in `detail` or selected by build
configuration/type aliases.

### Matrices and quaternions expose incidental wrapper parameters

`mat4x4<T, wrapper_type>` and `quat<T, wrapper_type>` allow an internal RTM
wrapper type to participate in public template identity. This creates
flexibility that consumers should not need and makes ABI/API reasoning harder.
Backend extension should happen through a deliberately small customization
contract, not an arbitrary public wrapper parameter.

### Acceleration is both requested and silently resolved

Requesting RTM does not guarantee RTM; integral vectors fall back to scalar.
The actual choice is observable through `::acceleration`. Silent fallback is
convenient, but it weakens generic assumptions and makes names such as
`fast_float3` conditional on platform/configuration.

V2 should distinguish portable semantic identity from explicit backend
experimentation. Most users should select `Vec3f`; backend specialists can use
internal/configuration mechanisms or clearly named policy types.

### The abstraction is asymmetric

`vec2` is scalar-only, `vec3`/`vec4` can use scalar or RTM, and matrices and
quaternions are RTM-only. This means `Acceleration` suggests a uniform policy
that the library does not provide. It also makes a fully scalar build or a
different SIMD backend more difficult than the generic surface implies.

### Direct RTM conversions weaken isolation

`to_rtm()` and `from_rtm()` are useful escape hatches but couple ordinary
consumers to RTM types and headers. Integration should live in a specific
adapter header so core types do not need to expose a third-party backend in
their normal interface.

## Naming and usability issues

### Aliases obscure representation

Representative aliases have materially different layouts:

| Name | Size | Alignment | Meaning |
|---|---:|---:|---|
| `float3` | 16 | 16 | RTM-backed compute vector |
| `vec3f` | 12 | 4 | scalar/storage vector |
| `float4` | 16 | 16 | RTM-backed compute vector |
| `float4x4` | 64 | 16 | RTM-backed compute matrix |
| `storage_float4x4` | 64 | 4 | storage matrix |

`float3` and `vec3f` look like stylistic synonyms but have different storage
and acceleration behavior. V2 names must make storage intent obvious and keep
the default spelling stable.

### Snake case conflicts with the project direction

Factories and operations use names such as `look_rotation`,
`project_onto_plane`, and `lerp_unclamped`. The approved v2 convention is
PascalCase under `mv::math`.

### Direction constants lack a declared coordinate convention

`Forward`, `Backward`, `Up`, and related constants are valuable, but they are
dangerous without a prominent statement of handedness, forward axis, matrix
vector convention, storage order, angle units, clip-space depth, and projection
behavior. These conventions must be centralized and testable.

### Angle units and Euler order are too implicit

Quaternion and matrix factories take raw scalar angles. Euler construction does
not make rotation order discoverable in its name or type. Game APIs repeatedly
show that Euler order and degrees-versus-radians are common sources of defects.
V2 should use `Radians`/`Degrees` types or explicit factory names, and an
explicit Euler-order enumeration where multiple orders are supported.

### Mixed member/static/free-function style reduces discoverability

Vector algorithms are often static members, scalar algorithms are free
functions, and transformations are members of matrices. A consistent rule
would improve autocomplete and generic use:

- inexpensive value observations and mutations as members;
- symmetric algorithms and cross-type queries as free functions;
- named construction as static factories;
- backend adapters in dedicated integration namespaces/headers.

## Conversion and type-safety issues

### Numeric narrowing is implicit

The converting vector constructors accept other component types without
`explicit`, so double-to-float conversion can happen silently. Backend
conversion and vector-length conversion can also happen through constructors.
V2 should require explicit narrowing and explicit storage/compute conversion.

### Compute types are not trivially copyable

Representative `float3`, `vec3f`, `quatf`, `float4x4`, and storage matrix types
are not trivially copyable because user-provided copy operations suppress
triviality. This needlessly constrains binary transport and low-level
containers. V2 value types should be trivially copyable whenever their
representation permits it.

### Three-vector RTM conversion has ambiguous homogeneous semantics

`vec3::to_rtm()` supplies a W lane of one, while `from_rtm()` restores a
three-component value whose hidden W behaves as zero. A generic vector has no
justification for choosing point semantics during conversion. A plain compute
vector should use a documented don't-care/zero lane, while `Point3` and
`Direction3` should choose homogeneous semantics explicitly at matrix
boundaries.

### Normalization is a transient property

`normalized()` returns the same vector type, so functions requiring a unit
vector cannot express that precondition. They either normalize repeatedly,
trust the caller, or accept invalid input. A `Direction3`/`UnitVec3` type can
validate once and make repeated dot, reflection, basis, ray, and plane
operations thinner.

The invariant must still account for floating-point drift. Operations known to
preserve unit length within their numerical contract may return a direction;
arbitrary arithmetic should return a vector or require reconstruction.

## Performance and compilation issues

### Header weight is high for the available functionality

A representative vector/quaternion/matrix translation unit preprocesses about
69,941 lines and uses about 163 MiB peak compiler memory with GCC. Warm GCC
compile time was about 0.79 seconds, compared with RTM 0.71, DirectXMath 0.37,
GLM 0.35, and Sony Vectormath 0.29 seconds in the same local measurement.

The direct cause is structural:

- public vector headers include scalar and RTM implementations;
- templates contain large forwarding surfaces;
- matrix/quaternion headers include RTM implementation headers;
- serialization/streaming hooks live alongside compute operations;
- generic overloads and converting constructors expand candidate sets.

V2 should split scalar/common primitives, compute vectors, matrices,
transforms, geometry, queries, formatting, serialization, and backend adapters.
An umbrella header can remain, but consumers should not pay for it by default.

### Wrapper overhead is operation-dependent

Move matched raw RTM instruction counts in the intersection kernels, but
normalization, camera-basis construction, and quaternion rotation were slower
than Sony Vectormath and sometimes raw RTM. The audit attributes part of this
to stronger safety contracts and part to expression/code-shape differences.
The lesson is not to remove the facade; it is to make expensive guarantees
visible and provide invariant-rich inputs that avoid repeating work.

Examples:

- `Direction3` avoids renormalizing a known unit direction;
- `Ray3` can cache or expose reciprocal direction/sign data for repeated AABB
  tests through an explicit prepared-query type;
- `RigidTransform3` can avoid general 4x4 matrix operations;
- unchecked query variants can exist only where their preconditions are
  structural or conspicuously named.

### Some current work is redundant

`mat4x4::look_at` normalizes the up vector before the RTM extension constructs
and normalizes the cross-product basis. Normalizing `up` is unnecessary for
that basis construction except for degeneracy handling. High-level algorithms
should be expressed in terms of precisely documented backend primitives and
examined for duplicate normalization/conversion.

## Game and graphics capability gaps

The most important missing facilities are:

- distinct points, displacement vectors, unit directions, and surface normals;
- radians/degrees and explicit Euler order;
- rigid and affine transforms separate from general matrices;
- AABB, OBB, sphere, capsule, plane, triangle, ray, line, and segment types;
- typed hit results containing distance/parameter, point, normal, barycentrics,
  and front/back-face classification where applicable;
- intersection, containment, closest-point, and distance queries;
- frustum extraction/construction, corners, plane masks, and
  inside/intersecting/outside classification;
- prepared queries for repeated ray/bounds and frustum/bounds testing;
- batch/SoA facilities for high-volume culling or skinning;
- projection/unprojection and viewport helpers;
- documented coordinate and clip-space conventions.

These should be modular. A user who only needs vectors and quaternions should
not parse collision and frustum code.

## Recommended v2 direction

1. Make `mv::math` PascalCase value types the stable public identity.
2. Hide backend representation and primitive selection behind a small internal
   policy vocabulary.
3. Preserve one shared high-level algorithm implementation wherever code
   generation proves equivalent.
4. Separate packed storage from compute types with explicit conversions.
5. Add semantic `Point`, `Direction`, `Normal`, angle, and transform types.
6. Add geometry/query modules with typed results and prepared-query variants.
7. Make coordinate conventions and failure behavior explicit.
8. Keep default value types trivially copyable and standard-layout where
   feasible.
9. Split headers so capability cost is opt-in.
10. Treat compile time, code generation, layout, and runtime counters as API
    acceptance criteria, not only post-implementation benchmarks.

## Avoid carrying forward

- public inheritance from backend implementations;
- public template parameters naming RTM wrappers;
- implicit narrowing or implicit storage/compute conversion;
- aliases whose spelling conceals a different layout;
- a generic `Vec3` conversion that silently chooses point semantics;
- monolithic headers containing optional formatting/serialization;
- raw-scalar angles with unstated units or Euler order;
- geometry queries returning ambiguous tuples, sentinel vectors, or bare
  booleans when useful hit data has already been computed.

