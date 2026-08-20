# Phase C capsule foundation

## Purpose

This slice adds the first sphere-swept volume needed for character controllers,
triggers, broad/narrow-phase bounds, and later shape casts without introducing
a second implementation of segment proximity.

## Surface and storage

- `Capsule3f` is a segment swept by a nonnegative sphere radius.
- `TryFromSegmentRadius` and `TryFromEndpointsRadius` reject non-finite values
  and negative radii.
- `Start`, `End`, `CenterLine`, `Radius`, `IsDegenerate`, and `IsFinite` make
  the representation contract explicit.
- `ClosestPoints(point, capsule)` returns the closest member of the solid
  capsule, the associated center-line fraction, and squared distance.
- `ClosestPoint`, `DistanceSquared`, `Distance`, and `Contains` provide concise
  point/capsule operations.
- `Intersects` covers sphere/capsule and capsule/capsule in both useful operand
  orders; touching counts as intersection.

The start point and radius share a `Vec4f`, followed by the end point. This
keeps the semantic type 32 bytes and 16-byte aligned rather than allowing a
`Segment3f` plus scalar radius to pad to 48 bytes. It remains trivially copyable
and standard layout.

## Provenance

The defining sphere-swept-volume reduction is cited directly on the type and
query implementations: Christer Ericson, *Real-Time Collision Detection*,
2005, sections 4.5 and 4.5.1. Point/capsule uses the existing point/segment
projection followed by radial sphere projection. Capsule overlap compares the
distance between center structures with the sum of radii. No upstream code was
copied.

## Verification

- constructor rejection and representation access;
- side-wall, end-cap, contained, boundary, and degenerate-sphere cases;
- sphere/capsule and capsule/capsule touching and separated cases;
- 32-byte layout and trivial-copy assertions;
- scalar and RTM behavior builds;
- generated capsule-pair loop and focused-header compile use.

## Remaining capsule work

- ray/capsule and segment/capsule intersection details;
- capsule/plane, capsule/AABB, capsule/OBB, and capsule/triangle queries;
- swept capsule/shape tests and contact/penetration results;
- comparative generated-code and working-set benchmarks.
