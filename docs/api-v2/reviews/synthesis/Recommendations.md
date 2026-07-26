# API v2 recommendations

## Required changes to the current core

1. Move the public surface to `mv::math` with PascalCase identifiers.
2. Replace backend-inheriting public vectors with stable value types whose
   backend is selected internally.
3. Remove public RTM wrapper template parameters and ordinary `to_rtm` methods.
4. Make numeric narrowing and storage/compute conversions explicit.
5. Make value types trivially copyable and standard-layout where feasible.
6. Give generic three-vectors neutral homogeneous semantics; transform points
   and directions through distinct operations/types.
7. Add explicit angle units and Euler order.
8. Split headers by capability and isolate formatting/serialization/backend
   adapters.
9. Provide explicit safe construction and measured preconditioned paths.
10. Publish coordinate, matrix, quaternion, and projection conventions as part
    of the API contract.
11. Treat familiar engine vocabulary as a discoverability input, not a
    compatibility contract.

## Core functionality to add

- vector move-toward, magnitude clamp/limit, signed angle, projection,
  orthogonal basis, component extrema, finite checks, and smooth/cubic
  interpolation;
- `Radians` and `Degrees`;
- invariant `Direction2/3`, `Normal3`, and `Rotation2/3`;
- `Point2/3` affine semantics;
- `RigidTransform2/3` and `AffineTransform2/3`;
- explicit packed vector/matrix/quaternion storage types;
- array/span transforms;
- optional no-init construction with a named tag.

## Geometry functionality to add

- `Line`, `Ray`, and `Segment` as distinct types;
- `Plane`, `Triangle`, `Sphere`, `Capsule`, `Aabb`, and `Obb`;
- optional combined box/sphere render bounds;
- containment and plane-side classification enums;
- overlap predicates;
- detailed ray/line/segment hits for plane, triangle, sphere, AABB, and OBB;
- primitive/primitive intersection coverage modeled initially on
  DirectXCollision;
- closest-point and squared-distance queries;
- barycentric coordinates and triangle feature classification;
- prepared ray/bounds and frustum/bounds queries.

## Frustum and graphics functionality to add

- perspective/orthographic frustum construction;
- extraction from view-projection matrices with explicit clip-space policy;
- normalized planes and corners;
- point/sphere/AABB/OBB containment classification;
- plane masks for hierarchical culling;
- reverse-Z and infinite-far handling;
- world/screen projection and unprojection in an optional graphics header;
- packed octahedral direction/normal formats as an optional storage facility.

## Deliberately separate

- engine physics traces, collision layers, entity/material filters;
- serialization frameworks;
- text formatting;
- editor/reflection/network replication;
- graphics-API-specific projection adapters;
- RTM/DirectXMath/native-backend interop;
- SoA/batch containers beyond generic span algorithms.

These can be first-party adapters without burdening foundational types.

## Approachability and migration

Move should be immediately searchable by Unity users without reproducing
Unity's surprising contracts. The initial documentation set should include a
task-based Unity on-ramp covering:

- `Vector3` arithmetic versus `Vec3`, `Point3`, `Direction3`, and `Normal3`;
- `Quaternion` versus invariant `Rotation3` and algebraic `Quat`;
- scene `Transform` versus rigid, TRS-authoring, and affine values;
- point, vector, direction, and normal transformation;
- exact equality versus `IsNearlyEqual`;
- unclamped `Lerp` versus `LerpClamped`;
- `TryNormalize` and `NormalizedOrZero`;
- `Aabb3` discovery from the familiar `Bounds` term;
- affine `TransformPoint` versus projective `TryProjectPoint`.

Documentation search should map familiar terms such as `Bounds`, `GetPoint`,
`.normalized`, and `MultiplyPoint3x4` to the relevant Move concepts. These are
documentation/search aliases, not duplicate C++ API aliases.

No convenience operation should silently ignore translation, scale, or shear;
change angle units; clamp an input; select a fallback; or choose local/world
hierarchy preservation. Those choices are visible through semantic types,
qualified names, or required policy arguments.

## Prioritization

### Phase A: architectural proof

- backend primitive contract;
- `Vec2/3/4`, packed counterparts, angles, `Direction3`, and `Rotation3`;
- compile/layout/codegen baselines;
- scalar and RTM backend test targets.

### Phase B: spatial semantics

- `Point3`, `Normal3`, matrices, rigid/affine transforms;
- explicit coordinate and projection conventions;
- invariant/failure policy tests.

### Phase C: geometry

- ray/line/segment, plane, triangle, sphere, capsule, AABB, OBB;
- predicate/classification/detailed query tiers;
- prepared ray/AABB path.

### Phase D: culling and graphics

- frustum extraction/construction/classification;
- combined bounds and plane-mask culling;
- projection/unprojection and packed direction formats.

### Phase E: ecosystem and optimization

- formatting/serialization/backend adapters;
- batch/SoA facilities based on measured workloads;
- benchmark comparison against v1 and competing libraries.

## Acceptance criteria

The redesign is successful only if:

- ordinary call sites are clearer than v1 and competitive with engine APIs;
- invariant types do not add storage overhead;
- shared algorithms generate backend-quality code;
- v2 ray kernels continue to match the chosen backend in retired instructions;
- vector-only compile cost is materially below v1;
- adding geometry does not affect translation units that do not include it;
- all failure and coordinate conventions are documented and tested;
- familiar engine tasks have concise examples and intentional semantic
  differences are documented;
- names and overloads expose clamping, fallback, tolerance, normalization,
  transform capability, and projective failure where applicable;
- approachability does not require a parallel compatibility facade or increase
  core-header dependencies;
- float and double types have explicit, stable layouts;
- benchmarks compare semantically equivalent contracts.
