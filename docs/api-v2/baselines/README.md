# API-v1 baseline

The v2 work begins from these permanent repository states:

| Repository | Branch | Commit |
|---|---|---|
| Move Vectormath | `main` | `d68818dd806bb5cbb5d23dbd5c4d0ef0a8570597` |
| vectormathbench | `master` | `d51e173` |
| RTM dependency | v2.3.1 | `745bd25673d93b46941eda55e0993327dbc12b53` |

The benchmark suite was configured from a clean temporary dependency cache
against the permanent Move commit. Its SSE4.2, AVX, and AVX2 executables all
built successfully.

Existing raw benchmark measurements remain outside version control under
`.local/benchmark-results/`. Durable runtime, compilation, and layout summaries
are recorded in:

- [`TypeLayouts.md`](TypeLayouts.md)
- [`CompilationPerformance.md`](CompilationPerformance.md)
- [`RuntimePerformance.md`](RuntimePerformance.md)
