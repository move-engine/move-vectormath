from pathlib import Path
import sys
import tempfile
import unittest


REPOSITORY_ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(REPOSITORY_ROOT / "tools" / "docs"))

import versioned_site  # noqa: E402


class VersionedSiteTests(unittest.TestCase):
    def setUp(self) -> None:
        self.temporary_directory = tempfile.TemporaryDirectory()
        self.site = Path(self.temporary_directory.name)

    def tearDown(self) -> None:
        self.temporary_directory.cleanup()

    def write_version(self, version: str) -> None:
        destination = self.site / version
        destination.mkdir(parents=True, exist_ok=True)
        (destination / "index.html").write_text(version, encoding="utf-8")

    def test_first_prepare_archives_the_unversioned_site(self) -> None:
        (self.site / "index.html").write_text("old", encoding="utf-8")
        (self.site / "asset.css").write_text("old", encoding="utf-8")
        (self.site / ".nojekyll").touch()

        versioned_site.prepare(self.site, "main")

        self.assertEqual(
            (self.site / "legacy" / "index.html").read_text(encoding="utf-8"),
            "old",
        )
        self.assertTrue((self.site / "legacy" / "asset.css").is_file())
        self.assertTrue((self.site / ".nojekyll").is_file())
        self.assertTrue((self.site / "main").is_dir())

    def test_prepare_replaces_only_the_requested_version(self) -> None:
        versioned_site.prepare(self.site, "main")
        self.write_version("main")
        self.write_version("v1.2.0")

        versioned_site.prepare(self.site, "main")

        self.assertFalse((self.site / "main" / "index.html").exists())
        self.assertTrue((self.site / "v1.2.0" / "index.html").is_file())

    def test_index_lists_main_releases_and_legacy(self) -> None:
        self.write_version("v1.2.0")
        self.write_version("v2.0.0")
        self.write_version("main")
        self.write_version("legacy")

        versioned_site.write_index(self.site)

        self.assertEqual(
            versioned_site.published_versions(self.site),
            ["main", "v2.0.0", "v1.2.0", "legacy"],
        )
        self.assertIn('href="main/"', (self.site / "index.html").read_text())

    def test_rejects_a_tag_that_could_escape_its_directory(self) -> None:
        with self.assertRaises(ValueError):
            versioned_site.prepare(self.site, "v1/../../main")


if __name__ == "__main__":
    unittest.main()
