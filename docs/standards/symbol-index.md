---
title: Master C23 Symbol Index & Non-ISO Ecosystem
description: Searchable master index of all ISO C89–C23 keywords, preprocessor directives, types, macros, and standard library functions, plus a curated 2026 directory of production non-ISO C libraries.
icon: lucide/library
---

# Master C23 Symbol Index & Non-ISO Ecosystem

Use the site's instant search bar (press `/` or `Ctrl+K`) to jump directly to any ISO C89–C23 keyword, type, macro, or library function indexed below. Every symbol links to its corresponding local deep-dive page in this documentation set.

This page mirrors [cppreference.com — C Symbol Index](https://en.cppreference.com/w/c/symbol_index) and [cppreference.com — Non-ISO Libraries](https://en.cppreference.com/w/c/links/libs) (modernized for 2026 systems engineering).

---

## 1. Core Language Keywords & Preprocessor Tokens

| Symbol | Header | Since | Category | Reference Link |
| :--- | :--- | :--- | :--- | :--- |
| `alignas` / `_Alignas` | *(keyword)* | <span class="c-badge c23">C23</span> / <span class="c-badge c11">C11</span> | Alignment specifier | [Keywords](../reference/language/keywords.md) |
| `alignof` / `_Alignof` | *(keyword)* | <span class="c-badge c23">C23</span> / <span class="c-badge c11">C11</span> | Alignment operator | [Expressions](../reference/language/expressions.md) |
| `_Atomic` | *(keyword)* | <span class="c-badge c11">C11</span> | Atomic qualifier/specifier | [Concurrency](../reference/library/concurrency.md) |
| `auto` | *(keyword)* | <span class="c-badge c89">C89</span> / <span class="c-badge c23">C23</span> | Type inference (C23) / storage class | [Declarations](../reference/language/declarations.md) |
| `_BitInt` | *(keyword)* | <span class="c-badge c23">C23</span> | Bit-precise integer type | [Declarations](../reference/language/declarations.md) |
| `bool` / `_Bool` | *(keyword)* | <span class="c-badge c23">C23</span> / <span class="c-badge c99">C99</span> | Boolean type | [Keywords](../reference/language/keywords.md) |
| `break` | *(keyword)* | <span class="c-badge c89">C89</span> | Jump statement | [Statements](../reference/language/statements.md) |
| `case` | *(keyword)* | <span class="c-badge c89">C89</span> | Switch label | [Statements](../reference/language/statements.md) |
| `char` | *(keyword)* | <span class="c-badge c89">C89</span> | Character type | [Declarations](../reference/language/declarations.md) |
| `_Complex` | *(keyword)* | <span class="c-badge c99">C99</span> | Complex number specifier | [Math & Complex](../reference/library/numerics-math.md) |
| `const` | *(keyword)* | <span class="c-badge c89">C89</span> | Type qualifier | [Declarations](../reference/language/declarations.md) |
| `constexpr` | *(keyword)* | <span class="c-badge c23">C23</span> | Constant-expression object | [Declarations](../reference/language/declarations.md) |
| `continue` | *(keyword)* | <span class="c-badge c89">C89</span> | Loop continuation jump | [Statements](../reference/language/statements.md) |
| `_Decimal32` / `_Decimal64` / `_Decimal128` | *(keyword)* | <span class="c-badge c23">C23</span> <span class="c-badge c-optional">optional</span> | IEEE 754 decimal FP types | [Declarations](../reference/language/declarations.md) |
| `default` | *(keyword)* | <span class="c-badge c89">C89</span> | Switch fallback label | [Statements](../reference/language/statements.md) |
| `do` | *(keyword)* | <span class="c-badge c89">C89</span> | Iteration statement | [Statements](../reference/language/statements.md) |
| `double` | *(keyword)* | <span class="c-badge c89">C89</span> | Double-precision float | [Declarations](../reference/language/declarations.md) |
| `else` | *(keyword)* | <span class="c-badge c89">C89</span> | Conditional branch | [Statements](../reference/language/statements.md) |
| `enum` | *(keyword)* | <span class="c-badge c89">C89</span> / <span class="c-badge c23">C23</span> | Enumeration (`enum : T` in C23) | [Declarations](../reference/language/declarations.md) |
| `extern` | *(keyword)* | <span class="c-badge c89">C89</span> | External linkage specifier | [Functions & Linkage](../reference/language/functions.md) |
| `false` / `true` | *(keyword)* | <span class="c-badge c23">C23</span> | Boolean constants | [Keywords](../reference/language/keywords.md) |
| `float` | *(keyword)* | <span class="c-badge c89">C89</span> | Single-precision float | [Declarations](../reference/language/declarations.md) |
| `for` | *(keyword)* | <span class="c-badge c89">C89</span> | Iteration statement | [Statements](../reference/language/statements.md) |
| `_Generic` | *(keyword)* | <span class="c-badge c11">C11</span> | Type-generic selection | [Expressions](../reference/language/expressions.md) |
| `goto` | *(keyword)* | <span class="c-badge c89">C89</span> | Unconditional jump | [Statements](../reference/language/statements.md) |
| `if` | *(keyword)* | <span class="c-badge c89">C89</span> | Selection statement | [Statements](../reference/language/statements.md) |
| `_Imaginary` | *(keyword)* | <span class="c-badge c99">C99</span> <span class="c-badge c-optional">optional</span> | Imaginary number specifier | [Math & Complex](../reference/library/numerics-math.md) |
| `inline` | *(keyword)* | <span class="c-badge c99">C99</span> | Function inline specifier | [Functions & Linkage](../reference/language/functions.md) |
| `int` / `short` / `long` | *(keyword)* | <span class="c-badge c89">C89</span> / <span class="c-badge c99">C99</span> | Integer type specifiers | [Declarations](../reference/language/declarations.md) |
| `_Noreturn` | *(keyword)* | <span class="c-badge c11">C11</span> <span class="c-badge c-deprecated">deprecated C23</span> | Non-returning function | [Attributes](../reference/language/attributes.md) |
| `nullptr` | *(keyword)* | <span class="c-badge c23">C23</span> | Typed null pointer literal | [Expressions](../reference/language/expressions.md) |
| `register` | *(keyword)* | <span class="c-badge c89">C89</span> | Storage-class specifier | [Declarations](../reference/language/declarations.md) |
| `restrict` | *(keyword)* | <span class="c-badge c99">C99</span> | Non-aliasing pointer qualifier | [Declarations](../reference/language/declarations.md) |
| `return` | *(keyword)* | <span class="c-badge c89">C89</span> | Function return statement | [Statements](../reference/language/statements.md) |
| `signed` / `unsigned` | *(keyword)* | <span class="c-badge c89">C89</span> | Integer signedness | [Declarations](../reference/language/declarations.md) |
| `sizeof` | *(keyword)* | <span class="c-badge c89">C89</span> | Object/type byte-size operator | [Expressions](../reference/language/expressions.md) |
| `static` | *(keyword)* | <span class="c-badge c89">C89</span> | Storage duration / internal linkage | [Functions & Linkage](../reference/language/functions.md) |
| `static_assert` / `_Static_assert` | *(keyword)* | <span class="c-badge c23">C23</span> / <span class="c-badge c11">C11</span> | Compile-time assertion | [Declarations](../reference/language/declarations.md) |
| `struct` / `union` | *(keyword)* | <span class="c-badge c89">C89</span> | Aggregate / overlapping types | [Declarations](../reference/language/declarations.md) |
| `switch` | *(keyword)* | <span class="c-badge c89">C89</span> | Multi-way selection | [Statements](../reference/language/statements.md) |
| `thread_local` / `_Thread_local` | *(keyword)* | <span class="c-badge c23">C23</span> / <span class="c-badge c11">C11</span> | Thread storage duration | [Concurrency](../reference/library/concurrency.md) |
| `typedef` | *(keyword)* | <span class="c-badge c89">C89</span> | Type alias declaration | [Declarations](../reference/language/declarations.md) |
| `typeof` / `typeof_unqual` | *(keyword)* | <span class="c-badge c23">C23</span> | Compile-time type query | [Declarations](../reference/language/declarations.md) |
| `void` | *(keyword)* | <span class="c-badge c89">C89</span> | Empty / generic pointer type | [Declarations](../reference/language/declarations.md) |
| `volatile` | *(keyword)* | <span class="c-badge c89">C89</span> | Hardware/signal access qualifier | [Declarations](../reference/language/declarations.md) |
| `while` | *(keyword)* | <span class="c-badge c89">C89</span> | Iteration statement | [Statements](../reference/language/statements.md) |
| `[[deprecated]]`, `[[fallthrough]]`, `[[maybe_unused]]`, `[[nodiscard]]`, `[[noreturn]]`, `[[reproducible]]`, `[[unsequenced]]` | *(attribute)* | <span class="c-badge c23">C23</span> | Standard C23 attributes | [Attributes (C23)](../reference/language/attributes.md) |
| `#embed`, `#elifdef`, `#elifndef`, `#warning`, `__VA_OPT__`, `__has_include`, `__has_embed`, `__has_c_attribute` | *(preprocessor)* | <span class="c-badge c23">C23</span> | Preprocessor directives & operators | [Preprocessor & `#embed`](../reference/language/preprocessor.md) |
| `__func__`, `__FILE__`, `__LINE__`, `__DATE__`, `__TIME__`, `__STDC__`, `__STDC_VERSION__`, `__STDC_HOSTED__` | *(predefined)* | <span class="c-badge c89">C89</span>–<span class="c-badge c99">C99</span> | Predefined identifiers & macros | [Preprocessor & `#embed`](../reference/language/preprocessor.md) |

---

## 2. Type Support, Fixed-Width Integers & Limits

| Symbol | Header | Since | Category | Reference Link |
| :--- | :--- | :--- | :--- | :--- |
| `size_t`, `ptrdiff_t`, `wchar_t`, `NULL`, `offsetof` | `<stddef.h>` | <span class="c-badge c89">C89</span> | Core types & macros | [Type Support & Limits](../reference/library/types.md) |
| `max_align_t` | `<stddef.h>` | <span class="c-badge c11">C11</span> | Fundamental alignment type | [Type Support & Limits](../reference/library/types.md) |
| `nullptr_t`, `unreachable` | `<stddef.h>` | <span class="c-badge c23">C23</span> | Null type / optimizer hint | [Type Support & Limits](../reference/library/types.md) |
| `int8_t`, `int16_t`, `int32_t`, `int64_t`, `uint8_t`, `uint16_t`, `uint32_t`, `uint64_t` | `<stdint.h>` | <span class="c-badge c99">C99</span> | Exact-width integer types | [Type Support & Limits](../reference/library/types.md) |
| `int_leastN_t`, `uint_leastN_t`, `int_fastN_t`, `uint_fastN_t` | `<stdint.h>` | <span class="c-badge c99">C99</span> | Minimum / fastest integer types | [Type Support & Limits](../reference/library/types.md) |
| `intptr_t`, `uintptr_t`, `intmax_t`, `uintmax_t`, `INT64_C`, `UINT64_C` | `<stdint.h>` | <span class="c-badge c99">C99</span> | Pointer-width & max integers | [Type Support & Limits](../reference/library/types.md) |
| `INTN_WIDTH`, `UINTN_WIDTH`, `SIZE_WIDTH`, `INTPTR_WIDTH` | `<stdint.h>` | <span class="c-badge c23">C23</span> | Bit-width limit macros | [Type Support & Limits](../reference/library/types.md) |
| `PRIdN`, `PRIuN`, `PRIxN`, `SCNdN`, `SCNuN`, `imaxabs`, `imaxdiv`, `strtoimax`, `strtoumax` | `<inttypes.h>` | <span class="c-badge c99">C99</span> | Format macros & `intmax_t` ops | [Type Support & Limits](../reference/library/types.md) |
| `PRIbN`, `PRIBN`, `SCNbN` | `<inttypes.h>` | <span class="c-badge c23">C23</span> | Binary format macros | [Type Support & Limits](../reference/library/types.md) |
| `CHAR_BIT`, `SCHAR_MIN`, `SCHAR_MAX`, `UCHAR_MAX`, `INT_MIN`, `INT_MAX`, `UINT_MAX`, `LONG_MAX`, `LLONG_MAX` | `<limits.h>` | <span class="c-badge c89">C89</span> / <span class="c-badge c99">C99</span> | Integer limits | [Type Support & Limits](../reference/library/types.md) |
| `BOOL_WIDTH`, `INT_WIDTH`, `LONG_WIDTH`, `LLONG_WIDTH`, `BITINT_MAXWIDTH` | `<limits.h>` | <span class="c-badge c23">C23</span> | Integer width macros | [Type Support & Limits](../reference/library/types.md) |
| `FLT_RADIX`, `FLT_MANT_DIG`, `FLT_EPSILON`, `FLT_MIN`, `FLT_MAX`, `DBL_EPSILON`, `DBL_MAX`, `LDBL_MAX` | `<float.h>` | <span class="c-badge c89">C89</span> | Floating-point limits | [Type Support & Limits](../reference/library/types.md) |
| `FLT_DECIMAL_DIG`, `DBL_DECIMAL_DIG`, `LDBL_DECIMAL_DIG`, `FLT_TRUE_MIN`, `DBL_TRUE_MIN` | `<float.h>` | <span class="c-badge c11">C11</span> | Exact decimal digits & subnormals | [Type Support & Limits](../reference/library/types.md) |
| `INFINITY`, `NAN`, `DEC_INFINITY`, `DEC_NAN`, `FLT_NORM_MAX`, `DBL_NORM_MAX` | `<float.h>` | <span class="c-badge c23">C23</span> | FP special values (moved from `<math.h>`) | [Type Support & Limits](../reference/library/types.md) |
| `and`, `or`, `not`, `bitand`, `bitor`, `xor`, `compl`, `not_eq` | `<iso646.h>` | <span class="c-badge c95">C95</span> | Alternative operator macros | [Type Support & Limits](../reference/library/types.md) |

---

## 3. Program Utilities, Error Handling, Signals & Variadics

| Symbol | Header | Since | Category | Reference Link |
| :--- | :--- | :--- | :--- | :--- |
| `abort`, `exit`, `atexit`, `getenv`, `system`, `EXIT_SUCCESS`, `EXIT_FAILURE` | `<stdlib.h>` | <span class="c-badge c89">C89</span> | Process termination & environment | [Program Utilities](../reference/library/program.md) |
| `_Exit` | `<stdlib.h>` | <span class="c-badge c99">C99</span> | Immediate process exit | [Program Utilities](../reference/library/program.md) |
| `quick_exit`, `at_quick_exit` | `<stdlib.h>` | <span class="c-badge c11">C11</span> | Fast controlled exit | [Program Utilities](../reference/library/program.md) |
| `signal`, `raise`, `sig_atomic_t`, `SIG_DFL`, `SIG_IGN`, `SIG_ERR`, `SIGINT`, `SIGTERM`, `SIGABRT`, `SIGFPE`, `SIGILL`, `SIGSEGV` | `<signal.h>` | <span class="c-badge c89">C89</span> | Asynchronous signal handling | [Program Utilities](../reference/library/program.md) |
| `setjmp`, `longjmp`, `jmp_buf` | `<setjmp.h>` | <span class="c-badge c89">C89</span> | Non-local jumps | [Program Utilities](../reference/library/program.md) |
| `assert`, `NDEBUG` | `<assert.h>` | <span class="c-badge c89">C89</span> | Runtime diagnostics | [Variadic & Error Handling](../reference/library/variadic-and-error.md) |
| `errno`, `EDOM`, `ERANGE` | `<errno.h>` | <span class="c-badge c89">C89</span> | Thread-local error status | [Variadic & Error Handling](../reference/library/variadic-and-error.md) |
| `EILSEQ` | `<errno.h>` | <span class="c-badge c95">C95</span> | Illegal multibyte sequence error | [Variadic & Error Handling](../reference/library/variadic-and-error.md) |
| `va_list`, `va_start`, `va_arg`, `va_end` | `<stdarg.h>` | <span class="c-badge c89">C89</span> / <span class="c-badge c23">C23</span> | Variadic argument traversal | [Variadic & Error Handling](../reference/library/variadic-and-error.md) |
| `va_copy` | `<stdarg.h>` | <span class="c-badge c99">C99</span> | Copy `va_list` state | [Variadic & Error Handling](../reference/library/variadic-and-error.md) |

---

## 4. Dynamic Memory & Algorithms

| Symbol | Header | Since | Category | Reference Link |
| :--- | :--- | :--- | :--- | :--- |
| `malloc`, `calloc`, `realloc`, `free` | `<stdlib.h>` | <span class="c-badge c89">C89</span> | Heap memory allocation | [Dynamic Memory](../reference/library/memory.md) |
| `aligned_alloc` | `<stdlib.h>` | <span class="c-badge c11">C11</span> | Over-aligned allocation | [Dynamic Memory](../reference/library/memory.md) |
| `free_sized`, `free_aligned_sized`, `memalignment` | `<stdlib.h>` | <span class="c-badge c23">C23</span> | Sized deallocation & alignment query | [Dynamic Memory](../reference/library/memory.md) |
| `qsort`, `bsearch` | `<stdlib.h>` | <span class="c-badge c89">C89</span> / <span class="c-badge c23">C23</span> | Generic sorting & binary search (`QVoid` in C23) | [Algorithms](../reference/library/algorithms.md) |
| `abs`, `labs`, `div`, `ldiv` | `<stdlib.h>` | <span class="c-badge c89">C89</span> | Integer abs & quotient/remainder | [Math & Complex](../reference/library/numerics-math.md) |
| `llabs`, `lldiv` | `<stdlib.h>` | <span class="c-badge c99">C99</span> | 64-bit `long long` abs & div | [Math & Complex](../reference/library/numerics-math.md) |

---

## 5. Byte Strings & Character Classification

| Symbol | Header | Since | Category | Reference Link |
| :--- | :--- | :--- | :--- | :--- |
| `memcpy`, `memmove`, `memset`, `memcmp`, `memchr` | `<string.h>` | <span class="c-badge c89">C89</span> / <span class="c-badge c23">C23</span> | Raw memory block operations | [Byte Strings](../reference/library/strings-byte.md) |
| `memset_explicit`, `memccpy`, `strdup`, `strndup` | `<string.h>` | <span class="c-badge c23">C23</span> | Secure zeroing, bounded copy & duplication | [Byte Strings](../reference/library/strings-byte.md) |
| `strlen`, `strcpy`, `strncpy`, `strcat`, `strncat`, `strcmp`, `strncmp`, `strcoll`, `strxfrm` | `<string.h>` | <span class="c-badge c89">C89</span> | Null-terminated string ops | [Byte Strings](../reference/library/strings-byte.md) |
| `strchr`, `strrchr`, `strspn`, `strcspn`, `strpbrk`, `strstr`, `strtok`, `strerror` | `<string.h>` | <span class="c-badge c89">C89</span> / <span class="c-badge c23">C23</span> | Search (`QChar` const-safe in C23) & errors | [Byte Strings](../reference/library/strings-byte.md) |
| `strtol`, `strtoul`, `strtod`, `atoi`, `atol`, `atof` | `<stdlib.h>` | <span class="c-badge c89">C89</span> | String-to-number conversion | [Byte Strings](../reference/library/strings-byte.md) |
| `strtoll`, `strtoull`, `strtof`, `strtold`, `atoll` | `<stdlib.h>` | <span class="c-badge c99">C99</span> | Extended numeric parsing | [Byte Strings](../reference/library/strings-byte.md) |
| `strfromd`, `strfromf`, `strfroml`, `strfromd32`, `strfromd64`, `strfromd128` | `<stdlib.h>` | <span class="c-badge c23">C23</span> | Float-to-string conversion | [Byte Strings](../reference/library/strings-byte.md) |
| `isalnum`, `isalpha`, `iscntrl`, `isdigit`, `isgraph`, `islower`, `isprint`, `ispunct`, `isspace`, `isupper`, `isxdigit`, `tolower`, `toupper` | `<ctype.h>` | <span class="c-badge c89">C89</span> | Byte character classification | [Byte Strings](../reference/library/strings-byte.md) |
| `isblank` | `<ctype.h>` | <span class="c-badge c99">C99</span> | Horizontal whitespace test | [Byte Strings](../reference/library/strings-byte.md) |

---

## 6. Multibyte, Unicode & Wide Strings

| Symbol | Header | Since | Category | Reference Link |
| :--- | :--- | :--- | :--- | :--- |
| `mblen`, `mbtowc`, `wctomb`, `mbstowcs`, `wcstombs`, `MB_CUR_MAX` | `<stdlib.h>` | <span class="c-badge c89">C89</span> | Non-restartable multibyte conversion | [Multibyte & Wide Strings](../reference/library/strings-multibyte-wide.md) |
| `mbstate_t`, `wint_t`, `WEOF`, `mbrtowc`, `wcrtomb`, `mbsrtowcs`, `wcsrtombs`, `mbrlen`, `mbsinit`, `btowc`, `wctob` | `<wchar.h>` | <span class="c-badge c95">C95</span> | Restartable multibyte conversion | [Multibyte & Wide Strings](../reference/library/strings-multibyte-wide.md) |
| `wcslen`, `wcscpy`, `wcsncpy`, `wcscat`, `wcsncat`, `wcscmp`, `wcsncmp`, `wcscoll`, `wcsxfrm`, `wcschr`, `wcsrchr`, `wcsstr`, `wcspbrk`, `wcsspn`, `wcscspn`, `wcstok` | `<wchar.h>` | <span class="c-badge c95">C95</span> | Wide-character string operations | [Multibyte & Wide Strings](../reference/library/strings-multibyte-wide.md) |
| `wmemcpy`, `wmemmove`, `wmemset`, `wmemcmp`, `wmemchr` | `<wchar.h>` | <span class="c-badge c95">C95</span> | Wide-character memory operations | [Multibyte & Wide Strings](../reference/library/strings-multibyte-wide.md) |
| `wprintf`, `fwprintf`, `swprintf`, `vwprintf`, `wscanf`, `fwscanf`, `swscanf`, `fgetwc`, `fputwc`, `fgetws`, `fputws`, `ungetwc`, `fwide`, `wcsftime` | `<wchar.h>` | <span class="c-badge c95">C95</span> | Wide-character I/O & time format | [Multibyte & Wide Strings](../reference/library/strings-multibyte-wide.md) |
| `iswalnum`, `iswalpha`, `iswcntrl`, `iswdigit`, `iswgraph`, `iswlower`, `iswprint`, `iswpunct`, `iswspace`, `iswupper`, `iswxdigit`, `towlower`, `towupper`, `wctype`, `iswctype`, `wctrans`, `towctrans` | `<wctype.h>` | <span class="c-badge c95">C95</span> | Wide-character classification | [Multibyte & Wide Strings](../reference/library/strings-multibyte-wide.md) |
| `iswblank` | `<wctype.h>` | <span class="c-badge c99">C99</span> | Wide blank test | [Multibyte & Wide Strings](../reference/library/strings-multibyte-wide.md) |
| `char16_t`, `char32_t`, `mbrtoc16`, `c16rtomb`, `mbrtoc32`, `c32rtomb` | `<uchar.h>` | <span class="c-badge c11">C11</span> | UTF-16 & UTF-32 conversions | [Multibyte & Wide Strings](../reference/library/strings-multibyte-wide.md) |
| `char8_t`, `mbrtoc8`, `c8rtomb` | `<uchar.h>` | <span class="c-badge c23">C23</span> | UTF-8 character type & conversions | [Multibyte & Wide Strings](../reference/library/strings-multibyte-wide.md) |

---

## 7. Input / Output Support (`<stdio.h>`)

| Symbol | Header | Since | Category | Reference Link |
| :--- | :--- | :--- | :--- | :--- |
| `FILE`, `fpos_t`, `EOF`, `BUFSIZ`, `FOPEN_MAX`, `FILENAME_MAX`, `stdin`, `stdout`, `stderr` | `<stdio.h>` | <span class="c-badge c89">C89</span> | Stream types, constants & handles | [Input/Output (`<stdio.h>`)](../reference/library/io.md) |
| `fopen`, `freopen`, `fclose`, `fflush`, `setbuf`, `setvbuf`, `_IOFBF`, `_IOLBF`, `_IONBF` | `<stdio.h>` | <span class="c-badge c89">C89</span> | Stream lifecycle & buffering | [Input/Output (`<stdio.h>`)](../reference/library/io.md) |
| `fread`, `fwrite`, `fgetc`, `getc`, `getchar`, `fgets`, `fputc`, `putc`, `putchar`, `fputs`, `puts`, `ungetc` | `<stdio.h>` | <span class="c-badge c89">C89</span> | Unformatted & block I/O | [Input/Output (`<stdio.h>`)](../reference/library/io.md) |
| `printf`, `fprintf`, `sprintf`, `vprintf`, `vfprintf`, `vsprintf`, `scanf`, `fscanf`, `sscanf` | `<stdio.h>` | <span class="c-badge c89">C89</span> | Formatted I/O (`%b`, `%wN` in C23) | [Input/Output (`<stdio.h>`)](../reference/library/io.md) |
| `snprintf`, `vsnprintf`, `vscanf`, `vfscanf`, `vsscanf` | `<stdio.h>` | <span class="c-badge c99">C99</span> | Bounded & variadic formatted I/O | [Input/Output (`<stdio.h>`)](../reference/library/io.md) |
| `fseek`, `ftell`, `rewind`, `fgetpos`, `fsetpos`, `SEEK_SET`, `SEEK_CUR`, `SEEK_END` | `<stdio.h>` | <span class="c-badge c89">C89</span> | Stream positioning | [Input/Output (`<stdio.h>`)](../reference/library/io.md) |
| `clearerr`, `feof`, `ferror`, `perror`, `remove`, `rename`, `tmpfile`, `tmpnam` | `<stdio.h>` | <span class="c-badge c89">C89</span> | Error inspection & filesystem ops | [Input/Output (`<stdio.h>`)](../reference/library/io.md) |

---

## 8. Numerics: Math, FP Environment, Complex, Bitwise & Checked Arithmetic

| Symbol | Header | Since | Category | Reference Link |
| :--- | :--- | :--- | :--- | :--- |
| `fabs`, `fmod`, `exp`, `log`, `log10`, `pow`, `sqrt`, `sin`, `cos`, `tan`, `asin`, `acos`, `atan`, `atan2`, `sinh`, `cosh`, `tanh`, `ceil`, `floor`, `frexp`, `ldexp`, `modf`, `HUGE_VAL` | `<math.h>` | <span class="c-badge c89">C89</span> | Elementary math functions | [Numerics: Math & Complex](../reference/library/numerics-math.md) |
| `remainder`, `remquo`, `fma`, `fmax`, `fmin`, `fdim`, `exp2`, `expm1`, `log2`, `log1p`, `cbrt`, `hypot`, `asinh`, `acosh`, `atanh`, `erf`, `erfc`, `tgamma`, `lgamma`, `trunc`, `round`, `lround`, `llround`, `nearbyint`, `rint`, `lrint`, `scalbn`, `ilogb`, `logb`, `nextafter`, `copysign`, `nan`, `fpclassify`, `isfinite`, `isinf`, `isnan`, `isnormal`, `signbit` | `<math.h>` | <span class="c-badge c99">C99</span> | C99 IEEE 754 math suite | [Numerics: Math & Complex](../reference/library/numerics-math.md) |
| `sinpi`, `cospi`, `tanpi`, `asinpi`, `acospi`, `atanpi`, `atan2pi`, `exp10`, `exp10m1`, `exp2m1`, `log10p1`, `log2p1`, `logp1`, `rsqrt`, `compoundn`, `pown`, `powr`, `rootn`, `fmaximum`, `fminimum`, `fmaximum_num`, `fminimum_num`, `fromfp`, `ufromfp`, `roundeven`, `canonicalize`, `totalorder`, `totalordermag`, `getpayload`, `setpayload`, `issignaling`, `issubnormal`, `iszero`, `iscanonical` | `<math.h>` | <span class="c-badge c23">C23</span> | C23 / IEEE 754-2019 math extensions | [Numerics: Math & Complex](../reference/library/numerics-math.md) |
| `fenv_t`, `fexcept_t`, `fegetround`, `fesetround`, `feclearexcept`, `fetestexcept`, `feraiseexcept`, `fegetexceptflag`, `fesetexceptflag`, `fegetenv`, `fesetenv`, `feholdexcept`, `feupdateenv`, `FE_ALL_EXCEPT`, `FE_DIVBYZERO`, `FE_INEXACT`, `FE_INVALID`, `FE_OVERFLOW`, `FE_UNDERFLOW`, `FE_TONEAREST`, `FE_TOWARDZERO`, `FE_UPWARD`, `FE_DOWNWARD` | `<fenv.h>` | <span class="c-badge c99">C99</span> | Floating-point environment | [Numerics: Math & Complex](../reference/library/numerics-math.md) |
| `complex`, `I`, `creal`, `cimag`, `cabs`, `carg`, `conj`, `cproj`, `cexp`, `clog`, `cpow`, `csqrt`, `csin`, `ccos`, `ctan`, `casin`, `cacos`, `catan`, `csinh`, `ccosh`, `ctanh` | `<complex.h>` | <span class="c-badge c99">C99</span> | Complex arithmetic | [Numerics: Math & Complex](../reference/library/numerics-math.md) |
| `CMPLX`, `CMPLXF`, `CMPLXL` | `<complex.h>` | <span class="c-badge c11">C11</span> | Complex constructor macros | [Numerics: Math & Complex](../reference/library/numerics-math.md) |
| Type-generic math macros (`sin`, `sqrt`, `exp`, `pow`, `fabs`, etc.) | `<tgmath.h>` | <span class="c-badge c99">C99</span> | Auto-dispatched real/complex math | [Numerics: Math & Complex](../reference/library/numerics-math.md) |
| `rand`, `srand`, `RAND_MAX` | `<stdlib.h>` | <span class="c-badge c89">C89</span> | Pseudo-random number generator | [Numerics: Math & Complex](../reference/library/numerics-math.md) |
| `__STDC_ENDIAN_LITTLE__`, `__STDC_ENDIAN_BIG__`, `__STDC_ENDIAN_NATIVE__`, `stdc_leading_zeros`, `stdc_leading_ones`, `stdc_trailing_zeros`, `stdc_trailing_ones`, `stdc_first_leading_zero`, `stdc_first_leading_one`, `stdc_first_trailing_zero`, `stdc_first_trailing_one`, `stdc_count_zeros`, `stdc_count_ones`, `stdc_has_single_bit`, `stdc_bit_width`, `stdc_bit_floor`, `stdc_bit_ceil` | `<stdbit.h>` | <span class="c-badge c23">C23</span> | Bit & byte manipulation | [Bit & Checked Int (C23)](../reference/library/numerics-bit-checked.md) |
| `ckd_add`, `ckd_sub`, `ckd_mul` | `<stdckdint.h>` | <span class="c-badge c23">C23</span> | Checked integer arithmetic | [Bit & Checked Int (C23)](../reference/library/numerics-bit-checked.md) |

---

## 9. Date, Time & Localization

| Symbol | Header | Since | Category | Reference Link |
| :--- | :--- | :--- | :--- | :--- |
| `time_t`, `clock_t`, `struct tm`, `CLOCKS_PER_SEC`, `time`, `clock`, `difftime`, `mktime`, `gmtime`, `localtime`, `strftime` | `<time.h>` | <span class="c-badge c89">C89</span> | Calendar & CPU time | [Date & Time (`<time.h>`)](../reference/library/chrono.md) |
| `asctime`, `ctime` | `<time.h>` | <span class="c-badge c89">C89</span> <span class="c-badge c-deprecated">deprecated C23</span> | Unsafe static-buffer time formatting | [Date & Time (`<time.h>`)](../reference/library/chrono.md) |
| `struct timespec`, `TIME_UTC`, `timespec_get` | `<time.h>` | <span class="c-badge c11">C11</span> | Nanosecond-resolution clock | [Date & Time (`<time.h>`)](../reference/library/chrono.md) |
| `gmtime_r`, `localtime_r`, `timegm`, `timespec_getres`, `TIME_MONOTONIC`, `TIME_ACTIVE`, `TIME_THREAD_ACTIVE` | `<time.h>` | <span class="c-badge c23">C23</span> | Thread-safe UTC/local & clock resolution | [Date & Time (`<time.h>`)](../reference/library/chrono.md) |
| `setlocale`, `localeconv`, `struct lconv`, `LC_ALL`, `LC_COLLATE`, `LC_CTYPE`, `LC_MONETARY`, `LC_NUMERIC`, `LC_TIME` | `<locale.h>` | <span class="c-badge c89">C89</span> | Locale configuration & formatting | [Localization (`<locale.h>`)](../reference/library/locale.md) |

---

## 10. Concurrency: Threads & Atomics <span class="c-badge c11">C11</span> <span class="c-badge c-optional">optional</span>

| Symbol | Header | Since | Category | Reference Link |
| :--- | :--- | :--- | :--- | :--- |
| `thrd_t`, `thrd_start_t`, `thrd_create`, `thrd_join`, `thrd_detach`, `thrd_current`, `thrd_equal`, `thrd_sleep`, `thrd_yield`, `thrd_exit`, `thrd_success`, `thrd_timedout`, `thrd_busy`, `thrd_nomem`, `thrd_error` | `<threads.h>` | <span class="c-badge c11">C11</span> | Portable thread lifecycle | [Concurrency](../reference/library/concurrency.md) |
| `mtx_t`, `mtx_init`, `mtx_lock`, `mtx_timedlock`, `mtx_trylock`, `mtx_unlock`, `mtx_destroy`, `mtx_plain`, `mtx_recursive`, `mtx_timed` | `<threads.h>` | <span class="c-badge c11">C11</span> | Mutual exclusion locks | [Concurrency](../reference/library/concurrency.md) |
| `cnd_t`, `cnd_init`, `cnd_signal`, `cnd_broadcast`, `cnd_wait`, `cnd_timedwait`, `cnd_destroy` | `<threads.h>` | <span class="c-badge c11">C11</span> | Condition variables | [Concurrency](../reference/library/concurrency.md) |
| `tss_t`, `tss_dtor_t`, `tss_create`, `tss_get`, `tss_set`, `tss_delete`, `once_flag`, `ONCE_FLAG_INIT`, `call_once` | `<threads.h>` | <span class="c-badge c11">C11</span> | Thread-specific storage & one-time init | [Concurrency](../reference/library/concurrency.md) |
| `memory_order`, `memory_order_relaxed`, `memory_order_consume`, `memory_order_acquire`, `memory_order_release`, `memory_order_acq_rel`, `memory_order_seq_cst` | `<stdatomic.h>` | <span class="c-badge c11">C11</span> | Hardware memory ordering | [Concurrency](../reference/library/concurrency.md) |
| `atomic_bool`, `atomic_int`, `atomic_size_t`, `atomic_uintptr_t`, `atomic_flag`, `ATOMIC_FLAG_INIT`, `atomic_flag_test_and_set`, `atomic_flag_clear` | `<stdatomic.h>` | <span class="c-badge c11">C11</span> | Atomic types & lock-free flags | [Concurrency](../reference/library/concurrency.md) |
| `atomic_init`, `atomic_load`, `atomic_load_explicit`, `atomic_store`, `atomic_store_explicit`, `atomic_exchange`, `atomic_compare_exchange_strong`, `atomic_compare_exchange_weak`, `atomic_fetch_add`, `atomic_fetch_sub`, `atomic_fetch_and`, `atomic_fetch_or`, `atomic_fetch_xor`, `atomic_thread_fence`, `atomic_signal_fence`, `atomic_is_lock_free`, `kill_dependency` | `<stdatomic.h>` | <span class="c-badge c11">C11</span> | Atomic operations & fences | [Concurrency](../reference/library/concurrency.md) |

---

## 11. Curated Non-ISO C Ecosystem Libraries (2026)

Because the ISO C standard library stays deliberately compact, production C systems rely on battle-tested, ABI-stable C libraries for networking, cryptography, persistence, compression, testing, and multimedia.

### Networking, HTTP & Asynchronous Event Loops

| Library | License | Scope & Engineering Notes | Homepage |
| :--- | :--- | :--- | :--- |
| **`libuv`** | MIT | Cross-platform asynchronous I/O event loop (wrapping `epoll`, `kqueue`, `io_uring`, Windows IOCP); powers Node.js and Julia. | [libuv.org](https://libuv.org/) |
| **`libevent`** | BSD-3 | Mature event-notification loop with built-in buffered sockets, DNS, and lightweight HTTP server primitives. | [libevent.org](https://libevent.org/) |
| **`libcurl`** | curl (MIT-like) | Industry-standard multi-protocol client library (HTTP/1.1, HTTP/2, HTTP/3/QUIC, TLS, FTP, SMTP, WebSockets) with async `multi` API. | [curl.se/libcurl](https://curl.se/libcurl/) |
| **`nghttp2`** | MIT | Production C framing library for HTTP/2 multiplexing, HPACK header compression, and stream prioritization (plus `ngtcp2`/`nghttp3` for QUIC). | [nghttp2.org](https://nghttp2.org/) |
| **`liburing`** | MIT / LGPL | Thin C wrapper over Linux `io_uring` submission/completion ring buffers for zero-syscall async disk and network I/O. | [github.com/axboe/liburing](https://github.com/axboe/liburing) |

### Cryptography & TLS

| Library | License | Scope & Engineering Notes | Homepage |
| :--- | :--- | :--- | :--- |
| **`OpenSSL`** | Apache-2.0 | Ubiquitous full-featured TLS 1.3 + X.509 + FIPS-validated cryptographic provider library (`libssl`, `libcrypto`). | [openssl.org](https://www.openssl.org/) |
| **`libsodium`** | ISC | Modern, misuse-resistant high-level cryptography (`XChaCha20-Poly1305`, `Ed25519`, `Argon2id`, `BLAKE2b`, constant-time memory helpers). | [libsodium.org](https://doc.libsodium.org/) |
| **`mbedTLS`** | Apache-2.0 | Modular, small-footprint TLS and crypto library ideal for embedded devices, RTOS targets, and static linking. | [trustedfirmware.org/projects/mbed-tls](https://www.trustedfirmware.org/projects/mbed-tls/) |
| **`BoringSSL`** | ISC / OpenSSL | Google's hardened OpenSSL fork powering Chrome, Android, Envoy, and gRPC (API evolves continuously; pin commits). | [boringssl.googlesource.com](https://boringssl.googlesource.com/boringssl/) |

### Databases, Serialization & Compression

| Library | License | Scope & Engineering Notes | Homepage |
| :--- | :--- | :--- | :--- |
| **`SQLite`** | Public Domain | World's most deployed transactional SQL database engine delivered as a single self-contained C amalgamation (`sqlite3.c`). | [sqlite.org](https://www.sqlite.org/) |
| **`LMDB`** | OpenLDAP | Ultra-fast, memory-mapped (`mmap`) ACID B+tree key-value store with zero-copy MVCC reads. | [symas.com/lmdb](https://www.symas.com/lmdb) |
| **`yyjson`** | MIT | High-speed ANSI C99 JSON reader/writer capable of gigabytes-per-second parsing with strict RFC 8259 compliance. | [github.com/ibireme/yyjson](https://github.com/ibireme/yyjson) |
| **`cJSON`** | MIT | Minimalist two-file (`cJSON.c`/`cJSON.h`) JSON parser widely used in firmware and embedded gateways. | [github.com/DaveGamble/cJSON](https://github.com/DaveGamble/cJSON) |
| **`zlib` / `zlib-ng`** | zlib | Ubiquitous DEFLATE/gzip compression library; `zlib-ng` adds SIMD (AVX2/NEON) drop-in acceleration. | [zlib.net](https://zlib.net/) |
| **`zstd` (Zstandard)** | BSD-3 / GPLv2 | Real-time compression library offering superior compression ratios and speed over `zlib`, plus dictionary training. | [facebook.github.io/zstd](https://facebook.github.io/zstd/) |
| **`lz4`** | BSD-2 | Extreme-throughput compression (>500 MB/s per core compression, multi-GB/s decompression) for databases, IPC, and kernels. | [lz4.org](https://lz4.org/) |

### High-Performance Memory Allocators

| Library | License | Scope & Engineering Notes | Homepage |
| :--- | :--- | :--- | :--- |
| **`jemalloc`** | BSD-2 | Fragmentation-resistant, per-arena multithreaded allocator with built-in heap profiling and C23 `free_sized` (`sdallocx`) roots. | [jemalloc.net](https://jemalloc.net/) |
| **`mimalloc`** | MIT | Compact, ultra-low-latency general-purpose allocator from Microsoft Research with free-list sharding and hardened security mode. | [github.com/microsoft/mimalloc](https://github.com/microsoft/mimalloc) |

### Testing, CLI, Regex, OS Foundations & Graphics

| Library | License | Scope & Engineering Notes | Homepage |
| :--- | :--- | :--- | :--- |
| **`Unity`** | MIT | Lightweight pure-C unit-testing framework suited for both host CI pipelines and bare-metal microcontrollers. | [throwtheswitch.org/unity](https://www.throwtheswitch.org/unity) |
| **`cmocka`** | Apache-2.0 | C unit-testing and mock-object framework supporting link-time function wrapping (`--wrap`) and leak checks. | [cmocka.org](https://cmocka.org/) |
| **`POSIX.1-2024`** | IEEE / Open Group | Standard UNIX systems API (`pthreads`, sockets, `mmap`, `poll`, `getline`, `clock_gettime`, `dlopen`, `regex.h`). | [pubs.opengroup.org](https://pubs.opengroup.org/onlinepubs/9799919799/) |
| **`PCRE2`** | BSD-3 | Full Perl-compatible regular expression engine with JIT compilation and UTF-8/16/32 support. | [pcre.org](https://www.pcre.org/) |
| **`SDL3`** | zlib | Modern cross-platform C abstraction layer for GPU rendering, windowing, audio, gamepads, and input across desktop/mobile. | [libsdl.org](https://www.libsdl.org/) |
| **`raylib`** | zlib | Clean, zero-dependency C99/C23 multimedia and 2D/3D graphics library beloved for tooling, simulations, and visualization. | [raylib.com](https://www.raylib.com/) |
| **`stb`** | Public Domain / MIT | Iconic single-header C libraries (`stb_image.h`, `stb_truetype.h`, `stb_ds.h`, `stb_sprintf.h`) for frictionless integration. | [github.com/nothings/stb](https://github.com/nothings/stb) |

---

## Related References

- [ISO C Standards History & Evolution](index.md)
- [C23 Migration & Breaking Changes Guide](c23-migration.md)
- [Compiler & Libc Support Matrix](compiler-support.md)
- [Technical Specifications & Reports (TS / TR)](technical-specifications.md)
- [Language Reference Index](../reference/language/index.md) · [Standard Library Index](../reference/library/index.md)
- [cppreference.com — C Symbol Index](https://en.cppreference.com/w/c/symbol_index) · [cppreference.com — Non-ISO C Libraries](https://en.cppreference.com/w/c/links/libs)
