# Comparative feature matrix

Legend:

- **Strong**: first-class, broad, and readily discoverable support
- **Partial**: present but incomplete, weakly typed, or coupled elsewhere
- **Low-level**: capable primitives with substantial caller assembly
- **Absent**: no meaningful reviewed support

This matrix compares the reviewed public experience, not private engine
implementation.

| Capability | Move v1 | Unity | Unreal 5.8 | Source | GMod Lua | Godot | DXM/DXTK |
|---|---|---|---|---|---|---|---|
| Vector game-loop operations | Partial | Strong | Strong | Strong | Strong | Strong | Strong |
| Explicit storage/compute split | Partial | Strong across layers | Partial | Partial | Absent | Partial | Strong |
| Backend hidden from public identity | No | Yes | Mostly | No | Yes | Yes | DXM yes; facade mostly |
| Float and double support | Strong | Split by API | Strong | Mostly float | Float | Build-wide choice | Mostly float |
| Point/vector type distinction | Absent | Absent | Absent | Absent | Absent | Absent | Absent |
| Unit-direction invariant type | Absent | Absent | Absent | Absent | Absent | Absent | Absent |
| Normal-specific type/transform | Absent | Named operations | Named operations | Convention | Convention | Named operations | Named operations |
| Rotation invariant type | Weak | Weak | Weak | Weak | Euler `Angle` | Weak | Weak |
| Angle unit type | Absent | Mixed layers | Named Deg/Rad ops | `QAngle` convention | `Angle` convention | Documented radians | Named radians |
| Explicit Euler order | Weak | Mathematics strong | Partial | Convention | Convention | Partial | Partial |
| Rigid transform | Absent | Strong | Within `TTransform` | Matrix/engine forms | Engine conversion | Within `Transform3D` | Matrix/facade |
| Affine transform | Matrix only | Strong | Strong | Strong | Engine conversion | Strong | Matrix |
| AABB value/operations | Absent | Strong | Strong | Strong/coupled | Utility/entity | Strong | Strong |
| OBB value/operations | Absent | Physics/coupled | Strong | Coupled | Utility/entity | Partial | Strong |
| Sphere/capsule primitives | Absent | Physics/coupled | Strong | Coupled | Utility/entity | Geometry helpers | Sphere strong |
| Ray/line/segment distinction | Absent | Ray only | Partial | Conflated trace | Often conflated | Segment/ray functions | Ray only |
| Typed rich hit results | Absent | Physics only | Out parameters | Engine traces | Lua results | Variant/arrays | Limited |
| Closest-point/distance queries | Absent | Partial | Strong | Partial | Partial | Strong | Partial |
| Containment classification | Absent | Mostly bool | Strong | Trace/coupled | Mostly bool | Partial | Strong |
| Frustum construction/culling | Absent | Strong basics | Strong | Engine-coupled | Engine-coupled | Strong | Strong |
| Prepared repeated-query API | Absent | Absent | Partial | Partial | Absent | Absent | Absent |
| Batch/array transforms | Absent | Mathematics/jobs | Engine systems | Partial | Absent | Packed arrays | Strong |
| Packed GPU formats | Absent | Ecosystem | Engine ecosystem | Engine formats | Absent | Octahedral normal | Strong |
| Safe/unchecked distinction | One safe policy | Mostly safe | Strong | Debug/config | Script-safe | Mostly safe | Low-level variants |
| Modular header cost | Weak | Binary engine/packages | Weak in engine | Weak | Not applicable | Binary engine | Strong by layer |
| PascalCase public naming | No | Engine yes | Yes | Mostly | Yes | No | SimpleMath yes |

## Cross-cutting conclusions

1. No reviewed API provides compile-time semantic separation for points, unit
   directions, normals, and rotations. Move can lead here.
2. DirectXMath has the clearest storage/compute architecture, but its raw API
   is too low-level for ordinary gameplay code.
3. Unity and Godot provide the most approachable common-operation vocabulary.
4. Unreal provides the broadest production geometry and explicit
   safe/preconditioned choices.
5. Source and Garry's Mod show why ray, segment, and swept hull must not share
   an ambiguous public name.
6. DirectXCollision provides the best compact cross-volume query matrix.
7. Every mature engine couples optional integration concerns into some math
   types. A standalone library should keep those adapters separate.

