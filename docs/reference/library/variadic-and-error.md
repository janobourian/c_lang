---
title: Variadic Functions & Error Handling
description: Complete ISO C89–C23 reference for <stdarg.h> (C23 single-argument va_start & parameterless f(...)), <errno.h>, <assert.h> (variadic assert & static_assert keyword), and Annex K bounds-checking constraint handlers.
icon: lucide/shield
---

# Variadic Functions & Error Handling

This reference mirrors [cppreference.com/c/variadic](https://en.cppreference.com/w/c/variadic) and [cppreference.com/c/error](https://en.cppreference.com/w/c/error), covering variable-argument traversal (`<stdarg.h>`), thread-local error reporting (`<errno.h>`), runtime and compile-time assertions (`<assert.h>`), and optional C11 Annex K bounds-checking constraint handlers.

---

## Variadic Functions: `<stdarg.h>`

**Version-test macro:** `__STDC_VERSION_STDARG_H__` (`202311L`) <span class="c-badge c23">C23</span> · **Freestanding:** Full

Variadic functions accept an unspecified number and type of trailing arguments denoted by `...` in their parameter list. Because C transmits **no runtime type or count metadata** across the ABI, the caller and callee must agree on a protocol (such as a `printf`-style format string, an explicit argument count, or a terminating sentinel like `nullptr` or `-1`).

### API Reference

| Entity | Standard | Signature / Semantics |
| :--- | :--- | :--- |
| `va_list` | <span class="c-badge c89">C89</span> | Complete object type holding the state needed by `va_start`, `va_arg`, `va_copy`, and `va_end`. |
| `va_start` | <span class="c-badge c89">C89</span> · <span class="c-badge c23">C23</span> | **C89–C17:** `void va_start(va_list ap, parmN);` — `parmN` must be the last named parameter before `...`.<br>**C23:** `void va_start(va_list ap, ...);` — **single-argument `va_start(ap)`**! Any additional arguments after `ap` are ignored and **not evaluated**, preserving source compatibility with C89–C17 while supporting parameterless `int f(...)`. |
| `va_arg` | <span class="c-badge c89">C89</span> | `T va_arg(va_list ap, T);` — Extracts the next argument of type `T` and advances `ap`. `T` must be a type such that `T *` is a valid pointer type (no raw function or array types without a `typedef`). |
| `va_copy` | <span class="c-badge c99">C99</span> | `void va_copy(va_list dest, va_list src);` — Duplicates the current traversal state of `src` into uninitialized `dest` (essential for multi-pass formatting such as measuring size before allocating). Every `va_copy` **must** be matched by `va_end(dest)`. |
| `va_end` | <span class="c-badge c89">C89</span> | `void va_end(va_list ap);` — Cleans up `ap`. Must be called in the **same function** before returning for every `va_start` or `va_copy` invocation. Failure to call `va_end` is **Undefined Behavior**. |

### C23 Upgrade: Parameterless Variadic Functions & Single-Argument `va_start` <span class="c-badge c23">C23</span>

In C89–C17, every variadic function had to declare at least one named parameter before `...` (so `int f(...)` was a syntax error), and `va_start(ap, parmN)` failed with UB if `parmN` was declared `register`, had function/array type, or had a type affected by default argument promotions (like `char`, `short`, or `float`).

**C23 (WG14 N2975)** lifts both restrictions:

=== "C23 (Single-Argument `va_start` & `f(...)`)"

    ```c
    #include <stdarg.h>

    /* Valid in C23! No dummy named parameter required before '...' */
    int sum_until_negative(...) {
        va_list ap;
        va_start(ap); // C23 single-argument form!

        int total = 0;
        for (;;) {
            int val = va_arg(ap, int);
            if (val < 0) {
                break;
            }
            total += val;
        }
        va_end(ap);
        return total;
    }
    ```

=== "C17 (Legacy Two-Argument `va_start`)"

    ```c
    #include <stdarg.h>

    /* Pre-C23 required at least one named parameter before '...' */
    int sum_until_negative(int first, ...) {
        if (first < 0) {
            return 0;
        }
        va_list ap;
        va_start(ap, first);

        int total = first;
        for (;;) {
            int val = va_arg(ap, int);
            if (val < 0) {
                break;
            }
            total += val;
        }
        va_end(ap);
        return total;
    }
    ```

### Critical Variadic Pitfalls & ABI Traps

!!! danger "Default Argument Promotions Trap in `va_arg`"
    Arguments passed through `...` automatically undergo **default argument promotions** (§6.5.2.2p6):

    - `bool`, `char`, `signed char`, `unsigned char`, `short`, and `unsigned short` promote to **`int`** (or `unsigned int` if `int` cannot hold all values).
    - `float` promotes to **`double`**.

    Therefore, writing `va_arg(ap, char)`, `va_arg(ap, short)`, `va_arg(ap, bool)`, or `va_arg(ap, float)` is **instant Undefined Behavior**! Always fetch the promoted type and narrow afterward:
    ```c
    char  c = (char)va_arg(ap, int);      // Correct
    float f = (float)va_arg(ap, double);  // Correct
    ```

!!! warning "Passing `va_list` to Helper Functions (x86-64 SysV ABI Quirk)"
    On x86-64 Linux/macOS (System V ABI), `va_list` is a **1-element array of structs**, whereas on Windows x64 and ARM32 it is a pointer (`char *`). Passing `va_list` by value therefore behaves like pass-by-reference on Linux x64 (array decay) and pass-by-value on Windows x64!

    - After passing `ap` to a helper function that calls `va_arg(ap, ...)`, the state of `ap` in the caller is **indeterminate** — you may only call `va_end(ap)` unless you first duplicated it with `va_copy`.
    - To portably advance a caller's `va_list` inside a helper, pass **`va_list *ap_ptr`** explicitly and use `va_arg(*ap_ptr, T)`.

### Complete Compilable C23 Variadic Example

Demonstrates both C23 parameterless `int sum_positive(...)` with single-argument `va_start(ap)` and a safe two-pass `va_copy` + `vsnprintf` heap formatter:

```c
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

/* C23 parameterless variadic function: sums non-negative ints until < 0 sentinel */
static int sum_positive(...) {
    va_list ap;
    va_start(ap);

    int total = 0;
    for (;;) {
        int v = va_arg(ap, int);
        if (v < 0) {
            break;
        }
        total += v;
    }

    va_end(ap);
    return total;
}

/* Safe two-pass dynamic formatter using va_copy */
static char *format_alloc(const char *fmt, ...) {
    va_list ap1;
    va_start(ap1);

    va_list ap2;
    va_copy(ap2, ap1);

    int needed = vsnprintf(nullptr, 0, fmt, ap1);
    va_end(ap1);

    if (needed < 0) {
        va_end(ap2);
        return nullptr;
    }

    size_t buf_size = (size_t)needed + 1U;
    char *buf = malloc(buf_size);
    if (buf != nullptr) {
        (void)vsnprintf(buf, buf_size, fmt, ap2);
    }

    va_end(ap2);
    return buf;
}

int main(void) {
    int total = sum_positive(10, 20, 30, 42, -1);
    char *msg = format_alloc("C23 variadic total = %d (pi ~ %.4f)", total, (double)3.14159F);
    if (msg == nullptr) {
        fputs("Formatting failed\n", stderr);
        return EXIT_FAILURE;
    }

    puts(msg);
    free(msg);
    return EXIT_SUCCESS;
}
```

---

## Error Numbers: `<errno.h>`

**Freestanding:** Hosted · **Version-test macro:** None in C23

`<errno.h>` defines the thread-local diagnostic channel **`errno`** used by mathematical, parsing, I/O, and operating-system functions to report failure details.

### The `errno` Macro Contract

- `errno` expands to a **modifiable lvalue of type `int`** (commonly `(*__errno_location())` on glibc or `(*__error())` on macOS).
- **Thread-local since C11 (§7.5p2)**: Each thread has its own independent `errno` object, initialized to `0` at thread startup.
- **Never take `&errno` across threads** expecting it to remain valid after thread exit, and never redeclare `extern int errno;` manually (a classic pre-C99 bug that breaks thread-local implementations!).

### Standard ISO C Error Constants

ISO C mandates only three positive integer constant macros in `<errno.h>`:

| Macro | Standard | Meaning | Typical Producers |
| :--- | :--- | :--- | :--- |
| `EDOM` | <span class="c-badge c89">C89</span> | Mathematics argument out of domain of function (e.g., `sqrt(-1.0)`, `acos(2.0)`) | `<math.h>` functions |
| `EILSEQ` | <span class="c-badge c95">C95</span> | Illegal or invalid multibyte/UTF byte sequence during encoding/decoding | `mbrtowc`, `mbrtoc8`, `mbrtoc16`, `mbrtoc32`, `fputwc` |
| `ERANGE` | <span class="c-badge c89">C89</span> | Result too large (overflow) or too small (underflow) to be represented | `strtol`, `strtod`, `exp`, `pow`, `tgamma` |

### Common POSIX / System `errno` Extensions

Hosted operating systems (POSIX.1-2024, Linux, macOS, Windows UCRT) define dozens of additional `E*` macros in `<errno.h>`:

| Code | Meaning | Common Cause |
| :--- | :--- | :--- |
| `EINVAL` | Invalid argument | Out-of-range flag, invalid file descriptor mode, null pointer |
| `ENOMEM` | Out of memory | `malloc`/`calloc`/`realloc`/`mmap` allocation failure |
| `ENOENT` | No such file or directory | `fopen`/`open`/`stat` on missing path |
| `EACCES` | Permission denied | Insufficient file/directory permissions |
| `EEXIST` | File exists | Exclusive creation (`O_CREAT \| O_EXCL` or `fopen(..., "wx")`) |
| `EAGAIN` / `EWOULDBLOCK` | Resource temporarily unavailable | Non-blocking socket/pipe read or write would block |
| `EINTR` | Interrupted system call | Blocking syscall interrupted by a caught signal |
| `EPIPE` | Broken pipe | Writing to a pipe or socket whose reader has closed |
| `ETIMEDOUT` | Connection or operation timed out | Network connect/recv or timed mutex timeout |

### Golden Rule: Library Calls Never Reset `errno` to Zero

!!! danger "Always Reset `errno = 0;` Before Checking It"
    Per ISO/IEC 9899:2024 §7.5p3:

    1. **No standard library function ever sets `errno` to `0`.** A stale nonzero `errno` from an earlier failure remains set across subsequent successful calls.
    2. **Successful calls may legally set `errno` to a nonzero value** (for example, `fopen` or `printf` checking `isatty()` internally!).

    Therefore, **never** check `errno` alone to determine whether a call failed — always check the function's **return value** first, and set `errno = 0;` immediately prior only for functions whose valid return range overlaps their error sentinel (such as `strtol` returning `LONG_MAX` / `LONG_MIN` or `strtod` returning `0.0` / `HUGE_VAL`).

```c
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

static bool parse_long_strict(const char *text, long *out) {
    char *end = nullptr;
    errno = 0; // Mandatory: clear any stale errno before strtol
    long value = strtol(text, &end, 10);

    if (end == text || *end != '\0') {
        return false; // No digits parsed or trailing garbage
    }
    if (errno == ERANGE && (value == LONG_MAX || value == LONG_MIN)) {
        return false; // Overflow or underflow
    }

    *out = value;
    return true;
}
```

### Converting `errno` to Human-Readable Diagnostics

- `perror(const char *s)` (`<stdio.h>`): Prints `s: <error message>\n` to `stderr` for the current `errno`.
- `strerror(int errnum)` (`<string.h>`): Returns a pointer to a descriptive string (not guaranteed thread-safe if another thread calls `strerror` or `setlocale`; prefer POSIX `strerror_r` / `strerror_l` in multithreaded code).

---

## Diagnostics & Assertions: `<assert.h>`

**Version-test macro:** `__STDC_VERSION_ASSERT_H__` (`202311L`) <span class="c-badge c23">C23</span> · **Freestanding:** Hosted

### Runtime Assertion: `assert`

| Entity | Standard | Behavior |
| :--- | :--- | :--- |
| `assert(...)` <span class="c-badge c23">C23</span> | <span class="c-badge c89">C89</span> · <span class="c-badge c23">C23</span> | **When `NDEBUG` is NOT defined:** Evaluates the scalar expression; if it compares equal to `0` (`false`), writes a diagnostic message (expression text, `__FILE__`, `__LINE__`, and `__func__` since C99) to `stderr` and calls **`abort()`**.<br>**When `NDEBUG` IS defined:** Expands to `((void)0)` — the expression is **not evaluated at all**.<br>**C23 improvement:** Defined as a variadic macro so unparenthesized commas (e.g., `assert((struct Pair){1, 2}.x == 1)`) no longer trigger preprocessor argument-count errors! |
| `NDEBUG` | <span class="c-badge c89">C89</span> | User-defined macro (commonly passed via `-DNDEBUG` in release builds) that disables `assert`. Unlike every other standard header, `<assert.h>` has **no include guard** — you can `#define`/`#undef NDEBUG` and `#include <assert.h>` repeatedly to toggle assertions per section of a file. |

!!! danger "The `assert` Side-Effects Trap"
    Never put expressions with side effects (memory allocation, I/O, state mutations, increment operators) inside `assert(...)`. In release builds compiled with `-DNDEBUG`, the entire expression vanishes!

=== "Bad (Bug vanishes in `-DNDEBUG`)"

    ```c
    #include <assert.h>
    #include <stdlib.h>

    void broken_init(size_t n) {
        int *buf = nullptr;
        assert((buf = malloc(n * sizeof(*buf))) != nullptr); // NEVER DO THIS!
        buf[0] = 42; // Instant NULL dereference in release builds!
        free(buf);
    }
    ```

=== "Good (Check errors explicitly; assert invariants only)"

    ```c
    #include <assert.h>
    #include <stdlib.h>

    bool safe_init(size_t n, int **out) {
        assert(out != nullptr); // Pure invariant check — zero side effects
        if (n == 0) {
            return false;
        }
        int *buf = malloc(n * sizeof(*buf));
        if (buf == nullptr) {
            return false; // Real runtime error handling survives -DNDEBUG
        }
        buf[0] = 42;
        *out = buf;
        return true;
    }
    ```

### Compile-Time Assertion: `static_assert` Evolution (C11 $\to$ C23)

| Revision | Syntax | Header Required? | Message String Required? |
| :--- | :--- | :---: | :---: |
| <span class="c-badge c11">C11</span> / <span class="c-badge c17">C17</span> | `_Static_assert(expr, "msg");` or `static_assert(expr, "msg");` | `<assert.h>` needed for `static_assert` | **Yes** |
| <span class="c-badge c23">C23</span> | `static_assert(expr, "msg");` **or** `static_assert(expr);` | **No** (core keyword!) | **Optional** |

```c
#include <limits.h>
#include <stdint.h>

/* C23: core keyword — no <assert.h> needed, and message is optional */
static_assert(CHAR_BIT == 8);
static_assert(sizeof(uint64_t) == 8, "64-bit exact-width integer required");
```

---

## Bounds-Checking Interfaces: Annex K <span class="c-badge c11">C11</span> <span class="c-badge c-optional">optional</span>

C11 introduced optional **Annex K ("Bounds-checking interfaces")** defining `_s`-suffixed variants of string, I/O, and utility functions (`strcpy_s`, `memcpy_s`, `sprintf_s`, `fopen_s`, `getenv_s`, `qsort_s`, …) together with a runtime constraint handler mechanism.

### Feature Detection & Opt-In

An implementation supports Annex K if and only if it predefines **`__STDC_LIB_EXT1__`** (`201112L` in C11/C17, `202311L` in C23). To expose Annex K declarations, user code must define `__STDC_WANT_LIB_EXT1__` to `1` **before** including any standard header:

```c
#define __STDC_WANT_LIB_EXT1__ 1
#include <stdlib.h>
#include <string.h>
```

### Constraint Handler Types, Constants & Functions (`<stdlib.h>`, `<stddef.h>`, `<errno.h>`)

| Entity | Header | Description |
| :--- | :--- | :--- |
| `typedef int errno_t;` | `<errno.h>`, `<stdlib.h>`, … | Return type used by most Annex K functions (`0` on success, nonzero error code on failure or constraint violation). |
| `typedef size_t rsize_t;` | `<stddef.h>`, `<stdlib.h>`, … | Restricted size type; values greater than `RSIZE_MAX` trigger a runtime constraint violation (defending against negative-to-unsigned wraparound bugs). |
| `RSIZE_MAX` | `<stdint.h>` | Maximum valid `rsize_t` value (typically `SIZE_MAX >> 1`). |
| `typedef void (*constraint_handler_t)(const char *restrict msg, void *restrict ptr, errno_t error);` | `<stdlib.h>` | Signature of a runtime constraint handler callback invoked when a `_s` function detects a null pointer, buffer overflow, or size `> RSIZE_MAX`. |
| `constraint_handler_t set_constraint_handler_s(constraint_handler_t handler);` | `<stdlib.h>` | Installs `handler` as the process-wide constraint handler (or restores the implementation default if `handler == nullptr`) and returns the previous handler. |
| `void abort_handler_s(const char *restrict msg, void *restrict ptr, errno_t error);` | `<stdlib.h>` | Built-in handler that writes `msg` to `stderr` and calls `abort()`. |
| `void ignore_handler_s(const char *restrict msg, void *restrict ptr, errno_t error);` | `<stdlib.h>` | Built-in no-op handler allowing the failing `_s` function to simply return its nonzero `errno_t` error status. |

!!! warning "Real-World Portability Reality: Why glibc, musl & BSD Reject Annex K"
    Despite being part of ISO C11–C23, **Annex K is almost universally unimplemented on UNIX/Linux/macOS** (glibc, musl, FreeBSD, OpenBSD, and Apple libc deliberately omit `__STDC_LIB_EXT1__`). As documented in **WG14 N1967 ("Field Experience With Annex K")**:

    1. **Global mutable handler breaks composability**: `set_constraint_handler_s` mutates global process state, so Library A installing `ignore_handler_s` silently breaks Library B expecting `abort_handler_s` — and races across threads.
    2. **MSVC divergence**: Microsoft's CRT predated ISO C11 Annex K and uses subtly incompatible parameter counts/types on several `_s` functions (e.g., `strncpy_s`, `qsort_s`, `strtok_s`) and does **not** define `__STDC_LIB_EXT1__`.
    3. **Portable modern C23 alternatives**: Instead of Annex K, write portable bounds-checked C23 using `snprintf` / `vsnprintf`, C23 `<stdckdint.h>` (`ckd_add`/`ckd_mul`), C23 `memccpy` and `strndup`, explicit `(ptr, len)` slice structs, and AddressSanitizer + UndefinedBehaviorSanitizer (`-fsanitize=address,undefined`).

---

## See Also

- [Program Utilities, Signals & setjmp](program.md) — `abort`, `exit`, and `unreachable()`.
- [Byte Strings & Character Utilities](strings-byte.md) — `strerror`, `memccpy`, `strndup`, and safe string parsing.
- [Input/Output Support](io.md) — `perror`, `vsnprintf`, and formatted I/O error states.
- [Bit Manipulation & Checked Integer Arithmetic](numerics-bit-checked.md) — Portable overflow checking via `<stdckdint.h>`.
- External reference: [cppreference.com — Variadic functions](https://en.cppreference.com/w/c/variadic) · [Error handling](https://en.cppreference.com/w/c/error)
