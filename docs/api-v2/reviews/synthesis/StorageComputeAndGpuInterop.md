# Storage, compute, and GPU interop review

## Question

Does the proposed v2 design preserve the practical motivation behind v1's
fast and non-fast types?

That motivation is valid:

- dense values reduce cache, memory, and upload bandwidth;
- SIMD-friendly values avoid repeated load/repack work during CPU computation;
- shader-facing values need exact, testable byte layouts;
- ordinary game code should not need to manipulate backend registers or write
  bespoke packing loops.

The v2 proposal preserves the idea, but its current two-way split between
`Vec3f` and `PackedVec3f` is not yet complete enough to guarantee those goals.

## What v1 got right

V1 recognized that one conceptual three-vector has at least two useful CPU
representations:

| Type | Size | Alignment | Intended use |
| --- | ---: | ---: | --- |
| `float3` | 16 | 16 | RTM/SIMD computation |
| `vec3f` | 12 | 4 | compact scalar storage |

The `fast()` and `storable()` conversions made the tradeoff accessible, and
the fast type could remain in an RTM-native representation across an operation
chain. That is preferable to a facade that loads a packed three-vector for
every individual method call.

The problems were not the distinction itself. They were that:

- acceleration policy participated in public type identity;
- aliases did not clearly communicate layout;
- backend base classes and conversions leaked through public headers;
- storage, compute, serialization, and backend interop concerns accumulated in
  the same type family;
- single-value conversion APIs encouraged accidental repeated transitions.

V2 should preserve the choice while changing its boundaries.

## There is no universal GPU float3 layout

“GPU blittable” must always name a buffer/layout contract.

- A compact C++ `{float X, Y, Z;}` naturally has size 12 and alignment 4. This
  is useful for vertex attributes, tightly packed CPU data, Metal's packed
  float3, and shader layouts that explicitly permit scalar alignment.
- Vulkan base/extended layouts still give a three-component vector 16-byte
  alignment. Arrays consequently use a 16-byte stride. Scalar block layout can
  permit tighter placement, but it is a feature/layout choice and the Vulkan
  specification notes that base alignment can be more performant.
- WGSL defines `vec3<f32>` with size 12 and alignment 16; an array therefore
  has stride 16. A following scalar in a structure may occupy offset 12.
- HLSL constant buffers pack values into 16-byte registers. A `float3` and
  following scalar can share one register, while arrays and nested structures
  follow additional packing rules.
- Matrix layout also depends on row/column major declaration and vector stride;
  a 36-byte CPU 3x3 matrix is not a generally valid shader-block `mat3`.

Consequently, `PackedVec3f` cannot honestly mean both “12-byte CPU storage” and
“safe for every shader buffer.” A 16-byte compute `Vec3f` may be byte-compatible
with a shader float3 array for selected layouts, but only if Move publishes and
tests its component offsets and padding contract.

## Gaps in the current v2 proposal

### GPU compatibility is implied rather than specified

The current design specifies sizes and alignments for `Vec3f` and
`PackedVec3f`, but not:

- component byte offsets;
- padding-lane policy;
- array stride;
- matrix major order in transfer types;
- which Vulkan, HLSL, Metal, or WGSL layouts each type satisfies;
- whether semantic wrappers are upload-compatible with their compute value;
- whether data read back from a GPU can re-enter an invariant type unchecked.

Trivial copyability alone is not a shader-layout contract.

### Packed spans do not cover typical game data

Game and graphics data is often interleaved:

```text
vertex: position | normal | tangent | UV
particle: position | age | velocity | lifetime
instance: transform | bounds | material index
```

An API accepting only `span<PackedVec3f>` cannot operate directly on a position
field inside those structures. Requiring callers to gather into a temporary
array defeats the storage and bandwidth goal.

The batch layer needs strided/field views and fused load-operate-store kernels,
not only contiguous spans.

### Packed versus compute is a workload decision

There is no single correct persistent CPU representation:

- CPU-hot vectors updated many times per frame often justify 16-byte compute
  storage so values load directly and remain SIMD-friendly.
- bandwidth/cache-hot collections with little arithmetic often favor 12-byte
  packed storage.
- GPU-owned or upload-heavy data should follow the selected shader resource
  layout, which may be 12-byte or 16-byte stride.
- high-volume kernels may favor SoA or AoSoA layouts rather than either scalar
  AoS representation.

The API should make both common choices ergonomic and let benchmarks identify
their crossover points.

The decision is dimension-specific. A padded 16-byte three-vector commonly
matches SIMD and shader-array constraints, while forcing a two-vector from 8
to 16 bytes may double storage for UVs or 2D particles without a corresponding
win. Phase A should measure each representation rather than applying one
alignment rule to every vector dimension.

### Single-value explicitness is insufficient

Making `Load` and `Store` explicit is correct, but a practical design also
needs:

- direct unaligned load3/store3 implementations without temporary arrays;
- bulk conversion;
- fused transforms and queries over storage views;
- a checked direct-upload view when bytes already match;
- explicit matrix transpose/major-order conversion;
- no forced round trip through a heap-allocated compute array.

## Recommended model

