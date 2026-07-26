# Proposed API sketch

This file shows intended call-site shape. It is not a final declaration list.

## Core and semantic values

```cpp
#include <mv/math/Core.hpp>
#include <mv/math/semantic/Point3.hpp>
#include <mv/math/semantic/Direction3.hpp>

using namespace mv::math;
using namespace mv::math::literals;

Vec3f velocity(2.0f, 0.0f, 4.0f);
Point3f position = Point3f::Origin();

position += velocity * deltaTime;

auto direction = Direction3f::TryFrom(target - position);
if (direction)
{
    auto facing =
        Rotation3f::TryLookTowards(*direction, Direction3f::Up());
}

Rotation3f turn =
    Rotation3f::FromAxisAngle(Direction3f::Up(), 15.0_deg);
```

The same underlying optimized vector primitives serve `Vec3f`, `Point3f`, and
`Direction3f`; semantic wrappers add no tag or dispatch.

## Transform hierarchy

```cpp
#include <mv/math/transforms/RigidTransform3.hpp>

RigidTransform3f parent = ...;
RigidTransform3f local = ...;
RigidTransform3f world = Compose(local, parent);

Point3f worldPosition = TransformPoint(world, localPosition);
Direction3f worldForward = TransformDirection(world, localForward);
Normal3f worldNormal = TransformNormal(world, localNormal);

RigidTransform3f worldToLocal = world.Inverse();
```

Non-uniform authoring data converts explicitly:

```cpp
TrsTransform3f authored(translation, rotation, scale);
AffineTransform3f affine = authored.ToAffine();
auto normal = TryTransformNormal(affine, localNormal);
```

## Bounds and casts

```cpp
#include <mv/math/geometry/Aabb3.hpp>
#include <mv/math/geometry/Ray3.hpp>
#include <mv/math/queries/BoundsQueries.hpp>

auto bounds = Aabb3f::TryFromCenterHalfExtents(
    center,
    Vec3f(1.0f, 2.0f, 1.0f));

Ray3f ray(cameraPosition, cameraForward);

if (bounds && Intersects(ray, *bounds))
{
    auto hit = Intersect(ray, *bounds);
    if (hit)
    {
        Point3f entry = ray.PointAt(hit->EntryDistance);
    }
}
```

Repeated broad-phase testing:

```cpp
PreparedRay3f prepared(ray);

for (const PackedAabb3f& packedBounds : candidates)
{
    if (Intersects(prepared, Load(packedBounds)))
    {
        // Candidate survives broad phase.
    }
}
```

## Triangle hit

```cpp
#include <mv/math/geometry/Triangle3.hpp>
#include <mv/math/queries/LinearQueries.hpp>

Triangle3f triangle(a, b, c);

RayTriangleOptions options{
    .BackFaceMode = BackFaceMode::Cull,
};

if (auto hit = Intersect(ray, triangle, options))
{
    Point3f point = hit->Point;
    Vec3f barycentric = hit->Barycentric;
    Normal3f normal = hit->GeometricNormal;
}
```

## Frustum culling

```cpp
#include <mv/math/culling/Frustum3.hpp>
#include <mv/math/culling/PreparedFrustum3.hpp>

auto frustum = Frustum3f::FromViewProjection(
    view * projection,
    ClipConvention::DirectXReverseZ);

if (!frustum)
{
    // Invalid or singular extraction.
    return;
}

PreparedFrustum3f prepared(*frustum);
FrustumPlaneMask planes = FrustumPlaneMask::All;

for (const SceneNode& node : nodes)
{
    CullingResult result = Classify(prepared, node.Bounds, planes);
    if (result.Classification == Containment::Outside)
    {
        continue;
    }

    CullChildren(node, result.RemainingPlanes);
}
```

## Packed storage

```cpp
#include <mv/math/packed/Vectors.hpp>

static_assert(sizeof(PackedVec3f) == 12);

Vec3f compute = Load(packed);
compute = Rotate(rotation, compute);
Store(packed, compute);

TransformPoints(inputSpan, worldTransform, outputSpan);
```

The conversion is visible at single-value boundaries and amortizable through
batch functions.

## Graphics convention

```cpp
Mat4f projection = PerspectiveFov(
    70.0_deg,
    aspect,
    nearPlane,
    farPlane,
    Handedness::Left,
    ClipDepth::ZeroToOne,
    DepthDirection::Reverse);
```

Convenience named profiles can shorten common cases:

```cpp
PerspectiveFovDirectXReverseZ(...);
PerspectiveFovOpenGL(...);
```

## Interop

```cpp
#include <mv/math/interop/Rtm.hpp>

rtm::vector4f native =
    interop::rtm::ToNativeDirection(direction);
Direction3f direction =
    interop::rtm::TryFromNativeDirection(native).value_or(
        Direction3f::Forward());
```

Core headers do not expose these types or conversions.

## Proposed aliases

```cpp
using Point3f = Point3<float>;
using Direction3f = Direction3<float>;
using Normal3f = Normal3<float>;
using Rotation3f = Rotation3<float>;
using RigidTransform3f = RigidTransform3<float>;
using AffineTransform3f = AffineTransform3<float>;

using Ray3f = Ray3<float>;
using Segment3f = Segment3<float>;
using Plane3f = Plane3<float>;
using Triangle3f = Triangle3<float>;
using Sphere3f = Sphere3<float>;
using Capsule3f = Capsule3<float>;
using Aabb3f = Aabb3<float>;
using Obb3f = Obb3<float>;
using Frustum3f = Frustum3<float>;
```

Double aliases follow the same pattern.

## Design-checkpoint decisions still needed

1. Is `mv::math` the desired full namespace, or should primary types live
   directly in `mv`?
2. Should +Z forward/row-vector behavior remain the default core convention?
3. Should general `Quat` remain public alongside invariant `Rotation3`?
4. Should `SimilarityTransform3` be in the first implementation phase?
5. Should compute components use accessor methods only, or offer constrained
   proxy fields?
6. Is `std::optional` acceptable for fallible construction/query results, or
   should the library provide a small status-bearing result type?
7. Which detailed hit fields justify eager computation versus derived
   accessors?
8. Should public unchecked constructors exist at all, or remain internal until
   a benchmark proves the need?
