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
from the versions already present on `gh-pages`. It uploads that complete site
as a GitHub Pages artifact and deploys it through the `github-pages`
environment. The workflow serializes runs so two releases cannot update the
persistent site concurrently.

The repository's Pages publishing source must be **GitHub Actions**. The
`gh-pages` branch is retained as durable version storage, not used as the Pages
publishing source.

To build one version locally:

```sh
export MV_DOC_SOURCE="$PWD"
export MV_DOC_OUTPUT="$PWD/build/documentation/main"
export MV_DOC_VERSION=main
doxygen docs/Doxyfile
```

The generated site must not be committed to the source branch.
