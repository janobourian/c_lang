---
title: "Verified C23 Labs & Repository Code Map"
description: "Compilable, zero-warning ISO C23 reference programs and a full map of all hands-on C practice files in this repository."
icon: lucide/flask-conical
---

# Verified C23 Labs & Repository Code Map

Every core C23 language and library feature documented in this hub is backed by a **verified, zero-warning, runnable C23 program** in `examples/c23/` that compiles cleanly under `-std=c23 -Wall -Wextra -Werror -Wpedantic` both locally and in GitHub Actions CI.

---

## Verified C23 Reference Suite (`examples/c23/`)

Run the entire suite in one command from the repository root:

```bash
make test-c23
```

| File | C23 & Standard Library Features Exercised | Related Documentation |
| :--- | :--- | :--- |
| `examples/c23/01_c23_core_language.c` | `nullptr`, `nullptr_t`, `constexpr`, `typeof`/`typeof_unqual`, `auto`, `= {}` empty init, `0b` binary literals, `'` digit separators, `enum : uint8_t`, `[[nodiscard]]`, `[[fallthrough]]`, `[[maybe_unused]]`, labels before declarations, unnamed parameters, single-arg `static_assert` | [Module 19](../curriculum/tier-5-expert/19-modern-c-c99-to-c23.md) · [Language Reference](../reference/language/index.md) |
| `examples/c23/02_checked_arithmetic_and_bits.c` | `<stdckdint.h>` (`ckd_add`, `ckd_sub`, `ckd_mul`), overflow-safe buffer allocation sizing, `<stdbit.h>` via `__has_include` with portable compiler-builtin fallback (`stdc_leading_zeros`, `stdc_count_ones`, `stdc_has_single_bit`, `stdc_bit_width`, `stdc_bit_ceil`) | [Bit & Checked Int](../reference/library/numerics-bit-checked.md) · [Module 22](../curriculum/tier-5-expert/22-security-exploits-mitigations.md) |
| `examples/c23/03_generic_containers_and_qsort.c` | `_Generic` + `typeof` type-safe dispatch, overflow-free `qsort` comparator, `bsearch`, `<stdatomic.h>` lock-free atomic counters & acquire-release fences | [Algorithms](../reference/library/algorithms.md) · [Concurrency](../reference/library/concurrency.md) · [Module 18](../curriculum/tier-5-expert/18-advanced-data-structures.md) |
| `examples/c23/04_hardened_io_and_time.c` | Safe `snprintf` truncation checks, binary `tmpfile()` round-trip (`fwrite`/`fread`/`rewind`), strict `strtol`/`strtod` parsing with `errno` & `endptr`, `timespec_get`, `strftime`, `localeconv` | [I/O Reference](../reference/library/io.md) · [Chrono](../reference/library/chrono.md) · [Byte Strings](../reference/library/strings-byte.md) |
| `examples/c23/05_bitint_and_embed.c` | C23 `#embed` with `suffix(, 0)` and `__has_embed`, `_BitInt(24)` / `_BitInt(12)` with `uwb`/`wb` suffixes, and `__has_c_attribute(unsequenced)` | [Preprocessor & `#embed`](../reference/language/preprocessor.md) · [Attributes](../reference/language/attributes.md) |

---

## Repository Hands-On Source Tree

In addition to the verified C23 suite, this repository contains curated hands-on C practice directories mapped directly to curriculum tiers:

| Directory | Contents | Aligned Curriculum Modules |
| :--- | :--- | :--- |
| `negxx/` | Introductory `hello.c` sandbox | [neg04 · Dev Environment](../curriculum/pre-tier/neg04-dev-environment.md) |
| `c_approach/`, `drbook/` | K&R / *C Programming: A Modern Approach* foundational exercises | [neg03 – neg00](../curriculum/pre-tier/index.md) |
| `c_functions/` | Stack memory & pointer-swap examples (`memory.c`, `swap.c`) | [01 · Functions & ABI](../curriculum/tier-1-core/01-functions-abi-stack.md) |
| `c_arrays/`, `c_pointers/` | Pointer arithmetic, `argv`, sorting, generic `void *` pointers | [02 · Arrays & Pointers](../curriculum/tier-1-core/02-arrays-pointers-arithmetic.md) |
| `c_strings/` | Null-terminated string manipulation (`basic_usage.c`, `introduction.c`) | [03 · Strings & Bounds](../curriculum/tier-1-core/03-strings-bounds-safety.md) |
| `c_pointer_memory/` | Dynamic heap allocation (`p_malloc.c`) | [05 · Dynamic Memory](../curriculum/tier-2-runtime/05-dynamic-memory-arenas.md) |
| `c_preprocessor/` | Header & macro design (`functions.h`, `main.c`) | [06 · Preprocessor](../curriculum/tier-2-runtime/06-preprocessor-macros-headers.md) |
| `sysman/`, `linuxpi/` | POSIX processes (`c_pid.c`), files (`files.c`), `pthreads` (`p_thread.c`) | [10 · UNIX Syscalls](../curriculum/tier-3-systems/10-unix-syscalls-file-descriptors.md) · [12 · Concurrency](../curriculum/tier-3-systems/12-concurrency-pthreads-atomics.md) |
| `labs/` | Binary BMP generation & fractal/math rendering (`bimp.c`, `complex_fractal.c`, `landscape.c`, `dog.c`) | [07 · Stdlib](../curriculum/tier-2-runtime/07-standard-library-deep-dive.md) · [25 · Complex Math](../curriculum/tier-5-expert/25-unicode-locale-complex-math.md) |
| `masteringc23/` | Modern C23 playground (`00_hello.c`, `01_nulpnt.c`) | [19 · Modern C (C99–C23)](../curriculum/tier-5-expert/19-modern-c-c99-to-c23.md) |

---

## Compiling With Sanitizers (Recommended Practice)

When experimenting with any lab in this repository, always enable AddressSanitizer and UndefinedBehaviorSanitizer:

=== "Clang (macOS / Linux)"

    ```bash
    clang -std=c23 -Wall -Wextra -Werror -Wpedantic \
        -fsanitize=address,undefined -fno-omit-frame-pointer -g -O1 \
        examples/c23/01_c23_core_language.c -o /tmp/c23_lab && /tmp/c23_lab
    ```

=== "GCC 14+ (Linux)"

    ```bash
    gcc -std=c23 -Wall -Wextra -Werror -Wpedantic \
        -fsanitize=address,undefined -fno-omit-frame-pointer -g -O1 \
        examples/c23/01_c23_core_language.c -o /tmp/c23_lab && /tmp/c23_lab
    ```
