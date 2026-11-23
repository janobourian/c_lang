---
title: C Keywords (C89–C23)
description: Complete alphabetical reference of all 59 ISO C23 keywords (45 primary + 14 underscore-prefixed), C23 header transitions, context-sensitive preprocessor/attribute tokens, compiler extension keywords, and iso646.h alternative operators.
icon: lucide/code-2
---

# C Keywords (C89–C23)

**Keywords** are reserved identifiers with fixed syntactic and semantic meaning in the C language (ISO/IEC 9899:2024 §6.4.1). A keyword cannot be used as an ordinary identifier (variable, function, typedef, or tag name), though a macro *may* technically share its spelling in Phase 4 (redefining a keyword as a macro while including any standard header is **undefined behavior**!).

This page mirrors [en.cppreference.com/c/keyword](https://en.cppreference.com/c/keyword) completely up to **C23 (ISO/IEC 9899:2024)**.

---

## Keyword Count Across C Revisions

| Revision | New Keywords Added | Total Keywords | Notes |
| :--- | :---: | :---: | :--- |
| <span class="c-badge c89">C89</span> | 32 | **32** | Baseline ANSI C89 / ISO C90 keyword set |
| <span class="c-badge c99">C99</span> | +5 | **37** | Added `inline`, `restrict`, `_Bool`, `_Complex`, `_Imaginary` |
| <span class="c-badge c11">C11</span> | +7 | **44** | Added `_Alignas`, `_Alignof`, `_Atomic`, `_Generic`, `_Noreturn`, `_Static_assert`, `_Thread_local` |
| <span class="c-badge c17">C17</span> | +0 | **44** | No keyword additions or removals |
| <span class="c-badge c23">C23</span> | +15 | **59** | **45 primary** + **14 `_`-prefixed** keywords (5 `_`-spellings kept as obsolescent alternates; `_Noreturn` deprecated) |

---

## Master Table of All 59 ISO C23 Keywords

### Primary Keywords (45)

| Keyword | Standard | Category | Purpose & Reference |
| :--- | :--- | :--- | :--- |
| `alignas` | <span class="c-badge c23">C23</span> | Alignment specifier | Specifies custom alignment requirement for an object/member (replaces `_Alignas`). See [Declarations](declarations.md). |
| `alignof` | <span class="c-badge c23">C23</span> | Unary operator | Queries alignment requirement in bytes of a type (replaces `_Alignof`). See [Expressions](expressions.md). |
| `auto` | <span class="c-badge c89">C89</span> <span class="c-badge c23">C23</span> | Storage class / Type inference | Automatic storage duration (C89); **type inference** from initializer expression in C23 (`auto x = 42;`). See [Declarations](declarations.md). |
| `bool` | <span class="c-badge c23">C23</span> | Type specifier | Boolean integer type holding `false` (`0`) or `true` (`1`) (replaces `_Bool` / `<stdbool.h>` macro). See [Basic Concepts](basic-concepts.md#arithmetic-types). |
| `break` | <span class="c-badge c89">C89</span> | Jump statement | Exits the innermost enclosing loop (`for`, `while`, `do`) or `switch`. See [Statements](statements.md). |
| `case` | <span class="c-badge c89">C89</span> | Label | Marks an integer constant branch target inside a `switch` statement. See [Statements](statements.md). |
| `char` | <span class="c-badge c89">C89</span> | Type specifier | Character integer type of size `1` byte (`CHAR_BIT` bits); signedness is implementation-defined. See [Basic Concepts](basic-concepts.md#arithmetic-types). |
| `const` | <span class="c-badge c89">C89</span> | Type qualifier | Marks an object or pointed-to lvalue as read-only (modifying a `const`-defined object is UB). See [Declarations](declarations.md). |
| `constexpr` | <span class="c-badge c23">C23</span> | Storage-class specifier | Declares a compile-time constant object usable in constant expressions (arrays, `case`, `static_assert`). See [Declarations](declarations.md). |
| `continue` | <span class="c-badge c89">C89</span> | Jump statement | Skips the rest of the current loop body and jumps to the next iteration condition/step. See [Statements](statements.md). |
| `default` | <span class="c-badge c89">C89</span> | Label / Selector | Fallback branch in a `switch` statement, or fallback association in `_Generic` <span class="c-badge c11">C11</span>. See [Statements](statements.md) & [Expressions](expressions.md). |
| `do` | <span class="c-badge c89">C89</span> | Iteration statement | Begins a post-test `do { ... } while (cond);` loop (always executes at least once). See [Statements](statements.md). |
| `double` | <span class="c-badge c89">C89</span> | Type specifier | Double-precision real floating-point type (or `long double` / `double _Complex`). See [Basic Concepts](basic-concepts.md#arithmetic-types). |
| `else` | <span class="c-badge c89">C89</span> | Selection statement | Alternate branch of an `if` statement (binds to the nearest preceding unmatched `if`). See [Statements](statements.md). |
| `enum` | <span class="c-badge c89">C89</span> <span class="c-badge c23">C23</span> | Type specifier | Declares an enumerated integer type and named integer constants; C23 adds fixed underlying types (`enum E : uint16_t`). See [Declarations](declarations.md). |
| `extern` | <span class="c-badge c89">C89</span> | Storage-class specifier | Declares external linkage (or references an object/function defined in another translation unit). See [Functions & Linkage](functions.md). |
| `false` | <span class="c-badge c23">C23</span> | Boolean constant | Predefined boolean literal of type `bool` with value `0` (also evaluates to `0` in `#if`). See [Expressions](expressions.md). |
| `float` | <span class="c-badge c89">C89</span> | Type specifier | Single-precision real floating-point type (typically IEEE 754 `binary32`). See [Basic Concepts](basic-concepts.md#arithmetic-types). |
| `for` | <span class="c-badge c89">C89</span> | Iteration statement | Loop with init-clause (declarations allowed since C99), condition, and iteration expression. See [Statements](statements.md). |
| `goto` | <span class="c-badge c89">C89</span> | Jump statement | Unconditionally transfers control to a named label in the same function. See [Statements](statements.md). |
| `if` | <span class="c-badge c89">C89</span> | Selection statement | Conditionally executes a statement when the controlling scalar expression compares unequal to `0`. See [Statements](statements.md). |
| `inline` | <span class="c-badge c99">C99</span> | Function specifier | Hints inline expansion and alters external linkage rules for header-defined functions. See [Functions & Linkage](functions.md). |
| `int` | <span class="c-badge c89">C89</span> | Type specifier | Fundamental signed integer type (at least 16 bits; typically 32 bits). See [Basic Concepts](basic-concepts.md#arithmetic-types). |
| `long` | <span class="c-badge c89">C89</span> | Type specifier | Extended-width integer (`long`, `long long` C99) or extended floating-point (`long double`). See [Basic Concepts](basic-concepts.md#arithmetic-types). |
| `nullptr` | <span class="c-badge c23">C23</span> | Pointer constant | Type-safe null pointer constant of type `nullptr_t` (`<stddef.h>`); does **not** implicitly convert to `int`. See [Expressions](expressions.md). |
| `register` | <span class="c-badge c89">C89</span> | Storage-class specifier | Block-scope automatic variable whose address **cannot** be taken with `&` (even in C23, forbids `&x`). See [Declarations](declarations.md). |
| `restrict` | <span class="c-badge c99">C99</span> | Type qualifier | Pointer-only promise that the referenced object is not aliased through any other pointer in scope. See [Declarations](declarations.md). |
| `return` | <span class="c-badge c89">C89</span> | Jump statement | Terminates execution of the current function and optionally returns a value to the caller. See [Statements](statements.md). |
| `short` | <span class="c-badge c89">C89</span> | Type specifier | Short signed/unsigned integer type (at least 16 bits). See [Basic Concepts](basic-concepts.md#arithmetic-types). |
| `signed` | <span class="c-badge c89">C89</span> | Type specifier | Explicitly requests a signed integer type (essential for `signed char` and signed bit-fields). See [Basic Concepts](basic-concepts.md#arithmetic-types). |
| `sizeof` | <span class="c-badge c89">C89</span> | Unary operator | Yields storage size in bytes (`size_t`) of an expression or parenthesized type (runtime for VLAs). See [Expressions](expressions.md). |
| `static` | <span class="c-badge c89">C89</span> <span class="c-badge c99">C99</span> | Storage class / Array param | Internal linkage (file scope), static duration (block scope), minimum array parameter size (`int a[static 4]` C99), or static compound literals (C23). See [Declarations](declarations.md) & [Functions](functions.md). |
| `static_assert` | <span class="c-badge c23">C23</span> | Compile-time assertion | Verifies an integer constant expression at compile time (replaces `_Static_assert`; message string optional in C23). See [Declarations](declarations.md). |
| `struct` | <span class="c-badge c89">C89</span> | Type specifier | Declares a composite record type containing sequentially allocated named members. See [Declarations](declarations.md). |
| `switch` | <span class="c-badge c89">C89</span> | Selection statement | Multi-way branch dispatching on an integer controlling expression to `case` / `default` labels. See [Statements](statements.md). |
| `thread_local` | <span class="c-badge c23">C23</span> | Storage-class specifier | Declares an object with thread storage duration (one instance per thread; replaces `_Thread_local`). See [Basic Concepts](basic-concepts.md#lifetime-and-storage-duration). |
| `true` | <span class="c-badge c23">C23</span> | Boolean constant | Predefined boolean literal of type `bool` with value `1` (also evaluates to `1` in `#if`). See [Expressions](expressions.md). |
| `typedef` | <span class="c-badge c89">C89</span> | Storage-class (syntactic) | Introduces a type alias in the ordinary identifier name space. See [Declarations](declarations.md). |
| `typeof` | <span class="c-badge c23">C23</span> | Type operator | Yields the exact type (including qualifiers) of an expression or type name (unevaluated unless VM type). See [Declarations](declarations.md). |
| `typeof_unqual` | <span class="c-badge c23">C23</span> | Type operator | Like `typeof`, but strips all top-level qualifiers (`const`, `volatile`, `restrict`, `_Atomic`). See [Declarations](declarations.md). |
| `union` | <span class="c-badge c89">C89</span> | Type specifier | Declares an overlapping composite type where all members share offset `0`. See [Declarations](declarations.md). |
| `unsigned` | <span class="c-badge c89">C89</span> | Type specifier | Unsigned integer type with modulo $2^N$ wrap-around arithmetic (never overflows into UB). See [Basic Concepts](basic-concepts.md#arithmetic-types). |
| `void` | <span class="c-badge c89">C89</span> | Type specifier | Incomplete empty type: no return value, empty parameter list `int f(void)`, or generic pointer `void *`. See [Declarations](declarations.md). |
| `volatile` | <span class="c-badge c89">C89</span> | Type qualifier | Prevents compiler from caching, reordering, or eliding accesses (MMIO registers, signal/setjmp state). See [Basic Concepts](basic-concepts.md#the-as-if-rule-and-observable-behavior). |
| `while` | <span class="c-badge c89">C89</span> | Iteration statement | Pre-test loop (`while (cond) stmt`) or closing condition of `do ... while (cond);`. See [Statements](statements.md). |

### Underscore-Prefixed Keywords (14)

Introduced in C99, C11, and C23 inside the implementation-reserved `_[A-Z]*` name space to avoid breaking legacy user identifiers. Several became obsolescent alternate spellings in C23 after their clean primary spellings were promoted to keywords.

| Keyword | Standard | C23 Status | Purpose & Preferred C23 Spelling |
| :--- | :--- | :--- | :--- |
| `_Alignas` | <span class="c-badge c11">C11</span> | Alternate spelling | Alignment specifier; prefer **`alignas`** in C23. |
| `_Alignof` | <span class="c-badge c11">C11</span> | Alternate spelling | Alignment query operator; prefer **`alignof`** in C23. |
| `_Atomic` | <span class="c-badge c11">C11</span> <span class="c-badge c-optional">optional</span> | Active | Atomic type qualifier (`_Atomic int`) and type specifier (`_Atomic(int)`). Absent if `__STDC_NO_ATOMICS__` is defined. See [Concurrency](../library/concurrency.md). |
| `_BitInt` | <span class="c-badge c23">C23</span> | Active | Bit-precise integer type specifier: `_BitInt(N)` / `unsigned _BitInt(N)` (`1 <= N <= BITINT_MAXWIDTH`, `N >= 2` for signed). |
| `_Bool` | <span class="c-badge c99">C99</span> | Alternate spelling | Boolean type; prefer **`bool`** in C23. |
| `_Complex` | <span class="c-badge c99">C99</span> <span class="c-badge c-optional">optional</span> | Active | Complex floating-point type specifier (`double _Complex`; `<complex.h>` defines convenience macro `complex`). Absent if `__STDC_NO_COMPLEX__` is defined. |
| `_Decimal32` | <span class="c-badge c23">C23</span> <span class="c-badge c-optional">optional</span> | Active | 32-bit IEEE 754 decimal floating-point (`df` suffix; requires `__STDC_IEC_60559_DFP__`). |
| `_Decimal64` | <span class="c-badge c23">C23</span> <span class="c-badge c-optional">optional</span> | Active | 64-bit IEEE 754 decimal floating-point (`dd` suffix; requires `__STDC_IEC_60559_DFP__`). |
| `_Decimal128` | <span class="c-badge c23">C23</span> <span class="c-badge c-optional">optional</span> | Active | 128-bit IEEE 754 decimal floating-point (`dl` suffix; requires `__STDC_IEC_60559_DFP__`). |
| `_Generic` | <span class="c-badge c11">C11</span> | Active | Compile-time type-generic selection expression (`_Generic((x), int: f_i, default: f_d)(x)`). See [Expressions](expressions.md). |
| `_Imaginary` | <span class="c-badge c99">C99</span> <span class="c-badge c-optional">optional</span> | Active | Pure imaginary floating-point type specifier (Annex G; `<complex.h>` defines `imaginary`). |
| `_Noreturn` | <span class="c-badge c11">C11</span> | <span class="c-badge c-deprecated">deprecated C23</span> | Function specifier indicating the function never returns; superseded by **`[[noreturn]]`** in C23. See [Attributes](attributes.md). |
| `_Static_assert` | <span class="c-badge c11">C11</span> | Alternate spelling | Compile-time assertion; prefer **`static_assert`** in C23. |
| `_Thread_local` | <span class="c-badge c11">C11</span> <span class="c-badge c-optional">optional</span> | Alternate spelling | Thread storage duration; prefer **`thread_local`** in C23. Absent if `__STDC_NO_THREADS__` is defined. |

---

## C23 Keyword Transitions & Header Changes

C23 modernized C's ergonomics by promoting seven widely used standard macros into **true first-class keywords**—eliminating header-order bugs and harmonizing C with C++.

| Keyword in C23 | Pre-C23 Header & Macro | What Changed in C23 (ISO/IEC 9899:2024) |
| :--- | :--- | :--- |
| `bool`, `true`, `false` | `<stdbool.h>` defined `bool` -> `_Bool`, `true` -> `1`, `false` -> `0` | Now **built-in keywords**! `true` and `false` have type **`bool`** (not `int`!), and evaluate to `1` and `0` in `#if` directives. `<stdbool.h>` now only defines obsolescent `__bool_true_false_are_defined`. |
| `alignas`, `alignof` | `<stdalign.h>` defined `alignas` -> `_Alignas`, `alignof` -> `_Alignof` | Now **built-in keywords**! `<stdalign.h>` only retains obsolescent `__alignas_is_defined` and `__alignof_is_defined`. |
| `static_assert` | `<assert.h>` defined `static_assert` -> `_Static_assert` | Now a **built-in keyword** usable without `<assert.h>`, and the diagnostic message string is now **optional**: `static_assert(sizeof(int) >= 4);`. |
| `thread_local` | `<threads.h>` defined `thread_local` -> `_Thread_local` | Now a **built-in keyword** (when `__STDC_NO_THREADS__` is not defined). |
| `[[noreturn]]` | `<stdnoreturn.h>` defined `noreturn` -> `_Noreturn` | Both `_Noreturn` and the entire `<stdnoreturn.h>` header are **deprecated (obsolescent)** in C23 in favor of `[[noreturn]]` (or `[[_Noreturn]]`). |

=== "C23 (Zero Boilerplate Headers Needed)"

    ```c
    #include <stdio.h>
    #include <stdlib.h>

    static_assert(sizeof(void *) >= 4); // No <assert.h>, no message string required!

    [[noreturn]] static void fatal(const char *msg) {
        fprintf(stderr, "fatal: %s\n", msg);
        exit(EXIT_FAILURE);
    }

    int main(void) {
        alignas(16) unsigned char buf[16] = {};
        bool ok = true; // Built-in keyword; type of 'true' is bool (sizeof(true) == sizeof(bool))
        if (!ok) {
            fatal("unreachable");
        }
        printf("alignof(buf) = %zu, sizeof(true) = %zu\n", alignof(buf), sizeof(true));
        return 0;
    }
    ```

=== "C17 (Legacy Macro Headers Required)"

    ```c
    #include <assert.h>
    #include <stdalign.h>
    #include <stdbool.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdnoreturn.h>

    static_assert(sizeof(void *) >= 4, "pointer must be at least 32-bit");

    noreturn static void fatal(const char *msg) {
        fprintf(stderr, "fatal: %s\n", msg);
        exit(EXIT_FAILURE);
    }

    int main(void) {
        alignas(16) unsigned char buf[16] = { 0 };
        bool ok = true; // Macro expanding to 1 (type int: sizeof(true) == sizeof(int)!)
        if (!ok) {
            fatal("unreachable");
        }
        printf("alignof(buf) = %zu, sizeof(true) = %zu\n", alignof(buf), sizeof(true));
        return 0;
    }
    ```

!!! warning "C23 Migration Gotcha: Existing Code Using New Keywords as Identifiers"
    Legacy C17 code that declared variables named `bool`, `true`, `false`, `nullptr`, `constexpr`, `typeof`, `typeof_unqual`, `alignas`, `alignof`, `static_assert`, or `thread_local` will fail to compile under `-std=c23`. Also watch out for `_Generic(true, bool: ..., int: ...)`: in C17 `true` selects `int`, whereas in C23 `true` selects `bool`!

### C2y Draft Candidate Keywords <span class="c-badge c2y">C2y</span>

Active WG14 working-draft proposals for the post-C23 revision include:

- `_Countof` (with convenience macro `countof` in `<stdcountof.h>`): yields the element count (`size_t`) of an array operand without pointer-decay bugs (unlike `sizeof(a) / sizeof(a[0])`).
- `_Defer` / `defer` (under active WG14 discussion): lexical block-exit cleanup statements.

---

## Context-Sensitive & Preprocessor Tokens

The identifiers below are **not** core C language keywords (so they remain legal variable names in Phase 7—unless otherwise reserved by `_` rules), but carry special meaning inside preprocessing directives, `#embed`, attributes, or `#pragma STDC`:

### Preprocessor Directive Names & Operators

| Category | Tokens | Notes |
| :--- | :--- | :--- |
| **Conditional directives** | `if`, `elif`, `else`, `endif`, `ifdef`, `ifndef`, `elifdef` <span class="c-badge c23">C23</span>, `elifndef` <span class="c-badge c23">C23</span> | Recognized immediately after `#` at start of logical line. |
| **Macro & inclusion directives** | `define`, `undef`, `include`, `embed` <span class="c-badge c23">C23</span> | See [Preprocessor](preprocessor.md). |
| **Diagnostic & control directives** | `line`, `error`, `warning` <span class="c-badge c23">C23</span>, `pragma` | `#warning` standardized in C23. |
| **Preprocessor operators** | `defined`, `_Pragma` <span class="c-badge c99">C99</span>, `__has_include` <span class="c-badge c23">C23</span>, `__has_embed` <span class="c-badge c23">C23</span>, `__has_c_attribute` <span class="c-badge c23">C23</span> | Evaluated in Phase 4 (`defined`, `__has_*` inside `#if`/`#elif`). |
| **`#embed` parameters** <span class="c-badge c23">C23</span> | `limit`, `prefix`, `suffix`, `if_empty` (and `__limit__`, etc.) | Recognized only in `#embed` and `__has_embed`. |

### Standard Attribute & `STDC` Pragma Tokens

| Category | Tokens | Standard |
| :--- | :--- | :--- |
| **C23 Standard Attributes** (inside `[[...]]`) | `deprecated`, `fallthrough`, `maybe_unused`, `nodiscard`, `noreturn`, `_Noreturn`, `reproducible`, `unsequenced` (plus `__name__` macro-safe forms) | <span class="c-badge c23">C23</span> See [Attributes](attributes.md). |
| **Standard `#pragma STDC` tokens** | `STDC`, `FP_CONTRACT`, `FENV_ACCESS`, `CX_LIMITED_RANGE`, `FENV_ROUND` <span class="c-badge c23">C23</span>, `FENV_DEC_ROUND` <span class="c-badge c23">C23</span>, `ON`, `OFF`, `DEFAULT` | <span class="c-badge c99">C99</span> / <span class="c-badge c23">C23</span> See [Preprocessor](preprocessor.md#standard-stdc-pragmas). |

---

## Common Non-Standard Extension Keywords

ISO C Annex J.5.10 explicitly mentions common extension keywords supported by major toolchains (GCC, Clang, MSVC, ICC). In strict ISO mode (`-std=c23 -pedantic`), unprefixed extensions like `asm` or `typeof` (pre-C23) may be disabled in favor of their double-underscore spellings:

| Extension Keyword | Compilers | Purpose & Standard Equivalent |
| :--- | :--- | :--- |
| `asm` / `__asm__` | GCC, Clang, MSVC (`__asm`) | Embeds inline target assembly instructions (ISO C Annex J.5.10). |
| `fortran` | Historical / Annex J.5.9 | Declares external functions following Fortran calling conventions. |
| `__typeof__` | GCC, Clang | Pre-C23 extension for `typeof` (use standard **`typeof`** / **`typeof_unqual`** in C23). |
| `__attribute__` | GCC, Clang | Compiler-specific attributes (`__attribute__((packed))`); prefer standard C23 `[[...]]` / `[[gnu::...]]` where applicable. |
| `__extension__` | GCC, Clang | Silences `-Wpedantic` warnings for a GNU C extension expression or declaration. |
| `__inline__`, `__restrict__` | GCC, Clang | C89-compatible spellings for C99 `inline` and `restrict`. |
| `__declspec(...)` | MSVC, Clang (Windows) | Storage-class extensions such as `__declspec(dllexport)` / `__declspec(dllimport)`. |
| `__int128` / `unsigned __int128` | GCC, Clang (64-bit targets) | 128-bit integer extension; in C23, consider portable `_BitInt(128)` / `unsigned _BitInt(128)`. |

---

## Alternative Operator Spellings (`<iso646.h>`)

Added in <span class="c-badge c95">C95</span> to assist developers on keyboards lacking certain ASCII punctuation characters, `<iso646.h>` defines **11 object-like macros** for logical and bitwise operators:

| Macro (`<iso646.h>`) | Expands To | Category |
| :---: | :---: | :--- |
| `and` | `&&` | Logical AND |
| `and_eq` | `&=` | Bitwise AND assignment |
| `bitand` | `&` | Bitwise AND |
| `bitor` | <code>&#124;</code> | Bitwise OR |
| `compl` | `~` | Bitwise NOT (ones' complement) |
| `not` | `!` | Logical NOT |
| `not_eq` | `!=` | Inequality |
| `or` | <code>&#124;&#124;</code> | Logical OR |
| `or_eq` | <code>&#124;=</code> | Bitwise OR assignment |
| `xor` | `^` | Bitwise XOR |
| `xor_eq` | `^=` | Bitwise XOR assignment |

!!! note "C vs. C++ Difference for `<iso646.h>`"
    In **C++**, `and`, `or`, `not`, `bitand`, etc. are built-in alternative tokens available without any header (and `<ciso646>` was removed in C++20). In **ISO C** (including C23), they remain **preprocessor macros** defined only when you `#include <iso646.h>`.

```c
#include <iso646.h>
#include <stdio.h>

int main(void) {
    unsigned mask = 0x0Fu;
    unsigned flags = 0x35u;
    if ((flags bitand mask) not_eq 0u and not (flags == 0u)) {
        flags xor_eq 0xFFu;
        printf("Updated flags: 0x%02X\n", flags);
    }
    return 0;
}
```

---

## See Also

- [Basic Concepts & Memory Model](basic-concepts.md) — Reserved identifier rules, punctuators, and digraphs
- [Preprocessor & `#embed`](preprocessor.md) — Directives, `__VA_OPT__`, `#embed`, and predefined macros
- [Declarations, Types & `typeof`](declarations.md) — Deep dive into `auto`, `constexpr`, `typeof`, `typeof_unqual`, `alignas`, and `static_assert`
- [Attributes (C23)](attributes.md) — Complete reference for `[[...]]` standard attributes replacing `_Noreturn`
- [C23 Migration & Breaking Changes](../../standards/c23-migration.md) — Updating legacy code for new C23 keywords
- External: [cppreference — C Keywords](https://en.cppreference.com/c/keyword)
