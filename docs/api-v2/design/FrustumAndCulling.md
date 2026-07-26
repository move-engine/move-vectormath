# Frustum and culling proposal

## Frustum3

The primary culling form stores up to six inward-facing normalized planes:

- left;
- right;
- bottom;
- top;
- near;
- far.

An active-plane mask allows infinite-far or specialized frusta without an
invalid sentinel plane.

Construction:

```cpp
Frustum3f::FromPlanes(...);
Frustum3f::FromViewProjection(matrix, ClipConvention::DirectX);
Frustum3f::FromViewProjection(matrix, ClipConvention::OpenGL);
Frustum3f::FromCamera(cameraPose, perspective);
```

Extraction accounts explicitly for:

- row-vector matrix convention;
- clip-depth range (`[0,1]` or `[-1,1]`);
- reverse Z;
- finite or infinite far plane;
- handedness already encoded by the view/projection construction.

Every extracted plane is normalized once, allowing distance-based tests
without per-object normalization.

## Parametric camera frusta

Optional `PerspectiveFrustum3` and `OrthographicFrustum3` values retain camera
parameters (pose, slopes/extents, near/far) for editing, corner generation, and
projection construction. They convert to the plane culling form.

This mirrors DirectXCollision's compact origin/orientation/slopes frustum while
keeping plane tests direct.

## Queries

Required operations:

- `Contains(Point3)`;
- `Classify(Sphere3)`;
- `Classify(Aabb3)`;
- `Classify(Obb3)`;
- `Classify(Frustum3)`;
- triangle classification;
- corners when all required planes are finite/nonparallel;
- plane access by `FrustumPlane` enum;
- rigid transformation.

Touching a plane is `Intersecting`, subject to tolerance.

## Hierarchical plane masks

Culling hierarchies benefit from carrying forward only planes that intersected
the parent:

```cpp
struct CullingResult
{
    Containment Classification;
    FrustumPlaneMask RemainingPlanes;
};

CullingResult Classify(
    const PreparedFrustum3f&,
    const Aabb3f&,
    FrustumPlaneMask active);
```

If a parent is fully inside a plane, children need not test it. If a parent is
outside any plane, the query exits immediately.

## PreparedFrustum3

The prepared form stores backend-friendly plane data, absolute normals for
AABB radius tests, and any packet/transposed representation justified by
benchmarks. It is derived once per camera/frustum update and reused over many
bounds.

The ordinary `Frustum3` remains compact and backend-neutral in public meaning.

## Conservative behavior

Culling APIs prioritize no false negatives:

- affine-transformed bounds use named conservative conversions;
- tolerance expands the accepted region;
- invalid/non-finite bounds are rejected during construction or classified
  through an explicit validation path;
- exact intersection geometry is separate from conservative visibility.

## Graphics helpers

An optional graphics module adds:

- project world point to normalized device coordinates;
- project to viewport;
- unproject screen/depth to world point;
- construct camera ray from viewport coordinates;
- reverse-Z projection constructors;
- jittered projection support without corrupting the stable culling frustum.

Jittered rendering and culling projections should be distinct values because
temporal jitter can otherwise cause visible culling instability.

## Benchmark workloads

- thousands of packed AABBs against one prepared frustum;
- hierarchical bounds with plane-mask propagation;
- sphere-then-AABB combined bounds;
- OBB culling;
- finite, reverse-Z, and infinite-far extraction;
- scalar one-shot versus prepared and batch layouts;
- correctness corpus around plane tangency and very large coordinates.

