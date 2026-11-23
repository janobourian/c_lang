---
title: Preprocessor & #embed
description: Complete ISO C23 reference for Translation Phase 4 preprocessing tokens, conditional inclusion (#if, #elifdef, #elifndef), macros (#, ##, __VA_ARGS__, __VA_OPT__), #include, __has_include, C23 #embed & __has_embed, #warning, #pragma & _Pragma, STDC pragmas, and all standard predefined macros.
icon: lucide/layers
---

# Preprocessor & `#embed`

The **C Preprocessor** executes during **Translation Phase 4** (ISO/IEC 9899:2024 §6.10), transforming preprocessing tokens *before* the C compiler parses types, scopes, declarations, or statements. This page mirrors [en.cppreference.com/c/preprocessor](https://en.cppreference.com/c/preprocessor) completely through **C23 (ISO/IEC 9899:2024)**.

---

## Translation Phase 4 & Preprocessing Tokens

Before preprocessing begins, Phases 1–3 have mapped characters, spliced backslash-newline pairs, replaced comments with single spaces, and tokenized the input into **preprocessing tokens** (`pp-tokens`):

| Preprocessing Token Kind | Examples | Notes |
| :--- | :--- | :--- |
| **Header name** | `<stdio.h>`, `"config.h"` | Recognized **only** inside `#include`, `#embed`, `__has_include`, and `__has_embed`. |
| **Identifier** | `main`, `_BitInt`, `MAX_LEN` | Keywords are NOT distinguished from identifiers yet (except `true`/`false` in `#if` since C23). |
| **Preprocessing number** (`pp-number`) | `42`, `3.14e-2`, `1'000'000` <span class="c-badge c23">C23</span>, `0x1p+4`, `123foo` | Much broader than C numeric literals; validated as a true number only in Phase 7. |
| **Character constant** | `'a'`, `u8'Z'` <span class="c-badge c23">C23</span>, `L'\n'` | Escapes interpreted in Phase 5 (or during `#if` evaluation). |
| **String literal** | `"hello"`, `u8"UTF-8"` | Adjacent literals are concatenated later in Phase 6. |
| **Punctuator** | `#`, `##`, `::` <span class="c-badge c23">C23</span>, `->`, `<:` | Includes digraphs; trigraphs removed in C23. |
| **Single non-whitespace char** | `@`, `$`, stray punctuation | Preserved until Phase 7 (or consumed by `#` stringification). |

### Directive Syntax Rules

- Every directive begins with `#` (or digraph `%:`) as the **first non-whitespace token on a logical line**, and ends at the next unescaped newline.
- Horizontal whitespace (spaces/tabs) is allowed both **before** and **after** `#` (e.g., indented `#  define X 1`).
- The **null directive** (`#` alone on a line) is valid and has no effect.
- Macros **cannot** generate new `#` directives (the sole exception is the `_Pragma(...)` operator).

```mermaid
flowchart LR
    TOK["Phase 3: pp-tokens"] --> DIR["Dispatch Directive\n(#if / #include / #embed / #define)"]
    DIR --> EXP["Macro Expansion\n(Arguments -> # and ## -> Replace)"]
    EXP --> RESCAN["Rescan Output\n(Self-Name Painted Non-Expandable)"]
    RESCAN --> OUT["Phase 5–7: Clean Token Stream\n(All Directives Removed)"]
```

---

## Conditional Inclusion

Conditional directives select which sections of source text survive Phase 4. Skipped branches are still tokenized into preprocessing tokens (so unmatched quotes or invalid comments inside `#if 0` can still cause errors!), while nested `#if`/`#endif` pairs are tracked properly.

| Directive | Standard | Semantics |
| :--- | :--- | :--- |
| `#if expr` | <span class="c-badge c89">C89</span> | Includes branch if constant expression `expr` evaluates to non-zero. |
| `#ifdef MACRO` | <span class="c-badge c89">C89</span> | Shorthand for `#if defined(MACRO)`. |
| `#ifndef MACRO` | <span class="c-badge c89">C89</span> | Shorthand for `#if !defined(MACRO)`. |
| `#elif expr` | <span class="c-badge c89">C89</span> | Else-if branch evaluated if all preceding branches were false. |
| `#elifdef MACRO` | <span class="c-badge c23">C23</span> | Shorthand for `#elif defined(MACRO)`. |
| `#elifndef MACRO` | <span class="c-badge c23">C23</span> | Shorthand for `#elif !defined(MACRO)`. |
| `#else` | <span class="c-badge c89">C89</span> | Fallback branch if all preceding branches were false. |
| `#endif` | <span class="c-badge c89">C89</span> | Terminates the conditional inclusion block. |

### `defined` & Constant-Expression Evaluation Rules

Inside `#if` and `#elif`:

1. `defined NAME` and `defined(NAME)` evaluate to `1` if `NAME` is currently defined as a macro (even if defined empty: `#define FOO`), and `0` otherwise.
2. **Undefined Behavior**: Generating the token `defined` as the result of expanding another macro inside `#if` or `#elif` is **undefined behavior** (ISO C23 §6.10.2p5).
3. All macros (except operands of `defined`, `__has_include`, `__has_embed`, and `__has_c_attribute`) are expanded.
4. In <span class="c-badge c23">C23</span>, `true` evaluates to `1` and `false` evaluates to `0`.
5. **Every remaining identifier is silently replaced with `0`!**
6. Integer arithmetic is evaluated using `intmax_t` / `uintmax_t` precision (since <span class="c-badge c99">C99</span>; `long`/`unsigned long` in C89). Operators like `sizeof`, type casts, floating-point constants, and `enum` constants are **not valid** in `#if`.

!!! warning "Silent Typo Hazard in `#if`"
    Because undefined identifiers become `0`, a misspelled macro like `#if ENABLE_LOGING` silently compiles as `#if 0` without any error! Compile with `-Wundef` to catch undefined identifiers in `#if` expressions.

=== "C23 (`#elifdef` & `#elifndef`)"

    ```c
    #if defined(__linux__)
        #define PLATFORM_NAME "Linux"
    #elifdef __APPLE__
        #define PLATFORM_NAME "macOS"
    #elifdef _WIN32
        #define PLATFORM_NAME "Windows"
    #elifndef PLATFORM_CUSTOM
        #define PLATFORM_NAME "POSIX/Other"
    #endif
    ```

=== "C17 (Verbose `#elif defined(...)`)"

    ```c
    #if defined(__linux__)
        #define PLATFORM_NAME "Linux"
    #elif defined(__APPLE__)
        #define PLATFORM_NAME "macOS"
    #elif defined(_WIN32)
        #define PLATFORM_NAME "Windows"
    #elif !defined(PLATFORM_CUSTOM)
        #define PLATFORM_NAME "POSIX/Other"
    #endif
    ```

---

## Macro Replacement (`#define`, `#undef`, `#`, `##`, `__VA_OPT__`)

### Object-Like & Function-Like Macros

```c
#define BUF_SIZE 4096                         // Object-like macro
#define SQUARE(x) ((x) * (x))                 // Function-like macro (NO space before '('!)
#define BAD_FUNC (x) ((x) * (x))              // WARNING: Object-like macro expanding to "(x) ((x) * (x))"
#undef BUF_SIZE                               // Undefines macro (safe no-op if not defined)
```

- **Whitespace rule**: For a function-like macro, there must be **zero whitespace** between the macro name and `(`. Adding a space turns it into an object-like macro whose replacement list starts with `(`.
- **Redefinition rule**: Redefining an already-defined macro is a constraint violation **unless** the new definition has identical parameter names (if function-like) and an identical replacement token sequence (ignoring whitespace amount).

### Stringification (`#`) & Token Pasting (`##`)

Inside a function-like macro replacement list:

- **`#param` (Stringification)**: Converts the **unexpanded** argument tokens for `param` into a string literal. Leading/trailing whitespace is trimmed, internal whitespace collapses to a single space, and `"` / `\` inside character/string literals are automatically escaped.
- **`a ## b` (Token Pasting)**: Concatenates the **unexpanded** tokens `a` and `b` into a single preprocessing token. Producing an invalid preprocessing token (e.g., `x ## +`) is **undefined behavior**!

Because both `#` and `##` suppress argument macro expansion, always use the classic **two-level indirection idiom** when you want macro arguments expanded first:

```c
#include <stdio.h>

#define VERSION_MAJOR 2
#define VERSION_MINOR 3

#define STR_IMPL(x) #x
#define STR(x)      STR_IMPL(x)          // Expands x BEFORE stringifying

#define CAT_IMPL(a, b) a##b
#define CAT(a, b)      CAT_IMPL(a, b)    // Expands a and b BEFORE pasting

int main(void) {
    int CAT(val_, VERSION_MAJOR) = 23;   // Expands to: int val_2 = 23;
    printf("STR_IMPL: %s\n", STR_IMPL(VERSION_MAJOR)); // Prints "VERSION_MAJOR"
    printf("STR:      %s\n", STR(VERSION_MAJOR));      // Prints "2"
    printf("val_2:    %d\n", val_2);
    return 0;
}
```

### Rescanning & Self-Referential Non-Recursion

After substitution, the replacement list is **rescanned** for further macro invocations. However, during rescanning, the macro currently being expanded (and any macro already active on the expansion stack) is marked **non-expandable ("painted blue")** permanently for those resulting tokens.

This prevents infinite recursion and enables two classic idioms:

1. **Wrapping a function with a macro of the same name**:
   `#define malloc(sz) debug_malloc((sz), __FILE__, __LINE__)` (and inside `debug_malloc`, calling `(malloc)(sz)` suppresses function-like macro expansion because `(` does not follow `malloc` directly!).
2. **Self-referential definitions**: `#define EPERM EPERM` lets `#ifdef EPERM` succeed while keeping enum constant `EPERM` usable!

### Variadic Macros (`__VA_ARGS__` & C23 `__VA_OPT__`)

Introduced in <span class="c-badge c99">C99</span>, variadic macros accept variable argument lists using `...` and substitute them via `__VA_ARGS__`.

Prior to C23, calling `LOG("ready")` with `#define LOG(fmt, ...) fprintf(stderr, fmt, __VA_ARGS__)` produced a stray trailing comma (`fprintf(stderr, "ready", )`), forcing non-standard GNU `##__VA_ARGS__` hacks.

**<span class="c-badge c23">C23</span> `__VA_OPT__(content)`** solves this cleanly and portably: `__VA_OPT__(content)` expands to `content` **if and only if** `__VA_ARGS__` expands to at least one preprocessing token; otherwise it expands to **nothing**.

=== "C23 Standard (`__VA_OPT__`)"

    ```c
    #include <stdio.h>

    #define LOG(level, fmt, ...) \
        fprintf(stderr, "[%s] " fmt "\n", level __VA_OPT__(,) __VA_ARGS__)

    int main(void) {
        LOG("INFO", "server started");                 // No trailing comma!
        LOG("WARN", "retrying %d/%d", 2, 5);           // Comma inserted automatically!
        return 0;
    }
    ```

=== "Pre-C23 Workaround (Non-Standard GNU Extension)"

    ```c
    #include <stdio.h>

    // Non-standard GCC/Clang extension; rejected under strict -pedantic C17!
    #define LOG_GNU(level, fmt, ...) \
        fprintf(stderr, "[%s] " fmt "\n", level, ##__VA_ARGS__)
    ```

### Defensive Macro Idioms

- **Parenthesize every parameter and the outer expression**: `#define MUL(a, b) ((a) * (b))` avoids operator precedence bugs (`MUL(1 + 2, 3)`).
- **Wrap multi-statement macros in `do { ... } while (0)`**: Ensures the macro behaves like a single statement inside unbraced `if (...) MACRO(); else ...` blocks.
- **Avoid multiple evaluation**: Prefer `static inline` functions over function-like macros whenever possible; when type-generic macros are needed in C23, capture arguments once via `auto` / `typeof` (or `_Generic` dispatching to `static inline` functions).

---

## Header Inclusion (`#include` & C23 `__has_include`)

| Form | Search Order | Typical Use |
| :--- | :--- | :--- |
| `#include <header.h>` | Searches implementation-defined system / standard include directories (`-isystem`, `-I`). | Standard library and external library headers. |
| `#include "local.h"` | Searches the current source file's directory (implementation-defined local path) **first**, then falls back to `<...>` paths. | Project-internal headers. |
| `#include MACRO` | Macro-expands `MACRO`, which must yield `<...>` or `"..."`. | Configurable platform/backend headers. |

### C23 Header Probing: `__has_include` <span class="c-badge c23">C23</span>

`__has_include(<header.h>)` and `__has_include("header.h")` may be used in `#if` / `#elif` expressions and evaluate to `1` if `#include` with that operand would find a file, or `0` otherwise.

!!! warning "Pre-C23 Portability Guard for `__has_include`"
    Do **not** write `#if defined(__has_include) && __has_include(<stdbit.h>)` if your code must compile on older C11/C17 compilers that lack `__has_include`—the preprocessor parses the entire line before short-circuiting `&&`! Use nested `#ifdef __has_include` instead:

```c
#ifdef __has_include
    #if __has_include(<stdbit.h>)
        #include <stdbit.h>
        #define HAS_STDBIT 1
    #endif
#endif
```

### Include Guards vs. `#pragma once`

=== "Portable ISO C Include Guard"

    ```c
    #ifndef MYLIB_PARSER_H
    #define MYLIB_PARSER_H

    #include <stddef.h>

    size_t parse_tokens(const char *input, size_t len);

    #endif /* MYLIB_PARSER_H */
    ```

=== "Non-Standard `#pragma once`"

    ```c
    #pragma once

    #include <stddef.h>

    size_t parse_tokens(const char *input, size_t len);
    ```

- **Standard Include Guards**: 100% ISO C portable, immune to symlink / hard-link / mounted-filesystem path aliasing, and recognized by modern compilers' include-guard optimization (which avoids even re-opening the file!).
- **`#pragma once`**: Concise and avoids macro-name collisions, supported by GCC, Clang, and MSVC, **not** part of ISO C23, and can fail if the same header exists at two distinct paths (e.g., vendored copies or symlinks).

---

## C23 Binary Resource Inclusion (`#embed` & `__has_embed`) <span class="c-badge c23">C23</span>

Added in **ISO/IEC 9899:2024 (§6.10.4)**, `#embed` directly embeds arbitrary binary or text files into C translation units as comma-separated integer constant expressions (bytes in `[0, UCHAR_MAX]`)—replacing brittle build-time `xxd -i` / Python / `objcopy` code generators and slashing compiler memory usage by orders of magnitude on large assets.

### Syntax

```c
#embed <resource-name> embed-parameter-sequence(opt)
#embed "resource-name" embed-parameter-sequence(opt)
#embed MACRO_EXPANDING_TO_HEADER_NAME_AND_PARAMS
```

### Standard `#embed` Parameters

Every standard parameter also accepts a macro-safe double-underscore spelling (`__limit__`, `__prefix__`, `__suffix__`, `__if_empty__`) in case user code defines a macro named `limit` or `prefix`:

| Parameter | Syntax | Behavior |
| :--- | :--- | :--- |
| **`limit`** | `limit(constant-expr)` | Caps the maximum number of elements emitted (e.g., `limit(16)` reads at most 16 bytes; `limit(0)` yields an empty sequence). |
| **`prefix`** | `prefix(pp-tokens)` | Emits `pp-tokens` immediately **before** the embedded bytes **only if the resource is non-empty**. |
| **`suffix`** | `suffix(pp-tokens)` | Emits `pp-tokens` immediately **after** the embedded bytes **only if the resource is non-empty**. |
| **`if_empty`** | `if_empty(pp-tokens)` | Emits `pp-tokens` **only if the resource is empty** (0 bytes or `limit(0)`). |

### Probing Resources with `__has_embed`

Inside `#if` / `#elif`, `__has_embed("file.bin" params...)` returns one of three mandatory C23 predefined macros:

| Predefined Macro | Value | Meaning |
| :--- | :---: | :--- |
| `__STDC_EMBED_NOT_FOUND__` | `0` | Resource could not be found, or an unsupported parameter was given. |
| `__STDC_EMBED_FOUND__` | `1` | Resource found and produces at least one element (non-empty). |
| `__STDC_EMBED_EMPTY__` | `2` | Resource found, valid, and produces zero elements (0 bytes or `limit(0)`). |

### Real-World C23 `#embed` Examples

```c
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

// Example 1: Guaranteed null-terminated text asset (even if file is 0 bytes!)
#if defined(__has_embed) && __has_embed("shader.glsl") != __STDC_EMBED_NOT_FOUND__
static const char shader_source[] = {
#embed "shader.glsl" suffix(, '\0') if_empty('\0')
};
#else
static const char shader_source[] = "void main() {}\n";
#endif

// Example 2: Reading only the 8-byte PNG signature with limit(8) + prefix/suffix
#if defined(__has_embed) && __has_embed("logo.png" limit(8)) == __STDC_EMBED_FOUND__
static const uint8_t png_sig[] = {
#embed "logo.png" limit(8)
};
#else
static const uint8_t png_sig[] = { 0x89, 'P', 'N', 'G', '\r', '\n', 0x1A, '\n' };
#endif

int main(void) {
    printf("Shader bytes (excl. NUL): %zu\n", sizeof(shader_source) - 1);
    printf("PNG header first 4 bytes: %02X %c%c%c\n",
           png_sig[0], png_sig[1], png_sig[2], png_sig[3]);
    return 0;
}
```

!!! tip "Why `suffix(, '\0') if_empty('\0')` Matters"
    If `shader.glsl` is non-empty, `#embed` emits `byte0, byte1, ..., byteN, '\0'`. If `shader.glsl` is empty (0 bytes), `suffix` is **not** emitted and `if_empty('\0')` emits `'\0'`. In both cases, `shader_source` is guaranteed to be a valid, non-empty, null-terminated C string!

---

## Diagnostics & Line Control (`#error`, `#warning`, `#line`)

| Directive | Standard | Effect |
| :--- | :--- | :--- |
| `#error message...` | <span class="c-badge c89">C89</span> | Emits a diagnostic containing `message...` and **halts translation** (program is ill-formed). |
| `#warning message...` | <span class="c-badge c23">C23</span> | Emits a diagnostic warning containing `message...` and **continues translation**! |
| `#line N` | <span class="c-badge c89">C89</span> | Sets the presumed line number of the **next** line to `N` (updates `__LINE__`). |
| `#line N "name"` | <span class="c-badge c89">C89</span> | Sets presumed line number to `N` and presumed filename (`__FILE__`) to `"name"`. |

```c
#if !defined(__STDC_VERSION__) || __STDC_VERSION__ < 201112L
    #error "This library requires at least C11 (ISO/IEC 9899:2011)."
#elif __STDC_VERSION__ < 202311L
    #warning "Building in pre-C23 compatibility mode; some C23 optimizations disabled."
#endif
```

`#line` is primarily emitted by parser/lexer generators (Bison, Flex, Ragel) so compiler diagnostics and debuggers point back to the original `.y` or `.l` grammar file instead of generated `.c` output.

---

## `#pragma`, `_Pragma(...)` & Standard `STDC` Pragmas

### `#pragma` & `_Pragma(...)` Operator

- **`#pragma pp-tokens`** <span class="c-badge c89">C89</span>: Implementation-defined compiler control directive. Any `#pragma` not recognized by the compiler **must be ignored** without error (ISO C23 §6.10.7p1).
- **`_Pragma("string-literal")`** <span class="c-badge c99">C99</span>: Unary preprocessing operator that can be emitted **inside macro expansions**! The compiler destringifies the literal (stripping outer quotes, unescaping `\"` -> `"` and `\\` -> `\`) and processes the result identically to `#pragma ...`.

```c
#define DO_PRAGMA(x) _Pragma(#x)
#define DISABLE_FP_CONTRACT() DO_PRAGMA(STDC FP_CONTRACT OFF)

double strict_multiply_add(double a, double b, double c) {
    DISABLE_FP_CONTRACT()
    return a * b + c; // Prevents fusing into a single FMA instruction
}
```

### Standard `STDC` Pragmas

Pragmas beginning with `STDC` are standardized by ISO C (no macro expansion occurs on the tokens after `#pragma STDC`):

| Standard Pragma | Allowed Values | Standard | Purpose |
| :--- | :--- | :--- | :--- |
| `#pragma STDC FP_CONTRACT` | `ON`, `OFF`, `DEFAULT` | <span class="c-badge c99">C99</span> | Controls whether floating-point expressions may be contracted (e.g., fusing `a * b + c` into hardware FMA with single rounding). |
| `#pragma STDC FENV_ACCESS` | `ON`, `OFF`, `DEFAULT` | <span class="c-badge c99">C99</span> | Informs the compiler that code inspects `<fenv.h>` status flags (`fetestexcept`) or modifies dynamic rounding mode (`fesetround`). |
| `#pragma STDC CX_LIMITED_RANGE` | `ON`, `OFF`, `DEFAULT` | <span class="c-badge c99">C99</span> | When `ON`, allows fast textbook complex multiplication/division/abs without overflow/underflow/NaN scaling guards. |
| `#pragma STDC FENV_ROUND` | `FE_DOWNWARD`, `FE_TONEAREST`, `FE_TONEARESTFROMZERO`, `FE_TOWARDZERO`, `FE_UPWARD`, `FE_DYNAMIC` | <span class="c-badge c23">C23</span> | Sets a **compile-time constant** binary floating-point rounding direction for the enclosing scope (faster than dynamic `fesetround`). |
| `#pragma STDC FENV_DEC_ROUND` | `FE_DEC_DOWNWARD`, `FE_DEC_TONEAREST`, `FE_DEC_TONEARESTFROMZERO`, `FE_DEC_TOWARDZERO`, `FE_DEC_UPWARD`, `FE_DEC_DYNAMIC` | <span class="c-badge c23">C23</span> | Sets a compile-time constant rounding direction for decimal floating-point (`_Decimal32`/`_Decimal64`/`_Decimal128`). |

!!! warning "Strict Placement Rule for `STDC` Pragmas"
    A `#pragma STDC ...` directive may appear **only** at file scope (outside any function) **or** inside a compound statement `{ ... }` **before any declarations or statements** in that block. Placing it after a declaration or statement inside a block is **undefined behavior**! Its effect ends at the closing `}` of the block (or next matching pragma).

---

## Complete Table of Standard Predefined Macros

Predefined macros (ISO C23 §6.10.9) must **never** be `#define`d or `#undef`ined by user code (doing so is undefined behavior).

### Mandatory Predefined Macros

| Macro | Standard | Type / Format | Description |
| :--- | :--- | :--- | :--- |
| `__FILE__` | <span class="c-badge c89">C89</span> | String literal | Presumed name of the current source file (modifiable via `#line`). |
| `__LINE__` | <span class="c-badge c89">C89</span> | Integer constant | Presumed current line number in the source file (modifiable via `#line`). |
| `__DATE__` | <span class="c-badge c89">C89</span> | `"Mmm dd yyyy"` | Date of translation (day has leading space if `< 10`, e.g., `"Nov  5 2026"`). |
| `__TIME__` | <span class="c-badge c89">C89</span> | `"hh:mm:ss"` | Time of translation (24-hour clock). |
| `__STDC__` | <span class="c-badge c89">C89</span> | `1` | Defined as `1` by a conforming ISO C implementation. |
| `__STDC_HOSTED__` | <span class="c-badge c99">C99</span> | `1` or `0` | `1` for a hosted implementation (full standard library), `0` for freestanding. |
| `__STDC_VERSION__` | <span class="c-badge c95">C95</span> | `yyyymmL` (`long`) | Identifies ISO C standard revision (see exact revision table below). |
| `__STDC_EMBED_NOT_FOUND__` | <span class="c-badge c23">C23</span> | `0` | Return value of `__has_embed` when resource is missing or unsupported. |
| `__STDC_EMBED_FOUND__` | <span class="c-badge c23">C23</span> | `1` | Return value of `__has_embed` when resource exists and is non-empty. |
| `__STDC_EMBED_EMPTY__` | <span class="c-badge c23">C23</span> | `2` | Return value of `__has_embed` when resource exists and is empty. |

#### Exact `__STDC_VERSION__` Values

| Standard Revision | Official Document | `__STDC_VERSION__` Value |
| :--- | :--- | :---: |
| <span class="c-badge c89">C89</span> / C90 | ANSI X3.159-1989 / ISO/IEC 9899:1990 | *(not defined)* |
| <span class="c-badge c95">C95</span> | ISO/IEC 9899/AMD1:1995 | `199409L` |
| <span class="c-badge c99">C99</span> | ISO/IEC 9899:1999 | `199901L` |
| <span class="c-badge c11">C11</span> | ISO/IEC 9899:2011 | `201112L` |
| <span class="c-badge c17">C17</span> / C18 | ISO/IEC 9899:2018 | `201710L` |
| <span class="c-badge c23">C23</span> | ISO/IEC 9899:2024 | `202311L` |

### Conditional & Feature-Test Predefined Macros

| Macro | Standard | Value When Defined | Meaning |
| :--- | :--- | :---: | :--- |
| `__STDC_ISO_10646__` | <span class="c-badge c99">C99</span> | `yyyymmL` | `wchar_t` values encode ISO/IEC 10646 (Unicode) scalar values as of `yyyymm`. |
| `__STDC_MB_MIGHT_NEQ_WC__` | <span class="c-badge c99">C99</span> | `1` | Basic character set members might have different code values in `char` vs `wchar_t`. |
| `__STDC_UTF_16__` | <span class="c-badge c11">C11</span> | `1` | `char16_t` uses UTF-16 encoding (**mandated `1` in C23**). |
| `__STDC_UTF_32__` | <span class="c-badge c11">C11</span> | `1` | `char32_t` uses UTF-32 encoding (**mandated `1` in C23**). |
| `__STDC_ANALYZABLE__` | <span class="c-badge c11">C11</span> | `1` | Implementation supports Annex L (Analyzability). |
| `__STDC_LIB_EXT1__` | <span class="c-badge c11">C11</span> | `yyyymmL` | Implementation supports Annex K bounds-checking interfaces (`202311L` in C23). |
| `__STDC_NO_ATOMICS__` | <span class="c-badge c11">C11</span> | `1` | Implementation does **not** support `_Atomic` or `<stdatomic.h>`. |
| `__STDC_NO_COMPLEX__` | <span class="c-badge c11">C11</span> | `1` | Implementation does **not** support `_Complex` or `<complex.h>`. |
| `__STDC_NO_THREADS__` | <span class="c-badge c11">C11</span> | `1` | Implementation does **not** support `thread_local` or `<threads.h>`. |
| `__STDC_NO_VLA__` | <span class="c-badge c11">C11</span> | `1` | Implementation does **not** support automatic-duration VLAs *(Note: in <span class="c-badge c23">C23</span>, variably modified types such as `int (*p)[n]` and VLA function parameters are mandatory even when `__STDC_NO_VLA__` is defined!)*. |
| `__STDC_IEC_60559_BFP__` | <span class="c-badge c23">C23</span> | `202311L` | Binary floating-point conforms to Annex F (IEC 60559 / IEEE 754). |
| `__STDC_IEC_60559_DFP__` | <span class="c-badge c23">C23</span> | `202311L` | Decimal floating-point (`_Decimal32`/`64`/`128`) conforms to IEC 60559. |
| `__STDC_IEC_60559_COMPLEX__` | <span class="c-badge c23">C23</span> | `202311L` | Complex arithmetic conforms to Annex G (IEC 60559). |
| `__STDC_IEC_559__` | <span class="c-badge c99">C99</span> <span class="c-badge c-deprecated">deprecated C23</span> | `1` | Legacy indicator of IEEE 754 binary floating-point; superseded by `__STDC_IEC_60559_BFP__`. |
| `__STDC_IEC_559_COMPLEX__` | <span class="c-badge c99">C99</span> <span class="c-badge c-deprecated">deprecated C23</span> | `1` | Legacy indicator of Annex G complex math; superseded by `__STDC_IEC_60559_COMPLEX__`. |

!!! info "`__func__` Is NOT a Preprocessor Macro!"
    Introduced in <span class="c-badge c99">C99</span> (ISO C23 §6.4.2.2), **`__func__`** is a **predefined local identifier**, implicitly declared at the start of every function body as:
    ```c
    static const char __func__[] = "function-name";
    ```
    Because `__func__` only exists in **Phase 7** (not Phase 4), you **cannot** concatenate it with string literals (`"in " __func__` is a syntax error!) and `#ifdef __func__` is false. Print it with `%s`: `fprintf(stderr, "%s:%d (%s)\n", __FILE__, __LINE__, __func__);`.

### Complete Diagnostic Inspection Program (C23)

```c
#include <stdio.h>

static void print_environment_report(void) {
    printf("Function: %s (defined at %s:%d)\n", __func__, __FILE__, __LINE__);
    printf("Built on: %s at %s\n", __DATE__, __TIME__);

#ifdef __STDC_VERSION__
    printf("__STDC_VERSION__: %ldL\n", __STDC_VERSION__);
#else
    printf("__STDC_VERSION__: <not defined (C89/C90)>\n");
#endif

    printf("Hosted: %d\n", __STDC_HOSTED__);

#ifdef __STDC_IEC_60559_BFP__
    printf("IEC 60559 Binary FP: %ldL\n", __STDC_IEC_60559_BFP__);
#endif
#ifdef __STDC_IEC_60559_DFP__
    printf("IEC 60559 Decimal FP: %ldL\n", __STDC_IEC_60559_DFP__);
#endif
#ifdef __STDC_NO_ATOMICS__
    printf("Atomics: unsupported\n");
#else
    printf("Atomics: supported\n");
#endif
#ifdef __STDC_NO_THREADS__
    printf("Threads: unsupported\n");
#else
    printf("Threads: supported\n");
#endif
}

int main(void) {
    print_environment_report();
    return 0;
}
```

---

## See Also

- [Basic Concepts: Translation Phases](basic-concepts.md#translation-phases) — How Phase 4 fits between Phases 1–3 and Phases 5–8
- [Keywords (C89–C23)](keywords.md) — Context-sensitive preprocessor tokens and `<iso646.h>` macros
- [Attributes (C23)](attributes.md) — Using `__has_c_attribute(...)` in `#if` directives
- [Compiler & Libc Support Matrix](../../standards/compiler-support.md) — GCC/Clang/MSVC support status for `#embed` and `__VA_OPT__`
- External: [cppreference — Preprocessor](https://en.cppreference.com/c/preprocessor)
