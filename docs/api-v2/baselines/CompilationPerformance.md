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

## C++20 module fan-out evidence

The post-cutover `mv.math` wrapper exports the canonical header implementation.
XMake installs module source and metadata and builds the BMI locally; it does
not distribute compiler-specific BMI artifacts. Direct import, a downstream
named module importing `mv.math`, RTM-backed behavior, and forced-scalar
behavior all pass locally with GCC 16 and Clang 22.

A 12-translation-unit fixture models game/graphics consumers using vectors,
semantic values, rotations, rigid transforms, projections, geometry queries,
and frustum culling. Measurements used Clang 22, XMake 3.1.0-dev, C++20,
Release mode, disabled object caching, and the matching LLVM dependency
scanner on the same local machine. `/usr/bin/time` reports aggregate child CPU
and the maximum resident set of any one process.

| Scenario | Headers wall / user | Module wall / user | Result |
|---|---:|---:|---|
| Clean serial (`-j1`) | 36.14 s / 21.50 s | 39.99 s / 6.61 s | 69% less compiler CPU, but scanner/BMI scheduling dominates wall time |
| Clean parallel (`-j18`) | 4.01 s / 27.70 s | 8.09 s / 7.93 s | 71% less compiler CPU, but a 4.08 s clean-build critical-path cost |
| No-op | 0.29 s / 0.24 s | 0.33 s / 0.27 s | Equivalent project-evaluation overhead |
| One consumer changed | 3.17 s / 1.98 s | 2.77 s / 0.65 s | 13% lower wall time and 67% less compiler CPU |

The incremental maximum resident set fell from about 162 MiB for reparsing the
umbrella header to 123 MiB for an importer. Clean module construction peaked at
about 169 MiB in the BMI compiler process, comparable to a single header parse;
the important reduction is avoiding many simultaneous full parses.

This evidence supports keeping modules opt-in. They materially reduce repeated
front-end work and improve ordinary source-only increments, but the current
12-file clean build is slower because every importer waits on the BMI. Broad
re-export from a large Move prelude is therefore not justified without a
higher-fan-out application measurement.

XMake's module dependency scan did not include transitively included public
headers in the BMI dependency file. The target therefore publishes an internal
content-revision definition derived from every public MVM header. Any public
header content change invalidates the BMI and all importers; a no-content
timestamp change does not. An explicit mutation test verified this behavior.
