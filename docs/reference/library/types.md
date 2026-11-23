---
title: Type Support & Limits
description: Complete ISO C89–C23 reference for <stddef.h>, <stdint.h>, <inttypes.h>, <limits.h>, <float.h>, <stdbool.h>, <stdalign.h>, and <iso646.h>.
icon: lucide/binary
---

# Type Support & Numeric Limits

ISO C separates portable type abstractions and hardware limits across eight lightweight headers (seven of which are required in freestanding C23 environments). Together they mirror [cppreference.com/c/types](https://en.cppreference.com/w/c/types), including fixed-width integers, format specifier macros, C23 bit-width constants, IEEE 754 binary/decimal floating-point properties, and `nullptr_t`.

---

## Common Definitions: `<stddef.h>`

**Version-test macro:** `__STDC_VERSION_STDDEF_H__` (`202311L`) <span class="c-badge c23">C23</span> · **Freestanding:** Full

| Entity | Kind | Standard | Description |
| :--- | :--- | :--- | :--- |
| `size_t` | Unsigned integer typedef | <span class="c-badge c89">C89</span> | Unsigned integer type returned by `sizeof`, `_Alignof`/`alignof`, and `offsetof`; guaranteed at least 16 bits (`SIZE_MAX >= 65535`). |
| `ptrdiff_t` | Signed integer typedef | <span class="c-badge c89">C89</span> | Signed integer type resulting from subtracting two pointers into the same array (`PTRDIFF_MIN <= -65535`, `PTRDIFF_MAX >= 65535`). |
| `wchar_t` | Integer typedef | <span class="c-badge c89">C89</span> | Wide-character type capable of representing all members of the largest extended character set among supported locales (16-bit UTF-16 on Windows, 32-bit UTF-32 on Linux/macOS). |
| `max_align_t` | Object typedef | <span class="c-badge c11">C11</span> | Object type whose alignment requirement is at least as great as that of every scalar type in every context (`alignof(max_align_t)` is the largest *fundamental alignment*, typically 16 bytes on x86-64/ARM64). |
| `nullptr_t` | Type alias | <span class="c-badge c23">C23</span> | Defined as `typedef typeof(nullptr) nullptr_t;`. Distinct type of the `nullptr` constant; implicitly converts to any pointer type and `bool` (`false`), but **not** to integer types. |
| `rsize_t` | Unsigned integer typedef | <span class="c-badge c11">C11</span> <span class="c-badge c-optional">optional</span> | Annex K bounds-checked size type (alias for `size_t`, validated against `RSIZE_MAX`). Only exposed when `__STDC_WANT_LIB_EXT1__` is `1`. |
| `NULL` | Macro constant | <span class="c-badge c89">C89</span> | Implementation-defined null pointer constant (typically `((void *)0)`, `0`, or `nullptr` in C23). |
| `offsetof(type, member)` | Macro | <span class="c-badge c89">C89</span> | Expands to an integer constant expression of type `size_t` giving the byte offset of `member` (which may include sub-members like `a.b[2]`) from the start of struct/union `type`. |
| `unreachable()` | Function-like macro | <span class="c-badge c23">C23</span> | Marks a control-flow path as impossible to reach; executing it invokes **Undefined Behavior**, enabling aggressive optimizer dead-code elimination. See [Program Utilities](program.md#unreachable-control-flow-unreachable-c23). |

### `nullptr` / `nullptr_t` vs. `NULL` <span class="c-badge c23">C23</span>

Before C23, `NULL` could legally expand to an integer constant `0` or `0L` rather than `(void *)0`, causing subtle bugs in variadic functions (`execl("/bin/ls", "ls", NULL)` passing a 32-bit `int` where a 64-bit pointer was expected) and `_Generic` dispatch.

=== "C23 (`nullptr` & `nullptr_t`)"

    ```c
    #include <stddef.h>
    #include <stdio.h>

    #define TYPE_NAME(x) _Generic((x), \
        nullptr_t: "nullptr_t",        \
        void *:    "void *",           \
        int:       "int",              \
        default:   "other"             \
    )

    void inspect_null(void) {
        nullptr_t np = nullptr;
        void *vp = np;        // Valid: implicit conversion to any pointer type
        bool ok = !np;        // Valid: converts to false in boolean context
        // int x = np;        // Compile error! Cannot accidentally convert to int
        printf("np=%s, vp=%s, ok=%d\n", TYPE_NAME(np), TYPE_NAME(vp), (int)ok);
    }
    ```

=== "C17 (`NULL` pitfalls)"

    ```c
    #include <stddef.h>

    void legacy_null_pitfall(void) {
        /* If NULL is defined as 0, this compiles silently — a bug! */
        int x = NULL;
        (void)x;
        /* In variadic functions, you had to cast NULL explicitly: (void *)NULL */
    }
    ```

### `offsetof` Constraints & Flexible Array Member Idiom

- `member` **must not** be a bit-field (constraint violation / UB).
- Defining a new struct type inside `offsetof` (e.g., `offsetof(struct { int x; }, x)`) is **Undefined Behavior** in ISO C (§7.21p4).
- **Flexible array members (FAMs)**: Always compute allocation size using `offsetof(struct Packet, payload) + payload_len` rather than `sizeof(struct Packet) + payload_len`, because `sizeof` may include trailing struct padding that overlaps the start of the flexible array member!

---

## Fixed-Width Integer Types: `<stdint.h>`

**Version-test macro:** `__STDC_VERSION_STDINT_H__` (`202311L`) <span class="c-badge c23">C23</span> · **Freestanding:** Full

C's built-in integer types (`int`, `long`) vary across data models (LP64 vs. LLP64 vs. ILP32). `<stdint.h>` provides width-guaranteed integer typedefs, limits, constant macros, and—starting in C23—exact bit-width macros.

### Integer Type Categories

| Category | Signed Types | Unsigned Types | Guarantees & Availability |
| :--- | :--- | :--- | :--- |
| **Exact-width** | `int8_t`, `int16_t`, `int32_t`, `int64_t`, `intN_t` | `uint8_t`, `uint16_t`, `uint32_t`, `uint64_t`, `uintN_t` | Optional (mandatory if implementation has a two's-complement integer type of exact width $N$ with **no padding bits**). In C23, two's complement is mandatory for all signed integers, and `intN_t`/`uintN_t` may also exist for extended widths (e.g., `int128_t`). |
| **Minimum-width** | `int_least8_t`, `int_least16_t`, `int_least32_t`, `int_least64_t` | `uint_least8_t`, `uint_least16_t`, `uint_least32_t`, `uint_least64_t` | **Always required** for $N \in \{8, 16, 32, 64\}$. Smallest integer type with width $\ge N$. |
| **Fastest minimum-width** | `int_fast8_t`, `int_fast16_t`, `int_fast32_t`, `int_fast64_t` | `uint_fast8_t`, `uint_fast16_t`, `uint_fast32_t`, `uint_fast64_t` | **Always required** for $N \in \{8, 16, 32, 64\}$. Usually fastest integer type with width $\ge N$ (beware ABI differences: `int_fast16_t` is 64-bit on glibc x86-64, 16-bit on macOS/MSVC!). |
| **Pointer-holding** | `intptr_t` | `uintptr_t` | Optional. Capable of holding any valid `void *` converted to integer and converted back yielding an equal pointer. |
| **Greatest-width** | `intmax_t` | `uintmax_t` | **Always required**. Widest *standard* signed/unsigned integer type. **C23 change (N2888):** `_BitInt(N)` and extended integer types may now be wider than `intmax_t`/`uintmax_t` without breaking ABI. |

### Limit, Width <span class="c-badge c23">C23</span>, and Constant Macros in `<stdint.h>`

Every typedef in `<stdint.h>` (plus standard library types `ptrdiff_t`, `sig_atomic_t`, `size_t`, `wchar_t`, `wint_t`) has matching min/max limit macros and **C23 `*_WIDTH` macros** expressing the exact bit width (value bits + sign bit, excluding padding bits).

| Type Family | Min Macro | Max Macro | C23 Width Macro <span class="c-badge c23">C23</span> | Constant Macro |
| :--- | :--- | :--- | :--- | :--- |
| `intN_t` | `INTN_MIN` ($-2^{N-1}$) | `INTN_MAX` ($2^{N-1}-1$) | `INTN_WIDTH` ($N$) | `INTN_C(val)` |
| `uintN_t` | `0` | `UINTN_MAX` ($2^N-1$) | `UINTN_WIDTH` ($N$) | `UINTN_C(val)` |
| `int_leastN_t` | `INT_LEASTN_MIN` | `INT_LEASTN_MAX` | `INT_LEASTN_WIDTH` | `INTN_C(val)` |
| `uint_leastN_t` | `0` | `UINT_LEASTN_MAX` | `UINT_LEASTN_WIDTH` | `UINTN_C(val)` |
| `int_fastN_t` | `INT_FASTN_MIN` | `INT_FASTN_MAX` | `INT_FASTN_WIDTH` | — |
| `uint_fastN_t` | `0` | `UINT_FASTN_MAX` | `UINT_FASTN_WIDTH` | — |
| `intptr_t` | `INTPTR_MIN` | `INTPTR_MAX` | `INTPTR_WIDTH` | — |
| `uintptr_t` | `0` | `UINTPTR_MAX` | `UINTPTR_WIDTH` | — |
| `intmax_t` | `INTMAX_MIN` | `INTMAX_MAX` | `INTMAX_WIDTH` | `INTMAX_C(val)` |
| `uintmax_t` | `0` | `UINTMAX_MAX` | `UINTMAX_WIDTH` | `UINTMAX_C(val)` |
| `ptrdiff_t` | `PTRDIFF_MIN` | `PTRDIFF_MAX` | `PTRDIFF_WIDTH` | — |
| `sig_atomic_t` | `SIG_ATOMIC_MIN` | `SIG_ATOMIC_MAX` | `SIG_ATOMIC_WIDTH` | — |
| `size_t` | `0` | `SIZE_MAX` | `SIZE_WIDTH` | — |
| `wchar_t` | `WCHAR_MIN` | `WCHAR_MAX` | `WCHAR_WIDTH` | — |
| `wint_t` | `WINT_MIN` | `WINT_MAX` | `WINT_WIDTH` | — |
| `rsize_t` (Annex K) | `0` | `RSIZE_MAX` | — | — |

!!! tip "Use `INTN_C` / `UINTN_C` for Portable Integer Literals"
    Writing `1 << 40` triggers signed integer overflow on 32-bit `int` platforms, and `1UL << 40` still overflows on Windows LLP64 where `unsigned long` is 32-bit. Use `UINT64_C(1) << 40` (which expands to the appropriate promoted `uint_least64_t` literal suffix, e.g., `1ULL` or `1UL`).

---

## Format Conversion & Greatest-Width Utilities: `<inttypes.h>`

**Version-test macro:** `__STDC_VERSION_INTTYPES_H__` (`202311L`) <span class="c-badge c23">C23</span> · **Freestanding:** Hosted

`<inttypes.h>` automatically includes `<stdint.h>` and supplies portable `printf`/`scanf` string-literal format macros plus `intmax_t` arithmetic and parsing functions.

### Complete `PRI*` and `SCN*` Format Macro Matrix

Because `int64_t` may be `long` (LP64 Linux/macOS) or `long long` (LLP64 Windows / ILP32), hardcoding `"%ld"` or `"%lld"` breaks portability and triggers `-Wformat` warnings. Concatenate the `PRI*`/`SCN*` macros adjacent to format strings (e.g., `printf("val = %" PRId64 "\n", v)`).

C23 adds binary format specifiers **`PRIb*`**, **`PRIB*`** (uppercase `0B` prefix with `#` flag), and **`SCNb*`** <span class="c-badge c23">C23</span>! Note that there is intentionally **no `SCNB*`** macro because `scanf`'s `%b` conversion accepts both `0b` and `0B` prefixes case-insensitively.

| Conversion | Exact (`N` = `8,16,32,64`) | Least (`N`) | Fast (`N`) | Max (`intmax_t`/`uintmax_t`) | Pointer (`intptr_t`/`uintptr_t`) |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **Signed decimal (`printf`)** | `PRIdN`, `PRIiN` | `PRIdLEASTN`, `PRIiLEASTN` | `PRIdFASTN`, `PRIiFASTN` | `PRIdMAX`, `PRIiMAX` | `PRIdPTR`, `PRIiPTR` |
| **Unsigned binary (`printf`)** <span class="c-badge c23">C23</span> | `PRIbN`, `PRIBN` | `PRIbLEASTN`, `PRIBLEASTN` | `PRIbFASTN`, `PRIBFASTN` | `PRIbMAX`, `PRIBMAX` | `PRIbPTR`, `PRIBPTR` |
| **Unsigned octal (`printf`)** | `PRIoN` | `PRIoLEASTN` | `PRIoFASTN` | `PRIoMAX` | `PRIoPTR` |
| **Unsigned decimal (`printf`)** | `PRIuN` | `PRIuLEASTN` | `PRIuFASTN` | `PRIuMAX` | `PRIuPTR` |
| **Unsigned hex (`printf`)** | `PRIxN`, `PRIXN` | `PRIxLEASTN`, `PRIXLEASTN` | `PRIxFASTN`, `PRIXFASTN` | `PRIxMAX`, `PRIXMAX` | `PRIxPTR`, `PRIXPTR` |
| **Signed decimal (`scanf`)** | `SCNdN`, `SCNiN` | `SCNdLEASTN`, `SCNiLEASTN` | `SCNdFASTN`, `SCNiFASTN` | `SCNdMAX`, `SCNiMAX` | `SCNdPTR`, `SCNiPTR` |
| **Unsigned binary (`scanf`)** <span class="c-badge c23">C23</span> | `SCNbN` | `SCNbLEASTN` | `SCNbFASTN` | `SCNbMAX` | `SCNbPTR` |
| **Unsigned octal (`scanf`)** | `SCNoN` | `SCNoLEASTN` | `SCNoFASTN` | `SCNoMAX` | `SCNoPTR` |
| **Unsigned decimal (`scanf`)** | `SCNuN` | `SCNuLEASTN` | `SCNuFASTN` | `SCNuMAX` | `SCNuPTR` |
| **Unsigned hex (`scanf`)** | `SCNxN` | `SCNxLEASTN` | `SCNxFASTN` | `SCNxMAX` | `SCNxPTR` |

!!! tip "C23 `%wN` and `%wfN` Length Modifiers in `<stdio.h>`"
    In C23, `<stdio.h>` also introduces native fixed-width length modifiers (`%w32d`, `%w64u`, `%w64b`, `%wf32x`) so you no longer need macro string concatenation in C23-only code! See [Input/Output Support](io.md).

### Greatest-Width Integer Functions (`<inttypes.h>`)

| Declaration | Standard | Behavior |
| :--- | :--- | :--- |
| `typedef struct { intmax_t quot; intmax_t rem; } imaxdiv_t;` | <span class="c-badge c99">C99</span> | Result structure of `imaxdiv` (member order is unspecified; access by name only). |
| `intmax_t imaxabs(intmax_t n);` | <span class="c-badge c99">C99</span> | Absolute value of `n`. **UB** if `n == INTMAX_MIN` (cannot be represented as positive `intmax_t`). |
| `imaxdiv_t imaxdiv(intmax_t numer, intmax_t denom);` | <span class="c-badge c99">C99</span> | Simultaneously computes quotient `numer / denom` and remainder `numer % denom`. **UB** if `denom == 0` or `numer == INTMAX_MIN && denom == -1`. |
| `intmax_t strtoimax(const char *restrict nptr, char **restrict endptr, int base);` | <span class="c-badge c99">C99</span> | Parses signed `intmax_t` in base `0` or `2..36` (C23 supports `0b`/`0B` in base `0` or `2`). Sets `errno = ERANGE` on overflow. |
| `uintmax_t strtoumax(const char *restrict nptr, char **restrict endptr, int base);` | <span class="c-badge c99">C99</span> | Parses unsigned `uintmax_t`; overflow returns `UINTMAX_MAX` and sets `errno = ERANGE`. |
| `intmax_t wcstoimax(const wchar_t *restrict nptr, wchar_t **restrict endptr, int base);` | <span class="c-badge c99">C99</span> | Wide-string equivalent of `strtoimax`. |
| `uintmax_t wcstoumax(const wchar_t *restrict nptr, wchar_t **restrict endptr, int base);` | <span class="c-badge c99">C99</span> | Wide-string equivalent of `strtoumax`. |

---

## Integer Numeric Limits: `<limits.h>`

**Version-test macro:** `__STDC_VERSION_LIMITS_H__` (`202311L`) <span class="c-badge c23">C23</span> · **Freestanding:** Full

`<limits.h>` exposes the implementation-defined ranges and (since C23) bit widths of every standard built-in integer type.

| Type / Property | Minimum Macro | Maximum Macro | C23 Width Macro <span class="c-badge c23">C23</span> | Minimum ISO Guarantee |
| :--- | :--- | :--- | :--- | :--- |
| Bits per byte (`char`) | — | `CHAR_BIT` | — | $\ge 8$ |
| Max bytes in multibyte char | — | `MB_LEN_MAX` | — | $\ge 1$ |
| `bool` | `false` (`0`) | `true` (`1`) | `BOOL_WIDTH` | `1` (value bit width; `sizeof(bool)` is `1` byte) |
| `char` | `CHAR_MIN` | `CHAR_MAX` | `CHAR_WIDTH` | Same range as either `signed char` or `unsigned char` |
| `signed char` | `SCHAR_MIN` | `SCHAR_MAX` | `SCHAR_WIDTH` | $\ge 8$ bits (`-128`..`127` in C23 two's complement) |
| `unsigned char` | `0` | `UCHAR_MAX` | `UCHAR_WIDTH` | $\ge 8$ bits (`0`..`255`) |
| `short` | `SHRT_MIN` | `SHRT_MAX` | `SHRT_WIDTH` | $\ge 16$ bits |
| `unsigned short` | `0` | `USHRT_MAX` | `USHRT_WIDTH` | $\ge 16$ bits |
| `int` | `INT_MIN` | `INT_MAX` | `INT_WIDTH` | $\ge 16$ bits (32-bit on LP64/LLP64/ILP32) |
| `unsigned int` | `0` | `UINT_MAX` | `UINT_WIDTH` | $\ge 16$ bits |
| `long` | `LONG_MIN` | `LONG_MAX` | `LONG_WIDTH` | $\ge 32$ bits (64-bit on LP64, 32-bit on Windows LLP64) |
| `unsigned long` | `0` | `ULONG_MAX` | `ULONG_WIDTH` | $\ge 32$ bits |
| `long long` <span class="c-badge c99">C99</span> | `LLONG_MIN` | `LLONG_MAX` | `LLONG_WIDTH` | $\ge 64$ bits |
| `unsigned long long` <span class="c-badge c99">C99</span> | `0` | `ULLONG_MAX` | `ULLONG_WIDTH` | $\ge 64$ bits |
| `_BitInt(N)` <span class="c-badge c23">C23</span> | — | `BITINT_MAXWIDTH` | — | $\ge$ `ULLONG_WIDTH` (max supported `N` for `_BitInt(N)`) |

!!! info "Why `BOOL_WIDTH` Is `1` (Not `CHAR_BIT`)"
    In ISO C23 (§6.2.6.2 & §5.2.4.2.1), the **width** of an unsigned integer type is the number of *value bits* (excluding padding bits). Because `bool` can hold only `0` or `1`, its width `BOOL_WIDTH` is `1`, even though its storage representation occupies `sizeof(bool) == 1` byte (`CHAR_BIT` bits, of which `CHAR_BIT - 1` are padding bits).

---

## Floating-Point Limits: `<float.h>`

**Version-test macro:** `__STDC_VERSION_FLOAT_H__` (`202311L`) <span class="c-badge c23">C23</span> · **Freestanding:** Full

`<float.h>` characterizes binary floating-point (`float`, `double`, `long double`) and, optionally in C23, **IEEE 754 decimal floating-point** (`_Decimal32`, `_Decimal64`, `_Decimal128`).

### General Floating-Point Environment Macros

| Macro | Standard | Values & Meaning |
| :--- | :--- | :--- |
| `FLT_RADIX` | <span class="c-badge c89">C89</span> | Radix $b$ of exponent representation (`2` on IEEE 754 binary systems). |
| `FLT_ROUNDS` | <span class="c-badge c89">C89</span> | Current rounding direction for addition (`-1` indeterminable, `0` toward zero, `1` to nearest ties-to-even, `2` toward $+\infty$, `3` toward $-\infty$, `4` <span class="c-badge c23">C23</span> to nearest ties-away-from-zero). May change dynamically via `fesetround`. |
| `FLT_EVAL_METHOD` | <span class="c-badge c99">C99</span> | Evaluation precision of intermediate expressions: `-1` indeterminable, `0` exact type precision (x86-64 SSE2/ARM64), `1` `float`/`double` evaluated as `double`, `2` all evaluated as `long double` (legacy x87 FPU). |
| `FLT_HAS_SUBNORM`, `DBL_HAS_SUBNORM`, `LDBL_HAS_SUBNORM` | <span class="c-badge c11">C11</span> | Subnormal support: `-1` indeterminable, `0` absent, `1` present. |
| `FLT_IS_IEC_60559`, `DBL_IS_IEC_60559`, `LDBL_IS_IEC_60559` | <span class="c-badge c23">C23</span> | Expands to nonzero (`1`) if the type conforms to IEC 60559 / IEEE 754 binary interchange/extended format, `0` otherwise. |
| `INFINITY` | <span class="c-badge c23">C23</span> | Moved/duplicated from `<math.h>` into `<float.h>` in C23 so freestanding code can use positive infinity (`float` constant expression). |
| `NAN` | <span class="c-badge c23">C23</span> | Moved/duplicated from `<math.h>` into `<float.h>` in C23 (defined when quiet NaNs are supported for `float`). |
| `DECIMAL_DIG` | <span class="c-badge c99">C99</span> <span class="c-badge c-deprecated">deprecated C23</span> | **Deprecated in C23.** Use the type-specific `FLT_DECIMAL_DIG`, `DBL_DECIMAL_DIG`, `LDBL_DECIMAL_DIG` instead. |
| `CR_DECIMAL_DIG` | <span class="c-badge c23">C23</span> | Minimum decimal digits needed so decimal-to-binary-to-decimal conversion rounds identically (defined when IEC 60559 binary floating-point is supported). |

### Binary Floating-Point Limits (`FLT_*`, `DBL_*`, `LDBL_*`)

| Macro Pattern (`P` = `FLT`, `DBL`, `LDBL`) | Standard | IEEE 754 `float` (binary32) | IEEE 754 `double` (binary64) | Meaning |
| :--- | :--- | :--- | :--- | :--- |
| `P_MANT_DIG` | <span class="c-badge c89">C89</span> | `24` | `53` | Number of base-`FLT_RADIX` digits in the significand |
| `P_DIG` | <span class="c-badge c89">C89</span> | `6` | `15` | Decimal digits guaranteed to survive decimal $\to$ float $\to$ decimal round-trip |
| `P_DECIMAL_DIG` | <span class="c-badge c11">C11</span> | `9` | `17` | Decimal digits needed to serialize any value without loss (float $\to$ decimal $\to$ float) |
| `P_MIN_EXP` | <span class="c-badge c89">C89</span> | `-125` | `-1021` | Minimum negative integer $e$ such that $b^{e-1}$ is normalized |
| `P_MIN_10_EXP` | <span class="c-badge c89">C89</span> | `-37` | `-307` | Minimum negative integer $e$ such that $10^e$ is normalized |
| `P_MAX_EXP` | <span class="c-badge c89">C89</span> | `128` | `1024` | Maximum integer $e$ such that $b^{e-1}$ is representable finite |
| `P_MAX_10_EXP` | <span class="c-badge c89">C89</span> | `38` | `308` | Maximum integer $e$ such that $10^e$ is representable finite |
| `P_MAX` | <span class="c-badge c89">C89</span> | `3.40282347e+38F` | `1.7976931348623157e+308` | Maximum finite floating-point value |
| `P_NORM_MAX` | <span class="c-badge c23">C23</span> | `3.40282347e+38F` | `1.7976931348623157e+308` | Maximum **normalized** finite value (differs from `P_MAX` on non-IEEE formats like IBM double-double `long double`) |
| `P_EPSILON` | <span class="c-badge c89">C89</span> | `1.19209290e-07F` | `2.2204460492503131e-16` | Difference between `1.0` and the next representable value ($b^{1-p}$) |
| `P_MIN` | <span class="c-badge c89">C89</span> | `1.17549435e-38F` | `2.2250738585072014e-308` | Minimum positive **normalized** floating-point value |
| `P_TRUE_MIN` | <span class="c-badge c11">C11</span> | `1.40129846e-45F` | `4.9406564584124654e-324` | Minimum positive **subnormal** (or normalized if no subnormals) value |
| `P_SNAN` | <span class="c-badge c23">C23</span> | Signaling NaN (`float`) | Signaling NaN (`double`) | Signaling NaN constant (defined only when signaling NaNs are supported; triggers `FE_INVALID` on arithmetic use) |

### IEEE 754 Decimal Floating-Point Limits (`DEC32_*`, `DEC64_*`, `DEC128_*`) <span class="c-badge c23">C23</span> <span class="c-badge c-optional">optional</span>

Defined in `<float.h>` only when the implementation supports optional C23 decimal floating-point types (`_Decimal32`, `_Decimal64`, `_Decimal128`, indicated by `__STDC_IEC_60559_DFP__`). Ideal for exact base-10 financial and accounting arithmetic without binary representation error (`0.1df + 0.2df == 0.3df`).

| Macro Pattern (`D` = `DEC32`, `DEC64`, `DEC128`) | `_Decimal32` | `_Decimal64` | `_Decimal128` | Meaning |
| :--- | :--- | :--- | :--- | :--- |
| `DEC_EVAL_METHOD` | — | — | — | Evaluation format (`-1` indeterminable, `0` exact type, `1` `_Decimal64`, `2` `_Decimal128`) |
| `D_MANT_DIG` | `7` | `16` | `34` | Decimal digits in the significand |
| `D_MIN_EXP` | `-94` | `-382` | `-6142` | Minimum exponent $e$ such that $10^{e-1}$ is normalized |
| `D_MAX_EXP` | `97` | `385` | `6145` | Maximum exponent $e$ such that $10^{e-1}$ is finite |
| `D_MAX` | `9.999999E96DF` | `9.999999999999999E384DD` | `9.999...9E6144DL` | Maximum finite value |
| `D_EPSILON` | `1E-6DF` | `1E-15DD` | `1E-33DL` | Difference between `1` and next value ($10^{1-p}$) |
| `D_MIN` | `1E-95DF` | `1E-383DD` | `1E-6143DL` | Minimum positive normalized value |
| `D_TRUE_MIN` | `0.000001E-95DF` | `0.000...1E-383DD` | `0.000...1E-6143DL` | Minimum positive subnormal value |
| `D_SNAN` | Signaling NaN | Signaling NaN | Signaling NaN | Decimal signaling NaN (when supported) |

---

## Obsolescent Keyword Headers & Alternative Operators

### `<stdbool.h>` <span class="c-badge c99">C99</span> <span class="c-badge c-deprecated">deprecated C23</span>

- **C99–C17**: Defined macros `bool` $\to$ `_Bool`, `true` $\to$ `1`, `false` $\to$ `0`, and `__bool_true_false_are_defined` $\to$ `1`.
- **C23**: `bool`, `true`, and `false` are **core language keywords** (see [Keywords](../language/keywords.md)). `<stdbool.h>` no longer defines `bool`, `true`, or `false` as macros — it defines **only** `__bool_true_false_are_defined` (`1`), and the entire header is marked **obsolescent** (ISO/IEC 9899:2024 §7.19 & §7.33.10). Keep `#include <stdbool.h>` only if compiling dual C17/C23 code.

### `<stdalign.h>` <span class="c-badge c11">C11</span> <span class="c-badge c-deprecated">deprecated C23</span>

- **C11–C17**: Defined `alignas` $\to$ `_Alignas`, `alignof` $\to$ `_Alignof`, `__alignas_is_defined` $\to$ `1`, and `__alignof_is_defined` $\to$ `1`.
- **C23**: `alignas` and `alignof` are **core language keywords**. Both **`__alignas_is_defined`** and **`__alignof_is_defined`** are **removed in C23** <span class="c-badge c-removed">removed C23</span>! Including `<stdalign.h>` in C23 has no effect and is obsolescent.

### `<iso646.h>` <span class="c-badge c95">C95</span>

**Freestanding:** Full · Defines 11 object-like macros providing readable or 7-bit-ASCII-safe alternative spellings for C bitwise and logical operators (in C++, these are built-in keywords; in C, you must `#include <iso646.h>`):

| Macro | Equivalent Token | Macro | Equivalent Token | Macro | Equivalent Token |
| :--- | :--- | :--- | :--- | :--- | :--- |
| `and` | `&&` | `bitor` | `\|` | `or_eq` | `\|=` |
| `and_eq` | `&=` | `compl` | `~` | `xor` | `^` |
| `bitand` | `&` | `not` | `!` | `xor_eq` | `^=` |
| `not_eq` | `!=` | `or` | `\|\|` | | |

---

## Complete Compilable C23 Example

This C23 program inspects type widths (`*_WIDTH`, `BITINT_MAXWIDTH`), fundamental and over-aligned layouts (`alignas`, `alignof(max_align_t)`), flexible array member `offsetof`, binary printing (`PRIb32`), IEEE 754 `<float.h>` constants, and `nullptr_t` type safety.

```c
#include <float.h>
#include <inttypes.h>
#include <limits.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct Packet {
    uint32_t magic;
    uint16_t flags;
    uint8_t payload[]; // Flexible array member
};

static void describe_pointer(const char *label, const void *ptr) {
    printf("  %-14s -> %s\n", label, ptr == nullptr ? "nullptr" : "non-null");
}

int main(void) {
    puts("=== C23 Integer & Width Inspection ===");
    printf("CHAR_BIT = %d, BOOL_WIDTH = %d, INT_WIDTH = %d, SIZE_WIDTH = %d\n",
           CHAR_BIT, BOOL_WIDTH, INT_WIDTH, SIZE_WIDTH);
    printf("INT64_WIDTH = %d, PTRDIFF_WIDTH = %d, BITINT_MAXWIDTH = %d\n",
           INT64_WIDTH, PTRDIFF_WIDTH, BITINT_MAXWIDTH);

    uint32_t mask = UINT32_C(0b1010'0110'0000'1111);
#if defined(__STDC_VERSION_INTTYPES_H__) && __STDC_VERSION_INTTYPES_H__ >= 202311L
    printf("mask (hex) = 0x%04" PRIX32 ", (bin) = 0b%" PRIb32 "\n", mask, mask);
#else
    printf("mask (hex) = 0x%04" PRIX32 "\n", mask);
#endif

    puts("\n=== Alignment & offsetof ===");
    alignas(64) unsigned char cache_line_buf[64] = {};
    printf("alignof(max_align_t) = %zu bytes\n", alignof(max_align_t));
    printf("alignof(cache_line_buf) = %zu bytes\n", alignof(cache_line_buf));
    printf("sizeof(struct Packet) = %zu, offsetof(payload) = %zu\n",
           sizeof(struct Packet), offsetof(struct Packet, payload));

    puts("\n=== IEEE 754 <float.h> Properties ===");
    printf("FLT_DECIMAL_DIG = %d, DBL_DECIMAL_DIG = %d, FLT_EVAL_METHOD = %d\n",
           FLT_DECIMAL_DIG, DBL_DECIMAL_DIG, FLT_EVAL_METHOD);
#if defined(__STDC_VERSION_FLOAT_H__) && __STDC_VERSION_FLOAT_H__ >= 202311L
    printf("DBL_IS_IEC_60559 = %d, DBL_NORM_MAX = %.*g, INFINITY = %f\n",
           DBL_IS_IEC_60559, DBL_DECIMAL_DIG, DBL_NORM_MAX, (double)INFINITY);
#endif

    puts("\n=== C23 nullptr_t ===");
    nullptr_t empty = nullptr;
    describe_pointer("empty", empty);
    describe_pointer("cache_line_buf", cache_line_buf);

    return EXIT_SUCCESS;
}
```

Compile with GCC 14+ or Clang 19+ in C23 mode:

```bash
gcc -std=c23 -Wall -Wextra -Wpedantic types_demo.c -o types_demo && ./types_demo
```

---

## See Also

- [Standard Library Index](index.md) — All 31 C23 headers and version-test macros.
- [Declarations, Types & `typeof`](../language/declarations.md) — Core type system, `_BitInt(N)`, and alignment specifiers.
- [Bit Manipulation & Checked Integer Arithmetic](numerics-bit-checked.md) — `<stdbit.h>` and `<stdckdint.h>` operating on fixed-width integers.
- [Input/Output Support](io.md) — C23 `%b`/`%B` and `%wN`/`%wfN` format specifiers.
- External reference: [cppreference.com — Type support](https://en.cppreference.com/w/c/types) · [Fixed width integer types](https://en.cppreference.com/w/c/types/integer) · [Numeric limits](https://en.cppreference.com/w/c/types/limits)
