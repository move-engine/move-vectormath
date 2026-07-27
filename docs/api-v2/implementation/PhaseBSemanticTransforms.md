# Phase B semantic-transform slice

## Scope

Phase B begins with one vertical slice that tests whether semantic game and
graphics types remain thin when they participate in real transform
operations:

- affine `Point3f` algebra;
- distinct unit `Direction3f` and `Normal3f` roles;
- rigid rotation/translation transforms;
- explicit affine point, direction, and normal behavior;
- composition, inverse, and rigid-to-affine conversion;
- focused-header compilation and representative generated code.

This is an implementation proof, not the complete matrix, transform, or
geometry API.

## Implemented surface

| Area | Types and facilities |
| --- | --- |
| Point semantics | `Point3f`, origin, component reads, named vector conversion |
| Point algebra | point ± displacement, point - point, compound displacement |
| Normal semantics | validated `Normal3f`, direction conversion, axis factories |
| Shared invariant logic | one scaling-safe normalization routine for directions and normals |
| Rotation | constant-cost inverse and normal rotation |
| Rigid transform | `RigidTransform3f`, point/vector/direction/normal transforms |
| Rigid algebra | named application-order `Compose`, inverse, identity |
| Affine semantics | point transform, fallible direction normalization, inverse-transpose normal transform |
| Conversion | lossless rigid-to-affine conversion |

Point addition, point scaling, and other vector-only operations are absent by
construction. General affine direction transformation is named
`TryTransformDirection`, so scale and shear are not silently discarded.
Affine normal transformation is separately named and uses inverse transpose;
it reports singular transforms.

## Numerical policy

Direction and normal construction share the Phase A scaling-safe normalization
algorithm. It accepts finite nonzero values across the useful float exponent
range without overflowing an intermediate squared length.

Affine normal transformation calculates cofactors and determinant in double
precision. It normalizes before narrowing to float and preserves orientation
through negative-determinant reflections. Exact singularity and non-finite
intermediates return `std::nullopt`; no hidden determinant tolerance has been
chosen.

Behavioral tests cover:

- forbidden point algebra;
- tiny, large, zero, and non-finite invariant inputs;
- rigid point/vector/direction/normal distinctions;
- noncommuting rigid composition and inverse;
- nonuniform scale, shear orthogonality, reflection, and singular affine
  normal transforms;
- rigid-to-affine equivalence.

## Layout and generated code

`Point3f` and `Normal3f` are each one `Vec3f`: 16 bytes, 16-byte aligned,
trivially copyable, and standard-layout. `RigidTransform3f` is 32 bytes,
16-byte aligned, trivially copyable, and standard-layout.

The local GCC 16 RTM code-generation fixture emits 37 instructions for both
the semantic `Point3f` rigid-transform loop and the equivalent raw-`Vec3f`
loop. Their generated loop bodies are identical apart from symbol addresses.
The invariant direction loop likewise operates directly on the underlying
vector representation.

## Focused compile cost

Warmed local GCC 16 measurements used C++20 and `-O2`:

| Fixture | Time | Peak memory | Preprocessed lines |
| --- | ---: | ---: | ---: |
| Phase B point | 0.60 s | 137.2 MiB | 86,498 |
| Phase B rigid transform | 0.64 s | 144.4 MiB | 88,670 |
| Phase B umbrella | 0.66 s | 146.5 MiB | 90,576 |

The point fixture adds about 102 preprocessed lines over the Phase A focused
`Vec3` fixture. The rigid-transform fixture adds about 329 lines over the
Phase A rotation fixture, and the Phase B umbrella adds about 429 lines over
the Phase A umbrella. The semantic layer therefore remains thin relative to
the backend include cost.

## Shader and benchmark evidence

CI now compiles the HLSL and GLSL fixtures to SPIR-V, validates both modules,
and reflects the composite member offsets, runtime-array strides, and HLSL
matrix stride. The WGSL fixture remains pending a stable Tint or Naga CLI in
CI.

The separate benchmark branch adds:

- sprite integration and UI affine-transform workloads for 8-byte and 16-byte
  `Vec2f` representations;
- semantic `Point3f` versus raw-`Vec3f` rigid transforms;
- prevalidated semantic directions versus raw normalized vectors;
- validate-once versus normalize-on-every-use direction paths.

The new benchmark executables build for SSE4.2, AVX, and AVX2. Timings remain
pending a controlled run; hosted reports are diagnostic only.

## Next gates

- Run the new `Vec2f` workloads under controlled frequency and thermal
  conditions before finalizing its representation.
- Add WGSL compilation and layout validation when a stable tool is available.
- Inspect Phase B code generation on MSVC and AppleClang/ARM.
- Add general `Mat3f`/`Mat4f`, then extend affine inverse and projective
  behavior without broadening ambiguous transform helpers.
- Do not begin geometry queries until the semantic-transform slice passes its
  full CI and benchmark build matrix.