Keep three concerns independent:

1. **semantic role** — vector, point, direction, normal, rotation;
2. **representation contract** — compute, compact CPU storage, GPU transfer
   layout, or encoded format;
3. **processing shape** — single value, contiguous AoS, strided AoS, SoA, or
   AoSoA.

Backend choice remains internal and target-wide. It should not be used as the
representation selector.

### Compute values

`Vec3f`, `Point3f`, `Direction3f`, and `Normal3f` remain 16-byte,
SIMD-friendly values. Phase A should prototype a stable byte representation
with XYZ at offsets 0, 4, and 8 and a non-semantic fourth slot. If that can be
made trivially copyable and direct-upload compatible without code-generation
regression, it should become part of the public layout contract.

The fourth slot is never homogeneous point/direction state and must not make a
three-vector interchangeable with `Vec4f`.

### Compact storage values

`PackedVec3f` remains a size-12, alignment-4 aggregate with public fields.
Semantic storage counterparts may share its representation while remaining
distinct C++ types where load validity differs.

Packed types may provide construction, component access, and equality, but
broad arithmetic belongs on compute values. This prevents convenient-looking
loops from repeatedly loading and storing each operation.

Common composite storage also needs deliberate coverage. Large AABB arrays can
use a 24-byte packed min/max form rather than a 32-byte compute form; a
shader-layout AABB using two 16-byte float3 slots may still be 32 bytes.
Likewise, packed 3x3 matrices and rigid transforms are not automatically valid
shader structures. Composite packed/GPU types should be added where real
assets, ECS components, culling arrays, or instance buffers justify them.

### GPU transfer values

An opt-in GPU-layout module should provide explicit building blocks rather than
one misleading `GpuVec3f`. At minimum the prototype should cover:

- a 12-byte float3 payload;
- a 16-byte-aligned/16-byte-stride float3 array slot;
- float3-plus-scalar packing in one 16-byte slot;
- row- and column-major 3x3/3x4/4x4 matrix transfer forms;
- common vertex encodings such as half, normalized integer, and
  10:10:10:2 formats;
- octahedral direction/normal encoding.

Names should communicate byte/layout behavior; `gpu::Float3Slot16`, for
example, says that the value occupies a 16-byte slot without implying that
every GPU requires that representation. Exact names should follow a prototype
across Vulkan, Direct3D, Metal, and WGSL.

### Layout declarations and validation

Move should provide traits and compile-time assertions for its leaf transfer
types:

```cpp
static_assert(sizeof(PackedVec3f) == 12);
static_assert(alignof(PackedVec3f) == 4);
static_assert(sizeof(gpu::Float3Slot16) == 16);
static_assert(alignof(gpu::Float3Slot16) == 16);
static_assert(offsetof(GpuPositionRadius, Radius) == 12);
```

Application buffer structures still need validation against generated shader
reflection or shared schema metadata. A math library cannot infer a complete
shader block layout from C++ alone, but it can provide stable leaf types,
offset traits, and helpers that make such validation straightforward.

### Direct upload is a checked capability

Byte-compatible types should support a zero-copy upload view:

```cpp
auto bytes = AsGpuBytes<GpuLayout::Float3Array16>(positions);
```

This is available only when compile-time layout traits prove compatibility.
Other combinations use `Store`/`Pack` or bulk conversion. An invariant
`Direction3f` may be uploaded directly when compatible; GPU readback must use
validated construction unless the producing shader contract is trusted through
an explicitly unchecked boundary.

### Batch and strided processing

Batch APIs should accept:

- contiguous compute spans;
- contiguous packed spans;
- strided views into interleaved structures;
- separate input/output representations;
- fused operations that load once and store once.

SoA/AoSoA containers can remain deferred, but the internal kernel interface
must not assume every batch is a contiguous array of public compute values.

## Required measurements

The architecture proof should add realistic crossover benchmarks:

- integrate particles stored as 12-byte packed versus 16-byte compute values;
- update 2D particles/UV-like data using 8-byte versus 16-byte two-vectors;
- transform interleaved vertex positions and normals;
- transform packed input directly into GPU-layout output;
- repeated operations per load at several chain lengths;
- contiguous versus strided inputs;
- direct-upload memcpy versus layout conversion;
- matrix upload with and without major-order conversion;
- scalar, SSE, AVX, and AVX2 code generation where supported;
- working sets spanning L1, L2, L3, and main-memory pressure;
- packed 24-byte versus compute/GPU-slot 32-byte AABB traversal and culling.

Report useful work per element, bytes moved, retired instructions, and cache
misses—not only nanoseconds. Suspiciously low timings must be guarded with
observable outputs and dataset variation.

## Conclusion

The new design should not discard the original fast/storage insight. It should
generalize it.

The recommended v2 model is:

- one stable, ergonomic compute identity with hidden backend selection;
- explicit compact storage types;
- explicit GPU transfer/layout types;
- zero-copy upload only when layout traits prove it;
- fused bulk and strided algorithms to amortize necessary conversion;
- benchmarked guidance for choosing persistent CPU representation.

This retains the performance reason for v1's fast types while removing backend
policy from ordinary public type identity.
