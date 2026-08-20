# Phase C generic math and graphics matrices

## Scope

This checkpoint completes four related migration slices:

- practical vector parity: approximate comparison, checked runtime indexing,
  compile-time component access, a deliberately small swizzle set, and
  status-bearing refraction;
- scalar generalization: float/double semantic transforms, geometry values,
  prepared rays, hit/closest results, and query kernels;
- discrete bounds: fixed-width signed and unsigned integer AABBs without
  nonsensical continuous center, half-extent, area, or volume contracts;
- general and graphics matrices: `Mat4<T>`, explicit projection conventions,
  and fallible look-at construction.

The vector swizzle surface remains `XY`, `XZ`, `YZ`, `ZW`, and `XYZ` rather
than generating every permutation. This covers common dimensional extraction
without imposing a combinatorial header parsing and completion cost.

## Projection and view contracts

`TryPerspectiveFov`, `TryPerspectiveFovInfinite`, and
`TryOrthographicOffCenter` require explicit `Handedness`, `ClipDepth`, and
`DepthDirection`. Finite and infinite far planes, left- and right-handed view
space, zero-to-one and negative-one-to-one NDC, and forward and reverse depth
are correctness-tested as independent policy combinations. Orthographic bounds
may be reversed to express an intentional axis flip.

`TryLookAt`, `TryLookAtLH`, and `TryLookAtRH` fail for coincident eye/target
positions, parallel up/view directions, and non-finite results. They do not
silently repair an invalid camera basis.

The projection formulas are derived from the requested near/far NDC endpoints
and cross-checked inline against DirectXMath and GLM. The view basis is the
standard Gram-Schmidt construction cross-checked against the same compatible-
licensed sources. Source references and deliberate Move policy remain beside
the algorithms in the public headers.

## Matrix migration benchmark

The separate benchmark repository now checks identical-input current/legacy
parity before measuring Mat4 transform, multiply, determinant, inverse,
perspective, orthographic, and look-at construction. Representative 4,096-item
results follow; time is nanoseconds per item.

| Capability | SSE4.2 mv / legacy | AVX mv / legacy | AVX2 mv / legacy |
| --- | ---: | ---: | ---: |
| Mat4 transform | 1.23 / 0.91 | 1.19 / 1.20 | 1.10 / 1.22 |
| Mat4 multiply | 3.52 / 3.61 | 3.65 / 3.63 | 3.01 / 3.02 |
| Mat4 determinant | 4.94 / 4.94 | 4.99 / 4.97 | 4.76 / 4.66 |
| Mat4 inverse | 15.60 / 11.81 | 16.96 / 11.84 | 17.21 / 11.35 |
| Perspective | 14.57 / 7.19 | 14.52 / 7.07 | 13.71 / 5.43 |
| Orthographic | 8.26 / 1.91 | 7.50 / 1.73 | 7.26 / 1.72 |
| Look-at | 24.20 / 15.07 | 20.91 / 15.10 | 20.90 / 15.01 |

The host used a powersave governor with frequency scaling, so these timings are
diagnostic rather than release gates. Hardware instruction counts and repeated
cross-ISA behavior make the causes clearer:

- Mat4 multiply and determinant are instruction-equivalent to legacy. AVX and
  AVX2 transform are also equivalent; SSE4.2 current transform eagerly loads
  four matrix rows through the private RTM adapter instead of folding them into
  arithmetic memory operands, adding four instructions.
- Checked inverse adds finite-result validation: five branches and roughly
  20--39 instructions depending on ISA. The legacy inverse is unchecked, so
  the rows are intentionally labeled as different contracts.
- Projection and view construction validate all input and output invariants.
  The legacy constructors assert only a subset in debug builds and are
  unchecked in this release benchmark. Projection is normally camera-boundary
  work, not an inner-loop operation; removing its safety contract to win this
  comparison is not justified.

## Inlining audit

Optimized object inspection found no facade calls in the representative vector,
semantic-transform, geometry-query, or Mat4 transform/multiply hot kernels.
Class-body definitions are implicitly inline, templates are visible to the
optimizer, and the existing private helpers are fully eliminated. Mat4
perspective retains the expected `tanf` call; other observed calls were stack-
protector failure paths.

Blanket force-inline annotations would therefore add compiler-specific API
machinery without changing the measured code. The accepted policy remains to
add a narrow private portability attribute only when a generated-code fixture
shows an actual missed inline and a runtime regression.

## Verification

- 21/21 root tests pass, including scalar and RTM configurations.
- The focused Clang build passes all 9 targets under AddressSanitizer and
  UndefinedBehaviorSanitizer (with leak detection disabled because LeakSanitizer
  cannot operate under the debugger/ptrace environment).
- Float/double projection, view, transforms, geometry, and queries pass.
- Integer AABB construction, containment, intersection, size, and closest-point
  contracts pass.
- Dimension-aware component division avoids evaluating unused SIMD/storage
  lanes; sanitizer coverage specifically exercises integer `Vec2` and `Vec3`
  division and compound division.
- SSE4.2, AVX, and AVX2 matrix benchmark binaries build and pass parity-only
  execution before timings are collected.
