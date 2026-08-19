# Algorithm provenance

## Scope and historical disclosure

This register covers both the legacy-API changes produced by the Codex audit
branch beginning at `ba96600` and the new `mv::math` API-v2 implementation
beginning at `036c68e`. It records algorithmic provenance; the comparative
engine/API research has its own source register in
[`../reviews/Sources.md`](../reviews/Sources.md).

The legacy audit changes, Phase A, Phase B, and the first Phase C implementation
were initially generated from the model's internalized mathematical knowledge
without consistently consulting a specific implementation source at coding
time. Some pre-existing legacy functions already named DXM or GLM in comments,
but those comments did not identify immutable revisions. The 2026-08-19 audit
below therefore labels retained Codex work as **source-validated independent
derivation** unless the generated change directly switched to a named upstream
primitive. This is an explicit historical limitation, not an attribution to
unspecified training data.

All subsequent nontrivial algorithms are source-first under
[`ProvenancePolicy.md`](../planning/ProvenancePolicy.md).

## Source catalog

| ID | Source | Kind and license |
| --- | --- | --- |
| `P-NORM-BLUE` | James L. Blue, “A Portable Fortran Program to Find the Euclidean Norm of a Vector,” ACM TOMS 4(1), 1978, [DOI 10.1145/355769.355771](https://doi.org/10.1145/355769.355771) | Original numerical-method paper |
| `P-NORM-ANDERSON` | Edward Anderson, “Algorithm 978: Safe Scaling in the Level 1 BLAS,” ACM TOMS 44(1), 2017, [DOI 10.1145/3061665](https://doi.org/10.1145/3061665) | Original numerical-method paper |
| `P-QUAT-SZELISKI` | Richard Szeliski, “Image Alignment and Stitching: A Tutorial,” Microsoft Research Technical Report MSR-TR-2004-92, 2005, [report PDF](https://robots.stanford.edu/cs223b05/MSR-TR-2004-92-Jan26.pdf), especially equations 22, 26, and 28 | Authoritative mathematical derivation |
| `P-QUAT-SHOEMAKE` | Ken Shoemake, “Animating Rotation with Quaternion Curves,” SIGGRAPH 1985, [DOI 10.1145/325165.325242](https://doi.org/10.1145/325165.325242) | Original graphics paper for quaternion interpolation/calculus |
| `P-RAY-TRIANGLE` | Tomas Möller and Ben Trumbore, “Fast, Minimum Storage Ray-Triangle Intersection,” JGT 2(1), 1997, [DOI 10.1080/10867651.1997.10487468](https://doi.org/10.1080/10867651.1997.10487468) | Original algorithm paper |
| `P-RAY-BOX` | Amy Williams, Steve Barrus, R. Keith Morley, and Peter Shirley, “An Efficient and Robust Ray-Box Intersection Algorithm,” JGT 10(1), 2005, [DOI 10.1080/10867651.2005.10487503](https://doi.org/10.1080/10867651.2005.10487503), [author-hosted PDF](https://perso.univ-lyon1.fr/jean-claude.iehl/Public/educ/M1IMAGE/williams_box.pdf) | Original robustness/optimization paper |
| `A-POINT-TRIANGLE-EBERLY` | David Eberly, [“Distance Between Point and Triangle in 3D”](https://www.geometrictools.com/Documentation/DistancePoint3Triangle3.pdf), created 1999, revised 2020 | Author algorithm paper; CC BY 4.0 |
| `B-RTCD-ERICSON` | Christer Ericson, *Real-Time Collision Detection*, 2005, sections 5.1.1, 5.1.2, and 5.1.5; [author site](https://realtimecollisiondetection.net/) | Authoritative game-oriented textbook |
| `A-NORMAL-LENGYEL` | Eric Lengyel, [“Transforming Normals”](https://terathon.com/blog/transforming-normals.html), 2024 | Author derivation of adjugate-transpose normal transformation |
| `L-RTM-231` | RTM v2.3.1 at [`745bd25673d93b46941eda55e0993327dbc12b53`](https://github.com/nfrechette/rtm/tree/745bd25673d93b46941eda55e0993327dbc12b53), particularly `vector4f.h`, `quatf.h`, and `qvf.h` | MIT; exact production dependency |
| `L-GTE-2026` | David Eberly's Geometric Tools at [`d29e7758ae2615e5e37da3eb573b7bf90ee94e9b`](https://github.com/davideberly/GeometricTools/tree/d29e7758ae2615e5e37da3eb573b7bf90ee94e9b), particularly `GTE/Mathematics/DistPointLine.h`, `DistPointRay.h`, `DistPointSegment.h`, and `DistPointTriangle.h` | Boost Software License 1.0; author-maintained implementation cross-check |
| `L-DXM-2026` | DirectXMath at [`d33ba2f150aeb6d3cf62d10f454652ee83672200`](https://github.com/microsoft/DirectXMath/tree/d33ba2f150aeb6d3cf62d10f454652ee83672200), particularly [`Inc/DirectXCollision.inl`](https://github.com/microsoft/DirectXMath/blob/d33ba2f150aeb6d3cf62d10f454652ee83672200/Inc/DirectXCollision.inl) and [`Inc/DirectXMathMisc.inl`](https://github.com/microsoft/DirectXMath/blob/d33ba2f150aeb6d3cf62d10f454652ee83672200/Inc/DirectXMathMisc.inl) | MIT; production-library cross-check |
| `L-GLM-2026` | GLM at [`6f14f4792a0cde5d0cf2c910506724d61cb95834`](https://github.com/g-truc/glm/tree/6f14f4792a0cde5d0cf2c910506724d61cb95834), particularly [`glm/gtx/intersect.inl`](https://github.com/g-truc/glm/blob/6f14f4792a0cde5d0cf2c910506724d61cb95834/glm/gtx/intersect.inl) and [`glm/ext/quaternion_exponential.inl`](https://github.com/g-truc/glm/blob/6f14f4792a0cde5d0cf2c910506724d61cb95834/glm/ext/quaternion_exponential.inl) | Dual Modified-MIT/MIT; production-library cross-check |
| `L-BOOST-189` | Boost.Test 1.89.0 [floating-point comparison rationale](https://www.boost.org/doc/libs/1_89_0/libs/test/doc/html/boost_test/testing_tools/extended_comparison/floating_point.html) | Boost Software License 1.0; conceptual cross-check only |

No source code from the papers or comparison libraries is copied into the
API-v2 implementation. The listed libraries were read to compare equations,
branch conditions, conventions, and edge handling.

## Register

### Legacy numerical audit

- **Coverage:** Codex changes in `ba96600` to `common.hpp`, scalar vector
  length/distance/normalization, angle clamping, mixed numeric comparisons, and
  the associated tests.
- **Classification:** elementary definitions plus source-validated
  Move-specific numerical hardening.
- **Sources:** `P-NORM-BLUE` and `P-NORM-ANDERSON` for scaling/normalization
  concerns; `L-BOOST-189` and Bruce Dawson's
  [comparison survey](https://randomascii.wordpress.com/2012/02/25/comparing-floating-point-numbers-2012-edition/)
  for finite, absolute, and relative comparison cases.
- **Differences:** the legacy `approx_equal` contract combines one caller
  tolerance as both an absolute and a scale-relative bound. Its overflow-safe
  mixed-signed integral comparison is Move-specific. These are retained bug
  fixes, not copied source implementations, and API v2 does not automatically
  inherit that global approximate-equality policy.

### Legacy quaternion audit

- **Coverage:** Codex changes in `ba96600` to `quat::look_rotation`,
  `same_rotation`, quaternion inverse, logarithm, and exponential.
- **Classification:** source-validated independent derivation.
- **Sources:** `P-QUAT-SHOEMAKE`, `P-QUAT-SZELISKI`, `L-RTM-231` matrix/quaternion
  construction and look-rotation conventions, `L-DXM-2026`
  `XMQuaternionInverse`, and `L-GLM-2026` quaternion exponential/logarithm.
- **Differences:** the legacy look-rotation adds zero-forward and collinear-up
  fallback policy; `same_rotation` recognizes the quaternion double cover;
  logarithm chooses positive X for a negative real quaternion and returns zero
  for the undefined logarithm of the zero quaternion. Those domain policies are
  Move decisions and must not be inferred from the comparison sources.

### Legacy RTM normalization

- **Coverage:** `0c939fe` changes to SIMD `base_vec3` and `base_vec4`
  normalization.
- **Classification:** direct compatible-library primitive adaptation.
- **Source:** `L-RTM-231` `scalar_sqrt_reciprocal` and vector multiply.
- **Differences:** Move preserves its legacy zero-vector-to-zero policy before
  invoking RTM's reciprocal square root.

### Legacy audit glue

- **Coverage:** all remaining Codex changes in `ba96600` that repair return
  types, storage loading, traits, constraints, wrapper forwarding, equality,
  serialization, build integration, CI, tests, and documentation.
- **Classification:** Move-specific API/build/test glue or elementary
  definitions; no external algorithm was adapted.
- **Evidence:** the exact commit diff and `AUDIT_REPORT.md`. Existing legacy
  routines that predate `ba96600` are outside the Codex-generated audit scope,
  even when that commit reformatted them.

### Phase A vector operations

- **Coverage:** `detail/VectorOps.hpp`, `Vec2.hpp`, `Vec3.hpp`, and `Vec4.hpp`.
- **Classification:** elementary definitions plus thin backend adaptation.
- **Sources:** `L-RTM-231`; standard componentwise vector, dot-product, and
  cross-product definitions.
- **Differences:** scalar code is Move-owned; the RTM path calls the pinned RTM
  primitives. Public storage and zeroed spare-lane policy are Move API/ABI
  decisions, not taken from RTM.

### Phase A safe normalization

- **Coverage:** `detail/Normalization.hpp`, semantic direction/normal
  construction, and quaternion normalization in `Rotation3.hpp`.
- **Classification:** source-validated independent derivation.
- **Sources:** `P-NORM-BLUE`, `P-NORM-ANDERSON`, and the LAPACK scaled
  sum-of-squares lineage.
- **Differences:** fixed 3- and 4-component inputs are divided by their largest
  absolute component, then normalized. This is a simpler bounded-dimension
  specialization, not a transcription of BLAS/LAPACK accumulation code.
- **Validation:** extreme finite magnitudes, subnormal-scale inputs, zero, NaN,
  and infinity are exercised in Phase A/B tests.

### Phase A rotation

- **Coverage:** `Rotation3f` axis-angle construction, Hamilton product,
  conjugate inverse, and vector rotation.
- **Classification:** source-validated independent derivation.
- **Sources:** `P-QUAT-SZELISKI` equations 22, 26, and 28; convention and
  generated-result cross-checks against `L-RTM-231` `quatf.h` and `qvf.h`.
- **Differences:** Move validates/renormalizes construction, has a named
  semantic rotation type, and fixes composition meaning through tests rather
  than inheriting RTM's public types.

### Phase B rigid and affine transforms

- **Coverage:** affine point/vector transforms and rigid transform composition,
  inverse, and application.
- **Classification:** source-validated independent derivation and Move-specific
  semantic facade.
- **Sources:** `L-RTM-231`, especially `qvf.h` composition, point transform,
  and inverse operations.
- **Differences:** Move names `Compose(first, second)` by application order and
  separates points, vectors, directions, and normals at the type level.

### Phase B normal transformation

- **Coverage:** `TryTransformNormal` in
  `transforms/AffineTransform3Semantics.hpp`.
- **Classification:** source-validated independent derivation.
- **Sources:** `A-NORMAL-LENGYEL` for the column-cross-product/cofactor
  derivation; standard inverse-transpose identity.
- **Differences:** the accepted Phase B contract currently requests normalized
  inverse-transpose behavior. The code multiplies the cofactor result by the
  determinant sign before normalization, so a negative-X reflection maps an
  X normal to negative X. An oriented surface normal treated as an antivector
  instead uses the adjugate transpose and would retain positive X in that
  example. This semantic distinction must be resolved before the Move 1.x API
  is frozen; the two formulations must not be documented as interchangeable.

### Phase C primitives

- **Coverage:** `Ray3f`, `Plane3f`, `Triangle3f`, `Sphere3f`, and `Aabb3f`
  construction and elementary measurements/containment.
- **Classification:** elementary definitions and Move-specific invariant/API
  policy, source-validated against `L-DXM-2026` and `L-GLM-2026` where those
  libraries expose the same operations.
- **Differences:** finite fallible factories, canonical empty AABBs, semantic
  point/direction/normal members, double intermediates for wide float spans,
  and explicit optional empty-derived values are Move decisions.

### Phase C ray/plane intersection

- **Coverage:** ray/plane predicate and detailed hit.
- **Classification:** source-validated independent derivation.
- **Sources:** `L-GLM-2026` `intersectRayPlane` plus the plane equation.
- **Differences:** Move accepts distance zero, exposes caller-selected parallel
  tolerance, rejects non-finite inputs/results, and returns point, normal, and
  face orientation.

### Phase C ray/triangle intersection

- **Coverage:** ray/triangle predicate and detailed hit.
- **Classification:** source-validated independent derivation.
- **Sources:** `P-RAY-TRIANGLE`; implementation and convention cross-checks
  against `L-DXM-2026` `TriangleTests::Intersects` and `L-GLM-2026`
  `intersectRayTriangle`.
- **Differences:** explicit back-face mode, configurable determinant tolerance,
  inclusive distance-zero/barycentric boundaries, finite rejection, full XYZ
  barycentric weights, semantic normal, and face orientation.

### Phase C ray/AABB intersection

- **Coverage:** `PreparedRay3f` and ray/AABB predicate and detailed interval.
- **Classification:** source-validated independent derivation.
- **Sources:** `P-RAY-BOX`; scalar/SIMD behavior cross-check against
  `L-DXM-2026` `BoundingBox::Intersects`.
- **Differences:** Move explicitly branches parallel axes to avoid
  zero-times-infinity at slab boundaries, treats reciprocal overflow as
  parallel for representable-distance queries, uses closed/touching intervals,
  clips entry to the ray domain, and returns entry/exit normals and strict
  `StartsInside`. Williams et al. rely more directly on IEEE infinities and
  precomputed reciprocal signs.
- **Validation caveat:** `tests/phase_c.cpp::ReferenceRayAabb` is a separately
  written double-precision division implementation, but it remains the same
  slab algorithm and is not an algorithmically independent oracle.

### Phase C ray/sphere intersection

- **Coverage:** ray/sphere predicate and detailed interval.
- **Classification:** source-validated independent derivation.
- **Sources:** `L-DXM-2026` `BoundingSphere::Intersects` and `L-GLM-2026`
  `intersectRaySphere`.
- **Differences:** Move evaluates the normalized-direction quadratic in double,
  returns both clipped entry and exit distances, distinguishes strict interior
  starts from boundary starts, and makes a zero-radius center normal absent.

### Phase C bounds-pair predicates

- **Coverage:** sphere/sphere, sphere/AABB, and AABB/AABB predicates.
- **Classification:** elementary definitions, source-validated independent
  derivation.
- **Sources:** `L-DXM-2026` `BoundingSphere::Intersects` and
  `BoundingBox::Intersects`/`Contains` implementations.
- **Differences:** Move's float API uses double intermediates for squared
  distances and defines touching as intersection.

### Phase C line and segment primitives

- **Coverage:** `geometry/Line3.hpp` and `geometry/Segment3.hpp`.
- **Classification:** elementary definitions and Move-specific semantic facade,
  source-validated against a compatible implementation.
- **Sources:** `L-GTE-2026` `Line.h`, `Segment.h`, and the point-distance query
  files; `B-RTCD-ERICSON` section 5.1.2.
- **Differences:** Move stores a normalized `Direction3f` for lines, making the
  signed parameter physical distance. Segments store endpoints, expose an
  explicitly named fraction parameter, allow degeneracy, and do not silently
  clamp `PointAtFraction`.

### Phase C point/linear closest queries

- **Coverage:** point/line, point/ray, point/segment, and point/plane detailed,
  closest-point, distance, and squared-distance overloads in
  `queries/ClosestPointQueries.hpp`.
- **Classification:** paper/book-derived implementation, cross-checked against
  a compatible author-maintained library.
- **Sources:** `L-GTE-2026` `DistPointLine.h`, `DistPointRay.h`, and
  `DistPointSegment.h`; `B-RTCD-ERICSON` sections 5.1.1 and 5.1.2.
- **Differences:** unit line/ray directions remove the upstream direction-length
  division; returned parameters preserve signed line distance, nonnegative ray
  distance, and segment fraction as distinct fields. A degenerate segment
  deterministically returns its start with fraction zero.

### Phase C point/triangle closest query

- **Coverage:** point/triangle detailed closest-point, barycentric, distance,
  and squared-distance overloads in `queries/ClosestPointQueries.hpp`.
- **Classification:** book-derived independent expression, cross-checked
  against the original author paper and compatible reference implementation.
- **Sources:** `B-RTCD-ERICSON` section 5.1.5 for the Voronoi-region tests,
  `A-POINT-TRIANGLE-EBERLY` for the constrained quadratic formulation, and
  `L-GTE-2026` `DistPointTriangle.h` for result conventions.
- **Differences:** unlike the cited nondegenerate formulations, Move explicitly
  handles degenerate triangles by evaluating all three segment edges. Ties are
  deterministic in edge order 01, 02, 12, and barycentrics describe the chosen
  representative point.
- **Validation:** fixed face, vertex, edge, collinear, and point-degenerate
  cases run on scalar and RTM backends. A 1,024-case double-precision oracle
  independently uses plane projection followed by exhaustive edge tests rather
  than the production Voronoi-region decision tree.

## Non-algorithmic generated code

The packed/GPU transfer structs, layout traits, focused umbrella headers,
`StridedSpan`, and batch loops are Move-specific API, ABI, and integration
code. Their provenance is the API-v2 design record and the shader layout
fixtures, not an external algorithm. Generated tests and benchmarks are also
Move-owned; where they duplicate the production algorithm, the relevant entry
states that they are not independent evidence.

## Audit finding to resolve

Before Phase B/C is presented as the Move 1.x surface, decide whether
`Normal3f` represents an ordinary inverse-transpose normal direction or an
oriented surface normal/antivector under reflections. The current code and
tests implement the former. `A-NORMAL-LENGYEL` demonstrates why the latter
uses an adjugate transpose and differs for negative determinants.
