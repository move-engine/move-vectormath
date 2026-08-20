# Geometry primitive proposal

## Design rules

- Primitive names state dimensionality.
- Stored values satisfy documented invariants.
- Construction from arbitrary external data is fallible or explicitly
  canonicalizing.
- Primitives contain geometry only—no entity, material, collision-layer, or
  allocator state.
- Common float and double aliases are provided.
- Types are trivially copyable where invariant-preserving private construction
  permits.

## Linear primitives

### Line3

An infinite line represented by `Point3 Origin` and `Direction3 Direction`.
Parameter `t` is signed physical distance:

```cpp
Point3 PointAt(T t) const;
```

### Ray3

A half-infinite ray with `Point3 Origin` and `Direction3 Direction`.
Valid parameters satisfy `t >= 0`; because direction is unit length, `t` is
physical distance. `TryFromOriginDirection` rejects a non-finite origin once;
the resulting `Ray3` type carries both the finite-origin and normalized-
direction invariants into repeated queries.

### Segment3

A finite segment represented by two endpoints. It exposes:

- start/end;
- displacement and length;
- `PointAtFraction(u)` for `u` in `[0,1]`;
- fallible direction for degenerate segments;
- closest-point and distance operations.

Source/Garry's Mod-style direction-times-distance traces convert explicitly to
`Segment3`; they are not called rays.

## Surface primitives

### Plane3

A normalized plane represented mathematically by a `Normal3` and signed offset:

```text
Dot(Normal, point) + Offset = 0
```

Proposed implementation packs normal XYZ and offset into one SIMD-sized
four-lane value (16 bytes for float) while preserving the normal invariant.
Construction supports point+normal, three non-collinear points, and normalized
coefficients.

Operations include signed distance, side classification, closest point,
projection, and flip.

### Triangle3

Three `Point3` vertices. It may be degenerate; operations that require area or
a normal are fallible. It exposes edges, centroid, area, barycentric
coordinates, support point, closest point, and winding-aware normal.

`TryFromPoints` rejects non-finite vertices. Finite construction is a type
invariant, while degeneracy remains valid data.

Degeneracy is data, not necessarily construction failure, because imported
meshes can contain degenerate triangles and queries need deterministic behavior.

## Volumes

### Sphere3

Center plus nonnegative radius. A float implementation can target a single
four-lane value with radius in W. Provides diameter, surface/volume, support,
containment, expansion, merging, and transform by rigid/similarity transforms.

General affine transform returns an ellipsoid or conservative sphere, selected
by an explicitly named function.

### Capsule3

A spine `Segment3` plus nonnegative radius. Degenerate spine naturally becomes
a sphere. Provides endpoints, center, axis/height where defined, support,
containment, closest point, and transform.

### Aabb3

Canonical representation is minimum and maximum points. Accessors derive
center, size, and half-extents. Construction includes:

- `FromMinMax` with validation;
- `TryFromCenterHalfExtents`;
- `FromPoints(span)`;
- `Empty` canonical builder state;
- `FromAbsoluteSize` for explicitly canonicalized external data.

Touching boundaries count as intersection. Empty bounds are represented by one
documented sentinel state and remain closed under expansion/merge.

Operations include point/volume containment, overlap, intersection, merge,
expand/grow, closest point, squared distance, support point, corners, surface
area, volume, and rigid/affine conservative transform.

### Obb3

Center, nonnegative half-extents, and `Rotation3` orientation. Provides local
axes, corners, support, closest point, containment, and rigid/affine transform.
A general affine transform may produce a parallelepiped; APIs must either
return that richer type or a named conservative OBB/AABB.

### BoxSphereBounds3

Optional renderer-oriented combination of AABB half-extents and sphere radius
around one center. It allows cheap sphere tests followed by tighter AABB tests.
This should remain in a culling/bounds header, not core geometry.

## Additional candidates

Useful later types:

- `Rect2`, `Circle2`, `Triangle2`, `Ray2`, and 2D equivalents;
- `Cone3`;
- `Ellipsoid3`;
- `Parallelepiped3`;
- `Viewport`;
- `Color` and packed colors;
- integer grid bounds.

They should not delay the initial 3D game/graphics set unless implementation
reuse makes them inexpensive.

## Proposed storage targets

| Float type | Target size | Notes |
|---|---:|---|
| `Ray3f` / `Line3f` | 32 bytes | point + direction |
| `Segment3f` | 32 bytes | two points |
| `Plane3f` | 16 bytes | normalized XYZ + offset |
| `Triangle3f` | 48 bytes | three points |
| `Sphere3f` | 16 bytes | center XYZ + radius |
| `Aabb3f` | 32 bytes | min/max |
| `Obb3f` | 48 bytes | center, half-extents, rotation |

These targets require prototype verification for triviality, alignment,
passing convention, and code generation.
