#!/usr/bin/env python3
"""Maintain the version index in the persistent GitHub Pages checkout."""

from __future__ import annotations

import argparse
import html
import json
from pathlib import Path
import re
import shutil


MARKER = ".move-vectormath-versioned"
RELEASE_PATTERN = re.compile(r"^v[0-9][0-9A-Za-z._-]*$")
PRESERVED_ROOT_ENTRIES = {".git", ".nojekyll", "CNAME", MARKER, "legacy"}


def validate_version(value: str) -> str:
    if value == "main" or RELEASE_PATTERN.fullmatch(value):
        return value
    raise ValueError(
        f"invalid documentation version {value!r}; expected 'main' or a "
        "slash-free release tag beginning with 'v' and a digit"
    )


def prepare(site: Path, version: str) -> None:
    """Archive an unversioned site once and empty this version's destination."""
    validate_version(version)
    site.mkdir(parents=True, exist_ok=True)

    marker = site / MARKER
    if not marker.exists():
        legacy = site / "legacy"
        legacy.mkdir(exist_ok=True)
        for entry in list(site.iterdir()):
            if entry.name in PRESERVED_ROOT_ENTRIES:
                continue
            if entry.name == "main" or RELEASE_PATTERN.fullmatch(entry.name):
                continue
            shutil.move(str(entry), legacy / entry.name)
        marker.write_text(
            "This branch is maintained by the documentation workflow.\n",
            encoding="utf-8",
        )

    destination = site / version
    if destination.exists():
        shutil.rmtree(destination)
    destination.mkdir(parents=True)


def release_key(version: str) -> tuple[tuple[int, object], ...]:
    """Sort common release tags naturally without imposing a tag format."""
    pieces = re.split(r"([0-9]+)", version.removeprefix("v"))
    return tuple(
        (0, int(piece)) if piece.isdigit() else (1, piece.lower())
        for piece in pieces
    )


def published_versions(site: Path) -> list[str]:
    versions: list[str] = []
    if (site / "main" / "index.html").is_file():
        versions.append("main")

    releases = sorted(
        (
            entry.name
            for entry in site.iterdir()
            if entry.is_dir()
            and RELEASE_PATTERN.fullmatch(entry.name)
            and (entry / "index.html").is_file()
        ),
        key=release_key,
        reverse=True,
    )
    versions.extend(releases)

    if (site / "legacy" / "index.html").is_file():
        versions.append("legacy")
    return versions


def write_index(site: Path) -> None:
    versions = published_versions(site)
    links = []
    for version in versions:
        if version == "main":
            description = "Latest development documentation"
        elif version == "legacy":
            description = "Archived pre-1.x documentation"
        else:
            description = "Release documentation"
        links.append(
            "<li><a href=\"{href}/\"><strong>{label}</strong>"
            "<span>{description}</span></a></li>".format(
                href=html.escape(version, quote=True),
                label=html.escape(version),
                description=html.escape(description),
            )
        )

    page = """<!doctype html>
<html lang="en">
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Move Vectormath documentation</title>
  <style>
    :root { color-scheme: light dark; font-family: system-ui, sans-serif; }
    body { margin: 0; background: #10141c; color: #e9edf5; }
    main { box-sizing: border-box; max-width: 52rem; margin: 0 auto;
           min-height: 100vh; padding: 10vh 1.5rem 4rem; }
    h1 { margin-bottom: .35rem; font-size: clamp(2rem, 6vw, 3.4rem); }
    p { color: #b9c2d3; font-size: 1.08rem; }
    ul { list-style: none; padding: 0; margin-top: 2rem; display: grid;
         gap: .8rem; }
    a { display: flex; justify-content: space-between; gap: 1rem;
        padding: 1rem 1.2rem; border: 1px solid #343d50; border-radius: .65rem;
        color: #e9edf5; background: #181e29; text-decoration: none; }
    a:hover, a:focus { border-color: #7aa2f7; background: #1d2533; }
    a span { color: #aab4c6; text-align: right; }
    footer { margin-top: 2rem; color: #7f899c; font-size: .9rem; }
  </style>
</head>
<body>
<main>
  <h1>Move Vectormath</h1>
  <p>Select the API documentation for the development branch or a release.</p>
  <ul>
    {links}
  </ul>
  <footer>Release documentation is retained when newer versions are published.</footer>
</main>
</body>
</html>
""".replace("{links}", "\n    ".join(links))

    (site / "index.html").write_text(page, encoding="utf-8")
    (site / "versions.json").write_text(
        json.dumps({"versions": versions}, indent=2) + "\n", encoding="utf-8"
    )
    (site / ".nojekyll").touch()


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser()
    subparsers = parser.add_subparsers(dest="command", required=True)

    prepare_parser = subparsers.add_parser("prepare")
    prepare_parser.add_argument("--site", type=Path, required=True)
    prepare_parser.add_argument("--version", required=True)

    index_parser = subparsers.add_parser("index")
    index_parser.add_argument("--site", type=Path, required=True)
    return parser.parse_args()


def main() -> None:
    args = parse_args()
    if args.command == "prepare":
        prepare(args.site.resolve(), args.version)
    else:
        write_index(args.site.resolve())


if __name__ == "__main__":
    main()
