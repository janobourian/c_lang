#!/usr/bin/env python3
"""Validate Zensical navigation targets, frontmatter, and relative Markdown links."""

from __future__ import annotations

import os
import re
import sys
import tomllib
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
DOCS = ROOT / "docs"
CONFIG = ROOT / "zensical.toml"

LINK_RE = re.compile(r"!?\[[^\]]*\]\(([^)\s]+)(?:\s+\"[^\"]*\")?\)")


def extract_nav_paths(node: object) -> list[str]:
    paths: list[str] = []
    if isinstance(node, str):
        paths.append(node)
    elif isinstance(node, list):
        for item in node:
            paths.extend(extract_nav_paths(item))
    elif isinstance(node, dict):
        for value in node.values():
            paths.extend(extract_nav_paths(value))
    return paths


def strip_code_blocks(text: str) -> str:
    out: list[str] = []
    in_fence = False
    for line in text.splitlines():
        stripped = line.lstrip()
        if stripped.startswith("```") or stripped.startswith("~~~"):
            in_fence = not in_fence
            continue
        if not in_fence:
            out.append(line)
    return "\n".join(out)


def main() -> int:
    errors: list[str] = []
    warnings: list[str] = []

    with CONFIG.open("rb") as fh:
        cfg = tomllib.load(fh)

    nav_paths = extract_nav_paths(cfg.get("project", {}).get("nav", []))
    nav_set = set(nav_paths)

    for rel in nav_paths:
        target = DOCS / rel
        if not target.is_file():
            errors.append(f"[NAV MISSING] {rel} listed in zensical.toml does not exist")

    md_files = sorted(DOCS.rglob("*.md"))
    for md in md_files:
        rel_doc = md.relative_to(DOCS).as_posix()
        if rel_doc not in nav_set:
            warnings.append(f"[UNLISTED] {rel_doc} is not referenced in zensical.toml nav")

        raw = md.read_text(encoding="utf-8")
        if not raw.startswith("---\n"):
            warnings.append(f"[NO FRONTMATTER] {rel_doc}")

        body = strip_code_blocks(raw)
        for match in LINK_RE.finditer(body):
            href = match.group(1)
            if href.startswith(("http://", "https://", "mailto:", "#", "tel:")):
                continue
            path_part = href.split("#", 1)[0].split("?", 1)[0]
            if not path_part:
                continue
            resolved = (md.parent / path_part).resolve()
            if not resolved.exists():
                errors.append(f"[BROKEN LINK] {rel_doc} -> {href}")

    print(f"Checked {len(md_files)} Markdown files and {len(nav_paths)} nav entries.")
    for w in warnings:
        print("WARN:", w)
    for e in errors:
        print("ERROR:", e)

    if errors:
        print(f"FAILED with {len(errors)} error(s).")
        return 1
    print("OK: 0 broken links, 0 missing nav files.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
