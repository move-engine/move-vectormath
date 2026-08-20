# Intersection and spatial-query proposal

## Three query levels

Use names that match the amount of requested work:

```cpp
bool Intersects(const Ray3f&, const Aabb3f&);
Containment Classify(const Frustum3f&, const Aabb3f&);
std::optional<RayAabbHit3f> Intersect(const Ray3f&, const Aabb3f&);
```

`Intersects` may use an early-out predicate and need not compute detailed hit
data. `Classify` returns a semantic enum. `Intersect` returns a typed result.

## Classifications

```cpp
enum class Containment
{
    Outside,
    Intersecting,
    Inside
};

enum class PlaneSide
{
    Back,
    Intersecting,
    Front
};

enum class FaceOrientation
{
    Front,
    Back
};
```

Names use object-relative semantics consistently. Documentation defines plane
inside orientation and boundary tolerance.

## Hit results

Do not create one oversized universal hit. Proposed specialized results:

```cpp
template <Floating T>
struct RayPlaneHit
{
    T Distance;
    Point3<T> Point;
    Normal3<T> Normal;
};

template <Floating T>
struct RayTriangleHit
{
    T Distance;
    Point3<T> Point;
    Normal3<T> GeometricNormal;
    Vec3<T> Barycentric;
    FaceOrientation Face;
};

template <Floating T>
struct RayBoundsHit
{
    T EntryDistance;
    T ExitDistance;
    Normal3<T> EntryNormal;
    Normal3<T> ExitNormal;
    bool StartsInside;
};
```

Exact fields will be refined against code-generation and use-case benchmarks.
For example, detailed point calculation may be an accessor taking the ray if
storing it is redundant in a hot result array.

Closest-point queries similarly return named pairs/features when needed:

```cpp
SegmentPairClosest3<T>
PointTriangleClosest3<T>
Barycentric3<T>
```

No result contains dynamic allocation.

## Parameter conventions

- line `t`: signed distance;
- ray `t`: nonnegative distance;
- segment fraction: `[0,1]`;
- segment distance: `[0, Length]` only in explicitly named APIs;
- triangle barycentrics: named or documented vertex order;
- touching counts as intersection;
- starting inside a volume is reported explicitly;
- back-face culling is an option, not an undocumented default.

Unit ray/line direction makes distance semantics reliable.

## Initial coverage

### Detailed cast/intersection

- line/ray/segment with plane;
- ray/segment with triangle;
- ray/segment with sphere;
- ray/segment with AABB;
- ray/segment with OBB;
- segment with capsule.

### Predicate/classification

- sphere/sphere, sphere/AABB, sphere/OBB;
- AABB/AABB, AABB/OBB, OBB/OBB;
- capsule/sphere, capsule/capsule, capsule/AABB;
- triangle/triangle, triangle/AABB;
- plane against sphere/AABB/OBB/triangle;
- frustum against point/sphere/AABB/OBB/frustum.

### Closest point/distance

- point to line/ray/segment/plane/triangle/AABB/OBB/sphere/capsule;
- segment to segment;
- segment to triangle;
- primitive squared-distance variants where meaningful.

DirectXCollision provides the initial cross-volume completeness target; Godot
and Unreal provide the closest-point breadth target.

## Prepared queries

Repeated tests should make reusable derived data explicit:

```cpp
PreparedRay3f prepared(ray);       // reciprocal direction and sign/masks
bool Intersects(prepared, box);

PreparedFrustum3f prepared(frustum);
CullingResult Classify(prepared, box, activePlaneMask);
```

`PreparedRay3` handles zero direction components without exposing sentinel
reciprocals as ordinary vectors. It may also store ray origin in the layout
best suited to the backend.

Prepared objects are immutable, cheap to copy if their size allows, and tied to
the same precision/backend as the source. Construction cost is benchmarked
separately from amortized use.

## Batch queries

Span-based forms avoid per-call scaffolding:

```cpp
CullAabbs(
    const PreparedFrustum3f& frustum,
    std::span<const PackedAabb3f> bounds,
    std::span<Containment> results);
```

Future SoA packets can be added behind separate headers after measurement.
The scalar one-object API should remain the semantic reference.

## Robustness policy

Algorithms define behavior for:

- parallel/coplanar line-plane cases;
- degenerate triangles and segments;
- tangency;
- starts-inside;
- non-finite input;
- tolerance at large and small scales.

Invariant primitives reject non-finite construction by default. Queries do not
silently repair invalid objects. Degenerate primitives that are explicitly
allowed have deterministic lower-dimensional behavior or a documented miss.

## Safe versus unchecked

Invariant types remove most repeated checks. Additional unchecked public
queries should be introduced only when generated-code evidence shows a
meaningful cost and their precondition cannot be expressed by an existing
type. They use explicit names/tags and validation-build assertions.

`Ray3` and `Triangle3` therefore validate arbitrary construction data through
fallible factories, not on every query. This preserves a single public query
tier: callers handling untrusted data pay at ingestion, while game-loop queries
consume invariant-bearing values without an unchecked overload.
