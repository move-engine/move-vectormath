# V1 compilation-performance baseline

## Representative workload

The measured translation unit exercised vector, quaternion, and matrix
construction and operations. Measurements used GCC 16, C++20, `-O2`, and five
warmed runs on the same local machine. Preprocessed line count and peak
resident memory provide more stable structural signals than elapsed time alone.

| Library/API | Warm compile | Peak memory | Preprocessed lines |
|---|---:|---:|---:|
| Sony Vectormath | 0.29 s | ~87 MiB | 24,432 |
| GLM | 0.35 s | ~98 MiB | 30,034 |
| DirectXMath | 0.37 s | ~98 MiB | 30,075 |
| DirectXTK SimpleMath | 0.68 s | ~147 MiB | 56,415 |
| RTM | 0.71 s | ~151 MiB | 62,553 |
| Move Vectormath v1 | 0.79 s | ~163 MiB | 69,941 |

A single Clang measurement showed the same ordering for the relevant subset:
Move 1.13 seconds, RTM 0.85 seconds, and GLM 0.40 seconds.

These are local comparative measurements, not portable absolute build times.
They indicate that the current Move facade adds meaningful parse/template cost
over its already substantial RTM backend.

## V2 acceptance criteria

- Measure minimal vector-only, rotation, matrix, transform, geometry, query,
  formatting, serialization, backend-adapter, and umbrella-header workloads.
- Keep optional modules from affecting a core-vector translation unit.
- Compare preprocessed lines, compiler front-end time, peak memory, object
  size, and clean parallel build time.
- Require the thin core facade to add only a small, explicitly budgeted cost
  over the selected backend.
- Track GCC, Clang, and MSVC because template and header costs differ.

