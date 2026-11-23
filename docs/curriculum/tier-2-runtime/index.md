---
title: "Tier 2 · Runtime & Preprocessor (Modules 05 – 08)"
description: "Dynamic heap allocation, linear arenas, preprocessor macro hygiene, standard library internals, error handling, and signals."
icon: lucide/database
---

# Tier 2 · Runtime & Preprocessor (`05` – `08`)

Master heap lifecycle discipline, custom memory allocators, translation-phase metaprogramming, core standard library guarantees, and deterministic fault recovery.

<div class="grid cards" markdown>

- **05 · Dynamic Memory, Heaps & Arenas**

    Allocator internals (`brk`/`mmap`), `malloc`/`calloc`/`realloc`/`free`, Use-After-Free prevention, linear arenas, and intrusive lists.

    [:octicons-arrow-right-24: Open Module 05](05-dynamic-memory-arenas.md)

- **06 · Preprocessor, Macros & Headers**

    CPP translation phases, `do { ... } while (0)` hygiene, `#` stringification, `##` token pasting, include guards, and C23 `#embed`.

    [:octicons-arrow-right-24: Open Module 06](06-preprocessor-macros-headers.md)

- **07 · Standard Library Deep Dive**

    `<stdio.h>` stream buffering (`setvbuf`), `<stdlib.h>` `qsort`/`bsearch`, monotonic `<time.h>` clocks, and IEEE-754 `<math.h>`.

    [:octicons-arrow-right-24: Open Module 07](07-standard-library-deep-dive.md)

- **08 · Error Handling, Signals & `setjmp`**

    Thread-local `<errno.h>`, reentrant `strerror_r`, POSIX `sigaction`, async-signal safety, and structured `setjmp`/`longjmp`.

    [:octicons-arrow-right-24: Open Module 08](08-error-handling-signals-setjmp.md)

</div>

**Previous Tier:** [Tier 1 · Core Architecture](../tier-1-core/index.md) · **Next Tier:** [Tier 3 · Toolchains, OS & Concurrency](../tier-3-systems/index.md)
