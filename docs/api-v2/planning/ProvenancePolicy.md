# Algorithm provenance policy

## Requirement

Every nontrivial algorithm generated or materially rewritten by an automated
assistant must have explicit, reviewable provenance before it is accepted.
Source consultation happens before implementation. Original papers and
standards are preferred; compatible-licensed production libraries are the
fallback and may also be used as independent validation.

This requirement applies to scalar and SIMD algorithms, numerical robustness
work, geometry and intersection routines, transforms, packing and layout
rules, approximations, and optimized special cases. Plain storage declarations,
accessors, overload forwarding, and direct expressions of elementary
definitions may share a file-level entry instead of one entry per function.

## Required record

Each implementation entry in
[`AlgorithmProvenance.md`](../implementation/AlgorithmProvenance.md) records:

1. the functions and files covered;
2. whether the code is paper-derived, library-adapted, source-validated
   independent work, a Move-specific derivation, or an elementary definition;
3. paper title, authors, publication, DOI, equation/section when useful, or an
   immutable repository commit and file path;
4. the upstream license for any source-code implementation consulted;
5. deliberate differences in conventions, domains, tolerances, degeneracy,
   precision, and returned data;
6. validation and tests, including whether a purported oracle is genuinely
   independent of the production algorithm.

An entry must not claim that code was derived from a source merely because a
later audit found a similar implementation. Retrospective work is labeled
"source-validated independent derivation" and retains its actual history.

## Source order

Use sources in this order when practical:

1. original peer-reviewed paper or normative specification;
2. author-maintained reference implementation;
3. established production library under a compatible license;
4. textbook or other authoritative technical treatment;
5. a documented Move-specific derivation from cited definitions.

MIT, BSD, Apache-2.0, zlib, and similarly permissive sources are preferred for
code-level comparison. Do not adapt incompatible copyleft or source-available
code into the library. If a substantial portion is copied or closely adapted,
preserve every notice required by its license; ordinarily, re-express the
algorithm against the paper and use library code only for cross-checking.

## Source comments

Nontrivial implementation sites carry a short, self-contained citation. A
reader must be able to identify the source without decoding an internal ID or
opening the provenance register. For example:

```cpp
// Moller-Trumbore ray/triangle test (JGT 1997, doi:10.1080/10867651.1997.10487468);
// Move adds explicit culling, finite-input, and boundary policy.
```

Prefer an author/algorithm or library/version and file, plus a terse note about
important Move-specific differences. The detailed register supplements these
comments with immutable revisions, licenses, and longer rationale; it is not a
lookup table required to understand the header.

## Review gate

Before a CPU feature is accepted:

- its inline source comment and detailed provenance entry exist;
- the cited material was actually inspected during the implementation work;
- license compatibility has been recorded;
- deviations from the cited algorithm are tested;
- at least one reviewer checks the implementation against the cited source;
- performance-driven changes update the provenance entry when they alter the
  algorithm rather than only its expression.

Code generated without this evidence remains provisional. The provenance gate
takes priority over adding the next geometry primitive, but the documentation
work should remain small enough that it does not displace the CPU roadmap.
