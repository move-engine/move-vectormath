# Documentation publishing

The public documentation site is versioned and stored on the generated
`gh-pages` branch:

- pushes to `main` replace `/move-vectormath/main/`;
- release tags beginning with `v` publish a retained directory named after the
  tag, such as `/move-vectormath/v1.2.0/`;
- the site root lists every published version; and
- the pre-versioning site is retained at `/move-vectormath/legacy/`.

`.github/workflows/documentation.yml` owns publishing. It replaces only the
directory for the ref that triggered the run, then regenerates the root index
from the versions already present on `gh-pages`. The workflow serializes runs
so two releases cannot update the persistent site concurrently.

To build one version locally:

```sh
export MV_DOC_SOURCE="$PWD"
export MV_DOC_OUTPUT="$PWD/build/documentation/main"
export MV_DOC_VERSION=main
doxygen docs/Doxyfile
```

The generated site must not be committed to the source branch.
