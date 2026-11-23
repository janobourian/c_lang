# C Language Mastery & Complete C23 Reference (`c_lang`)

[![Documentation Engine](https://img.shields.io/badge/Docs-Zensical-4f46e5)](https://zensical.org/)
[![ISO C Standard](https://img.shields.io/badge/ISO%20C-C23%20(ISO%2FIEC%209899%3A2024)-0891b2)](https://en.cppreference.com/c/23)
[![Deploy](https://img.shields.io/badge/Deploy-GitHub%20Pages%20%2B%20Actions-22c55e)](.github/workflows/docs.yml)

A complete, modern, **Zero-to-Systems-Architect C programming documentation hub and runnable codebase** combining:

1. **32-Module Guided Curriculum (`docs/curriculum/`)** — Progressive path from installing your first compiler (`neg04`) through memory models, pointers, arenas, UNIX syscalls, `epoll`/`kqueue`, lock-free concurrency, SIMD, exploit mitigations, ABI/FFI, and modern C23 (`26`).
2. **Complete C Reference (`docs/reference/`)** — Full, up-to-date parity with [`en.cppreference.com/c`](https://en.cppreference.com/c) covering **ISO/IEC 9899:2024 (C23)**, C89–C17 history, and C2y/C29 (`N3998`) previews:
   - **9 Language Reference sections**: Basic Concepts & Memory Model, Keywords, Preprocessor & `#embed`, Expressions & `_Generic`, Declarations & `typeof`, Initialization & `= {}`, Functions & Linkage, Statements, and all 7 C23 `[[attributes]]`.
   - **All 31 C23 Standard Library Headers** (+ C29 `<stdmchar.h>` preview): Types, Program Utilities, Variadic/Error, Dynamic Memory (`free_sized`), Byte/Multibyte/Wide Strings (`memset_explicit`, `strdup`, `char8_t`), Algorithms, Math/Fenv/Complex/Tgmath, `<stdbit.h>`, `<stdckdint.h>`, Chrono (`timegm`, `gmtime_r`), I/O (`%b`, `%wN`), Locale, and Concurrency (`<threads.h>`, `<stdatomic.h>`).
3. **C23 Migration, Compiler Matrix, ISO TS/TR & Symbol Index (`docs/standards/`)** — Breaking changes, GCC/Clang/Apple Clang/MSVC/libc support tables, ISO Technical Specifications (including TS 6010 Provenance), and a searchable A–Z Symbol Index.
4. **Verified C23 Code Examples (`examples/c23/`)** — Runnable, warning-free C23 test programs validated with `-std=c23 -Wall -Wextra -Werror -Wpedantic`.

---

## Quick Start

### Preview Documentation Locally (Zensical)

This site is built with [**Zensical**](https://zensical.org/) (the Rust/Python successor to deprecated MkDocs).

```bash
# Activate project .venv (optional; Makefile auto-uses .venv/bin/zensical)
source .venv/bin/activate
pip install -r requirements.txt

# Live preview with instant search, Mermaid diagrams & MathJax (http://127.0.0.1:8000)
make docs-serve

# Build production static site into ./site
make docs-build

# Run markdownlint & internal link checks
make lint-md
make check-links

# Compile & run all verified C23 reference examples
make test-c23
```

> Uses `.venv/bin/zensical` when present (installed via `requirements.txt` / `pyproject.toml`), with automatic fallback to `uvx zensical`.

---

## Deploy to GitHub Pages

Automated verification and deployment are preconfigured in [`.github/workflows/docs.yml`](.github/workflows/docs.yml):

1. In GitHub repository settings (**Settings → Pages → Build and deployment → Source**), select **GitHub Actions**.
2. Push to `main` — GitHub Actions builds the Zensical site, compiles & runs `examples/c23/*.c` with GCC `-std=c23`, and publishes to `https://janobourian.github.io/c_lang/`.

---

## Documentation Map

| Tab | Directory | Contents |
| :--- | :--- | :--- |
| **Home** | [`docs/index.md`](docs/index.md) | Hub entry point, learning tracks & quick start |
| **Curriculum (Zero to Master)** | [`docs/curriculum/`](docs/curriculum/index.md) | 32 modules across Tier 0 (`neg04`–`neg00`) through Tier 5 (`18`–`26`) |
| **Language Reference** | [`docs/reference/language/`](docs/reference/language/index.md) | Complete `en.cppreference.com/c/language` C89–C23 reference |
| **Standard Library** | [`docs/reference/library/`](docs/reference/library/index.md) | Complete catalog of all 31 C23 headers + C29 `<stdmchar.h>` |
| **C23 & Standards** | [`docs/standards/`](docs/standards/index.md) | Standards history, C23 migration guide, compiler/libc matrix, TS/TRs & Symbol Index |
| **Labs & Operations** | [`docs/labs/`](docs/labs/index.md) | Verified C23 suite (`examples/c23/`), repo code map & Zensical CI playbook |
