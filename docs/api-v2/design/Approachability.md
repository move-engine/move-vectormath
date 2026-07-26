# Approachability and semantic transparency

## Goal

Move should feel approachable to game and graphics programmers without
inheriting accidental complexity from the APIs they already know. Familiarity
reduces search and learning cost; it does not override correctness,
predictability, or performance.

Unity is the clearest case because many prospective users will arrive knowing
`Vector3`, `Quaternion`, `Transform`, `Bounds`, and `Ray`. The same policy
applies to Unreal, Godot, Source, DirectXMath, and shader-language conventions:
use established vocabulary as discovery evidence, then review every contract
on its own merits.

## Familiarity without imitation

The public surface follows four rules:

1. Keep a familiar noun or verb when it accurately names the operation.
2. Expose meaningful policy choices in types, names, or required arguments.
3. Do not preserve a familiar contract that silently discards information,
   changes units, falls back, clamps, or selects hierarchy behavior.
4. Explain intentional differences through task-oriented migration material
   rather than compatibility aliases.

This favors `Dot`, `Cross`, `MoveTowards`, `TransformPoint`, `Ray3::PointAt`,
and `Lerp` because their intent is broadly recognizable. It rejects approximate
`operator==`, ambiguous `.Normalized()`, defaulted scene reparenting policy,
and transform helpers that silently ignore scale.

## Semantic transparency

An operation name and its accepted types should answer the questions that
affect correctness:

- Is translation applied?
- Are scale and shear applied?
- Is a unit-length invariant required or restored?
- Is interpolation clamped?
- Are angles degrees or radians?
- Can homogeneous division, inversion, or normalization fail?
- Does a mutation preserve local or world state?

When the answer changes the mathematical operation, it must not live only in
documentation.

Examples:

```cpp
Point3f worldPoint = TransformPoint(localToWorld, localPoint);
Vec3f worldVelocity = TransformVector(localToWorld, localVelocity);
Direction3f worldForward = TransformDirection(rigidLocalToWorld, localForward);
auto worldNormal = TransformNormal(affineLocalToWorld, localNormal);

Vec3f blended = Lerp(a, b, t);
Vec3f clampedBlend = LerpClamped(a, b, t);

auto direction = Direction3f::TryFrom(input);
Vec3f fallbackValue = NormalizedOrZero(input);
```

The absence of `TransformDirection(AffineTransform3, Direction3)` is
intentional: a general affine transform can change length and direction through
scale and shear. Callers can apply `TransformVector` and explicitly normalize,
or extract/supply the rotation policy they actually intend.

## Type discoverability

Semantic types should not make ordinary vector work ceremonial:

- `Vec2/3/4` remain the obvious general arithmetic types;
- `Point`, `Direction`, and `Normal` appear where their algebra or transform
  behavior materially differs;
- common aliases (`Vec3f`, `Point3f`, `Direction3f`) are short;
- conversion at engine, file, and GPU boundaries is named and direct;
- compiler diagnostics should mention the semantic mismatch rather than expose
  backend implementation types.

Documentation should lead with tasks, not the type hierarchy. Searching the
reference for familiar terms such as "Bounds", "GetPoint", "normalized", or
"TransformDirection" should lead to the corresponding Move operation and its
intentional differences even when that term is not a public compatibility
alias.

## Unity on-ramp

The Unity review contains the detailed mapping. The primary teaching sequence
should be:

1. `Vector3` arithmetic maps directly to `Vec3f`.
2. Locations, unit directions, and surface normals gain distinct types only
   when the distinction prevents an invalid operation.
3. Unity's scene `Transform` maps to several pure value types:
   `RigidTransform3f`, authoring-oriented `TrsTransform3f`, and
   `AffineTransform3f`.
4. Familiar transform verbs remain, but overload availability communicates
   which mathematical behavior is valid.
5. Silent Unity policies become explicit: `LerpClamped`,
   `NormalizedOrZero`, `IsNearlyEqual`, `TryProjectPoint`, and named angle
   units.

Examples should include a "Unity equivalent" label where useful, followed by a
short explanation of any deliberate contract change. Move should not create a
second Unity-shaped facade: duplicate names increase documentation,
instantiation, and maintenance cost while weakening the single coherent API.

## Scene and math boundaries

A pure transform value does not own a parent, cache world state, dirty a
hierarchy, or choose reparenting policy. Those are engine responsibilities.

If Move Engine later presents scene helpers, their behavior should remain
explicit:

```cpp
node.SetParent(parent, ReparentPolicy::PreserveWorld);
node.SetParent(parent, ReparentPolicy::PreserveLocal);
```

There should be no defaulted boolean whose effect includes recomputing local
translation, rotation, or scale. The scene API may use familiar words, but it
must not blur the boundary back into the math library.

## Review checklist

A proposed convenience API is acceptable when:

- a newcomer can search for it using common game/graphics terminology;
- the name and types expose behavior that changes the mathematical result;
- failure and fallback are distinct;
- it does not require a compatibility wrapper or duplicate implementation;
- it composes with semantic types without returning to raw-vector ambiguity;
- its convenience does not add headers or template instantiations to unrelated
  core use;
- its behavior can be stated in one sentence and tested directly.
