---
title: C23 Migration & Breaking Changes Guide
description: Complete C11/C17 to ISO C23 (ISO/IEC 9899:2024) migration manual covering every removed feature, deprecation, new language feature, new library API, and compiler audit workflow.
icon: lucide/wrench
---

# C23 Migration & Breaking Changes Guide <span class="c-badge c23">C23</span>

**ISO/IEC 9899:2024 (C23)** (`__STDC_VERSION__ == 202311L`) is the most aggressive cleanup and modernization of C since C99. While well-written modern C11/C17 code usually compiles cleanly under `-std=c23`, legacy codebases encounter real **source-breaking removals**, **new core keywords**, **stricter `const`-preservation rules**, and **reclassified undefined behaviors** (notably `realloc(ptr, 0)`).

This guide mirrors [cppreference.com — C23](https://en.cppreference.com/w/c/23) and walks through every breaking change, deprecation, language addition, and library addition with direct **C17 vs. C23** migrations.

---

## 1. Breaking Changes: Removed Features <span class="c-badge c-removed">removed C23</span>

Audit these **nine high-impact breaking changes** before switching build flags to `-std=c23` (or upgrading to GCC 15, where `-std=gnu23` is the default).

### 1.1 Old-Style K&R Function Declarations & Empty `()` Meaning

C23 removes K&R identifier-list function definitions (`int add(a, b) int a; int b; { ... }`) and redefines an empty parameter list `int f()` to mean **`int f(void)`** (zero parameters), matching C++. Previously, `int f()` meant *"unspecified number and types of parameters"*.

=== "C17 (Legacy / Breaks in C23)"

    ```c
    /* K&R definition: syntax error in C23 */
    int compute(a, b)
        int a;
        int b;
    {
        return a + b;
    }

    /* Empty () meant "unspecified parameters" in C17 */
    void (*callback)();

    void register_cb(void (*fn)()) {
        callback = fn;
        callback(42, "legacy"); // Valid in C17, hard error in C23!
    }
    ```

=== "C23 (Strict Prototypes Required)"

    ```c
    /* Prototype syntax mandatory everywhere */
    int compute(int a, int b) {
        return a + b;
    }

    /* Empty () now strictly means (void) — zero parameters */
    void init_subsystem() { // Equivalent to void init_subsystem(void)
        // ...
    }

    /* Typed function pointers are mandatory */
    typedef void (*event_cb_t)(int code, const char *msg);
    static event_cb_t g_callback = nullptr;

    void register_cb(event_cb_t fn) {
        g_callback = fn;
        if (g_callback != nullptr) {
            g_callback(42, "typed");
        }
    }
    ```

!!! warning "Function Pointer Casts & `dlsym` / Callback Tables"
    Legacy plugin APIs often declared generic callback slots as `void (*fn)()`. Under C23, assigning `void (*)(int)` to `void (*)()` is a **constraint violation** (incompatible function pointer types). Declare explicit typed function pointers or use a documented generic placeholder cast such as `void (*)(void)` cast back to the exact signature before calling.

### 1.2 Mandatory Two's Complement Signed Integers

C23 removes permission for **sign-magnitude** and **ones' complement** representations of signed integers. All signed integer types are guaranteed to use **two's complement** without negative zero or sign-bit trap representations.

- `INT_MIN` is strictly equal to `-INT_MAX - 1` (likewise for all signed integer widths).
- Signed integer overflow (`INT_MAX + 1`) **remains Undefined Behavior**! Two's complement fixes *representation*, not overflow wrap-around semantics. Use `<stdckdint.h>` for defined overflow detection.
- Right-shifting a negative signed integer (`x >> k`) remains **implementation-defined** (usually arithmetic shift, though not required to be).

### 1.3 Mandatory UTF-16 / UTF-32 for `u` and `U` Literals

C11 allowed `u'x'`, `u"..."`, `U'x'`, and `U"..."` to use non-Unicode encodings unless `__STDC_UTF_16__` or `__STDC_UTF_32__` were defined. C23 removes that permission: `u` literals are **always UTF-16** (`char16_t`), `U` literals are **always UTF-32** (`char32_t`), and `u8` literals are **always UTF-8** (`char8_t` / `unsigned char`).

### 1.4 Removal of Mixed-Width Wide String Literal Concatenation

Concatenating string literals with conflicting encoding prefixes (for example `u"hello" U"world"` or `L"a" u8"b"`) was implementation-defined in C11/C17. In C23 it is **ill-formed** (a constraint violation). Concatenating an unprefixed literal with a prefixed literal (e.g., `u8"hello " "world"`) remains valid and adopts the prefix.

=== "C17 (Implementation-Defined)"

    ```c
    /* Rejected in C23! */
    const char16_t *bad = u"prefix_" U"suffix";
    ```

=== "C23 (Valid)"

    ```c
    /* Unprefixed + prefixed is valid; all prefixes must match otherwise */
    const char16_t *ok1 = u"prefix_" "suffix";
    const char16_t *ok2 = u"prefix_" u"suffix";
    ```

### 1.5 `realloc(ptr, 0)` Is Now Undefined Behavior

!!! danger "Critical Runtime Security Pitfall"
    In C17, calling `realloc(ptr, 0)` with a non-null `ptr` was *implementation-defined* (glibc freed `ptr` and returned `NULL`; BSD/macOS returned a minimal unique pointer or freed it). **In C23 (ISO/IEC 9899:2024 §7.24.3.7), calling `realloc(ptr, 0)` is strictly Undefined Behavior!**

=== "C17 (Non-Portable / UB in C23)"

    ```c
    /* DANGEROUS: UB in C23 when new_size == 0! */
    void *resized = realloc(buf, new_size);
    if (!resized && new_size > 0) {
        handle_oom();
    }
    buf = resized;
    ```

=== "C23 (Safe Portable Wrapper)"

    ```c
    #include <stdlib.h>

    [[nodiscard]] void *safe_realloc(void *ptr, size_t new_size) {
        if (new_size == 0) {
            free(ptr);
            return nullptr;
        }
        return realloc(ptr, new_size);
    }
    ```

### 1.6 Removal of Trigraphs

All nine trigraph sequences (`??=`, `??/`, `??'`, `??(`, `??)`, `??!`, `??<`, `??>`, `??-`) have been **removed** from Translation Phase 1. Strings like `"What??!"` now literally contain `??!` instead of `"What|"`. Digraphs (`<:`, `:>`, `<%`, `%>`, `%:`, `%:%:`) remain valid.

### 1.7 Macro-to-Keyword Promotion & Removed `*_is_defined` Macros

C23 promotes seven standard keywords out of compatibility headers into the core language and removes obsolete indicator macros:

- **Removed macros**: `__alignas_is_defined`, `__alignof_is_defined` (from `<stdalign.h>`), and `ATOMIC_VAR_INIT` (from `<stdatomic.h>`, deprecated in C17 and removed in C23).
- **Promoted keywords**: `bool`, `true`, `false`, `alignas`, `alignof`, `static_assert`, `thread_local`, `nullptr`, `constexpr`, `typeof`, `typeof_unqual`.
- **Two migration breaks to watch for**:
  1. **Identifier collisions**: Any variable, struct field, typedef, or function named `bool`, `true`, `false`, `nullptr`, `constexpr`, `typeof`, `alignas`, `alignof`, `static_assert`, or `thread_local` will fail to compile under `-std=c23`.
  2. **Preprocessor `#ifdef` checks**: Code checking `#ifdef static_assert` or `#ifdef thread_local` or `#ifdef bool` now evaluates to **false** because keywords are no longer preprocessor macros! Use `#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L`.

### 1.8 `QChar` / `QVoid` `const`-Preserving Search Functions

In C17, `strchr(const char *s, int c)` always returned `char *`, silently discarding `const`. In C23, `memchr`, `strchr`, `strpbrk`, `strrchr`, `strstr`, `wcschr`, `wcspbrk`, `wcsrchr`, `wcsstr`, `wmemchr`, and `bsearch` are **type-generic macros** that preserve `const` qualification when given a `const` pointer!

=== "C17 (Silently Strips `const`)"

    ```c
    const char *path = "/etc/hosts";
    char *slash = strchr(path, '/'); // Compiled silently in C17!
    ```

=== "C23 (`const` Preserved — Compile Error if Discarded)"

    ```c
    const char *path = "/etc/hosts";
    const char *slash = strchr(path, '/'); // Returns const char *!

    char mutable_path[] = "/etc/hosts";
    char *m_slash = strchr(mutable_path, '/'); // Still returns char *!
    ```

---

## 2. Deprecated Features in C23 <span class="c-badge c-deprecated">deprecated C23</span>

Deprecated features remain valid in C23 for backward compatibility, but compilers emit warnings and WG14 may remove them in C2y/C3x.

| Deprecated Feature | Header / Location | C23 Replacement | Reason for Deprecation |
| :--- | :--- | :--- | :--- |
| `<stdnoreturn.h>` | Standard header | `[[noreturn]]` attribute | Superseded by standard attribute syntax. |
| `_Noreturn` | Function specifier | `[[noreturn]]` | Superseded by `[[noreturn]]` (also matches C++). |
| `__STDC_IEC_559__` | Predefined macro | `__STDC_IEC_60559_BFP__` | Reflects updated IEEE 754-2019 / ISO/IEC 60559:2020 naming and version value (`202311L`). |
| `__STDC_IEC_559_COMPLEX__` | Predefined macro | `__STDC_IEC_60559_COMPLEX__` | Aligned with ISO/IEC 60559:2020 complex arithmetic annex. |
| `asctime()` | `<time.h>` | `strftime()` with `gmtime_r()` / `localtime_r()` | Non-reentrant static buffer; UB on years > 9999 or out-of-range fields. |
| `ctime()` | `<time.h>` | `strftime()` with `localtime_r()` | Same thread-safety and buffer-overflow hazards as `asctime()`. |
| `DECIMAL_DIG` | `<float.h>` | `FLT_DECIMAL_DIG`, `DBL_DECIMAL_DIG`, `LDBL_DECIMAL_DIG` | Ambiguous with extended and decimal floating-point types. |
| `INFINITY`, `NAN`, `DEC_INFINITY`, `DEC_NAN` via `<math.h>` | `<math.h>` | Include `<float.h>` instead | Moved to `<float.h>` as fundamental floating-point representation constants. |
| `__bool_true_false_are_defined` | `<stdbool.h>` | `__STDC_VERSION__ >= 202311L` | `bool`, `true`, `false` are now language keywords; `<stdbool.h>` itself is obsolescent. |
| `_Alignas`, `_Alignof`, `_Bool`, `_Static_assert`, `_Thread_local` | Core keywords | `alignas`, `alignof`, `bool`, `static_assert`, `thread_local` | Alternate spellings kept for compatibility; clean spellings are preferred. |

---

## 3. New C23 Language Features Checklist <span class="c-badge c23">C23</span>

Every major language addition in ISO/IEC 9899:2024 is shown below with C17 vs. C23 comparisons.

### 3.1 `nullptr` and `nullptr_t`

`nullptr` is a null pointer constant of distinct type `nullptr_t` (declared in `<stddef.h>`). Unlike `NULL` (which may expand to integer `0`), `nullptr` never implicitly converts to an integer, preventing overload/variadic bugs and enabling clean `_Generic` dispatch.

=== "C23"

    ```c
    #include <stddef.h>
    #include <stdio.h>

    void log_tag(const char *tag, ...) {
        (void)tag;
    }

    int main(void) {
        int *p = nullptr;
        nullptr_t np = nullptr;
        // int x = nullptr; // Compile error! Cannot assign to int

        /* Safe in variadic calls — guaranteed pointer width, never 32-bit int 0 */
        log_tag("sentinel", "alpha", "beta", nullptr);
        printf("p is null: %s\n", p == np ? "true" : "false");
        return 0;
    }
    ```

=== "C17"

    ```c
    #include <stddef.h>
    #include <stdio.h>

    void log_tag(const char *tag, ...) {
        (void)tag;
    }

    int main(void) {
        int *p = NULL;
        int x = NULL; // Might compile silently if NULL is 0!
        (void)x;

        /* Must explicitly cast in variadic lists if NULL could be plain 0 */
        log_tag("sentinel", "alpha", "beta", (const char *)NULL);
        printf("p is null: %s\n", p == NULL ? "true" : "false");
        return 0;
    }
    ```

### 3.2 `constexpr` Object Definitions

`constexpr` declares true compile-time constant objects (scalars, structs, arrays) usable in integer constant expressions, array bounds, `case` labels, and `static_assert`, without macro hygiene hazards. *(Note: C23 supports `constexpr` **objects** only — not `constexpr` functions.)*

=== "C23"

    ```c
    constexpr size_t MAX_SLOTS = 64;
    constexpr double PI_APPROX = 3.14159265358979323846;

    struct Config {
        size_t slots;
        int retries;
    };
    constexpr struct Config DEFAULT_CFG = { .slots = MAX_SLOTS, .retries = 3 };

    static int table[DEFAULT_CFG.slots]; // Valid fixed-size array (NOT a VLA!)
    static_assert(DEFAULT_CFG.retries == 3);
    ```

=== "C17"

    ```c
    #define MAX_SLOTS 64u
    #define PI_APPROX 3.14159265358979323846

    /* const objects are NOT constant expressions in C17! */
    enum { DEFAULT_RETRIES = 3 }; // Traditional hack for int constants only

    static int table[MAX_SLOTS];
    _Static_assert(DEFAULT_RETRIES == 3, "retries must be 3");
    ```

### 3.3 `typeof` and `typeof_unqual`

`typeof(expr_or_type)` yields the exact type (including `const`/`volatile`/`_Atomic` qualifiers); `typeof_unqual(expr_or_type)` strips top-level qualifiers and `_Atomic`, enabling type-safe generic macros.

=== "C23"

    ```c
    #define SWAP(a, b)                                \
        do {                                          \
            typeof_unqual(a) _tmp = (a);              \
            (a) = (b);                                \
            (b) = _tmp;                               \
        } while (0)

    const int cx = 10;
    typeof_unqual(cx) mutable_copy = cx; // type is int, not const int
    mutable_copy += 5;
    ```

=== "C17"

    ```c
    /* Required non-standard GNU __typeof__ or explicit type argument */
    #define SWAP(T, a, b)     \
        do {                  \
            T _tmp = (a);     \
            (a) = (b);        \
            (b) = _tmp;       \
        } while (0)
    ```

### 3.4 `auto` Type Inference

`auto` deduces the type of a single initialized object from its initializer expression (repurposing the unused C89 storage-class keyword). Unlike C++, strict ISO C23 (`N3007`) permits **only a plain identifier declarator** (`auto ptr = &count;`) — writing `auto *ptr = &count;` is not valid ISO C23!

=== "C23"

    ```c
    #include <math.h>

    auto count = 42u;              // unsigned int
    const auto ratio = cos(0.5);   // const double
    auto ptr = &count;             // unsigned int * (do NOT write 'auto *ptr'!)
    ```

=== "C17"

    ```c
    #include <math.h>

    unsigned int count = 42u;
    const double ratio = cos(0.5);
    unsigned int *ptr = &count;
    ```

### 3.5 Promoted Core Keywords & Single-Argument `static_assert`

`bool`, `true`, `false`, `alignas`, `alignof`, `static_assert`, and `thread_local` are built-in keywords requiring no header. `static_assert(expr)` no longer requires a redundant message string.

=== "C23"

    ```c
    /* No <stdbool.h>, <stdalign.h>, <assert.h>, or <threads.h> required! */
    alignas(64) thread_local bool ready = false;
    static_assert(alignof(ready) == 64);
    ```

=== "C17"

    ```c
    #include <assert.h>
    #include <stdalign.h>
    #include <stdbool.h>
    #include <threads.h>

    alignas(64) thread_local bool ready = false;
    static_assert(alignof(ready) == 64, "ready must be cache-line aligned");
    ```

### 3.6 Empty Initializer `= {}` (Including VLAs)

`{}` zero-initializes any scalar, struct, union, fixed array, or **variable-length array (VLA)** cleanly without `{0}` nested-brace warnings (`-Wmissing-braces`).

=== "C23"

    ```c
    struct Packet { int id; char payload[32]; };

    struct Packet p = {};      // All members & padding zero-initialized
    int zero = {};             // 0
    void clear_vla(size_t n) {
        int scratch[n] = {};   // Valid in C23! Zeroes all n elements
        (void)scratch;
    }
    ```

=== "C17"

    ```c
    struct Packet { int id; char payload[32]; };

    struct Packet p = {0};     // Empty {} was a GNU extension, invalid ISO C17
    int zero = 0;
    void clear_vla(size_t n) {
        int scratch[n];        // VLAs could NOT have initializers in C17
        memset(scratch, 0, sizeof scratch);
    }
    ```

### 3.7 Binary Integer Literals (`0b`) & Digit Separators (`'`)

=== "C23"

    ```c
    #include <stdint.h>

    uint8_t mask = 0b1010'0110u;
    uint64_t hz  = 2'400'000'000ull;
    uint32_t hex = 0xDEAD'BEEFu;
    double pi    = 3.141'592'653'589;
    ```

=== "C17"

    ```c
    #include <stdint.h>

    uint8_t mask = 0xA6u;          // No 0b prefix in ISO C17
    uint64_t hz  = 2400000000ull;  // No digit separators
    uint32_t hex = 0xDEADBEEFu;
    double pi    = 3.141592653589;
    ```

### 3.8 Bit-Precise Integers `_BitInt(N)`

`_BitInt(N)` defines signed (minimum width 2) or `unsigned _BitInt(N)` (minimum width 1) integers of exact bit width `N` up to `BITINT_MAXWIDTH` (`<limits.h>`), with literal suffixes `wb`/`WB` and `uwb`/`UWB`. They do **not** undergo implicit integer promotion to `int`.

=== "C23"

    ```c
    #include <limits.h>

    unsigned _BitInt(24) rgb24 = 0xFF'88'00uwb;
    _BitInt(128) accumulator = 0wb;
    static_assert(BITINT_MAXWIDTH >= 64);
    ```

=== "C17"

    ```c
    #include <stdint.h>

    /* Required manual masking or non-portable __int128 extensions */
    uint32_t rgb24 = 0x00FF8800u & 0x00FFFFFFu;
    ```

### 3.9 IEEE 754-2019 Decimal Floating-Point (`_Decimal32`, `_Decimal64`, `_Decimal128`)

Optional (`__STDC_IEC_60559_DFP__`) base-10 floating-point types with suffixes `df`, `dd`, `dl` eliminate binary rounding error in financial and decimal-exact calculations.

=== "C23"

    ```c
    #ifdef __STDC_IEC_60559_DFP__
    #include <stdio.h>

    _Decimal64 price = 0.1dd;
    _Decimal64 total = price + 0.2dd; // Exactly 0.3dd!
    // Formatted with %Df / %DDf: printf("%.2Df\n", total);
    #endif
    ```

=== "C17"

    ```c
    #include <stdint.h>

    /* Required fixed-point integer cents or external libmpdec */
    int64_t price_cents = 10;
    int64_t total_cents = price_cents + 20;
    ```

### 3.10 Enhanced Enumerations (`enum : T` & Arbitrary-Width Enumerators)

Enumerations can now specify a **fixed underlying integer type** (fixing ABI size and signedness across compilers) and enumerators without a fixed type can exceed `int` range (e.g., 64-bit bitflags).

=== "C23"

    ```c
    #include <stdint.h>

    enum StatusCode : uint8_t {
        STATUS_OK = 0,
        STATUS_TIMEOUT = 1,
        STATUS_FATAL = 255
    };
    static_assert(sizeof(enum StatusCode) == 1);

    enum BigFlags : uint64_t {
        FLAG_HIGH_BIT = 1ULL << 62
    };
    ```

=== "C17"

    ```c
    #include <stdint.h>

    /* Enum size was implementation-defined; enumerators capped at int! */
    enum StatusCode { STATUS_OK = 0, STATUS_TIMEOUT = 1, STATUS_FATAL = 255 };
    typedef uint8_t status_code_t; // Separate typedef needed for wire ABI
    #define FLAG_HIGH_BIT (1ULL << 62)
    ```

### 3.11 Storage-Class Specifiers on Compound Literals

Compound literals can carry `static`, `constexpr`, `register`, or `thread_local` directly, avoiding repeated runtime stack construction and enabling file-scope or thread-local literals.

=== "C23"

    ```c
    const int *get_primes(void) {
        /* Lives in static read-only storage; safe to return address! */
        return (static constexpr int[]){ 2, 3, 5, 7, 11, 13 };
    }
    ```

=== "C17"

    ```c
    const int *get_primes(void) {
        /* Returning &(int[]){...} was a dangling stack pointer! */
        static const int primes[] = { 2, 3, 5, 7, 11, 13 };
        return primes;
    }
    ```

### 3.12 Labels Before Declarations & At End of Compound Statements

=== "C23"

    ```c
    void dispatch(int mode) {
        switch (mode) {
        case 1:
            int val = 42; // Direct declaration after label — legal in C23!
            (void)val;
            break;
        default:
            goto done;
        }
    done:
    } // Label right before closing '}' — no dummy ';' needed!
    ```

=== "C17"

    ```c
    void dispatch(int mode) {
        switch (mode) {
        case 1: {         // Extra braces or empty ';' required before decl
            int val = 42;
            (void)val;
            break;
        }
        default:
            goto done;
        }
    done:
        ; // Empty statement mandatory before '}' in C17!
    }
    ```

### 3.13 Unnamed Parameters in Function Definitions

=== "C23"

    ```c
    int on_timer_tick(int /*unused_fd*/, void *ctx) {
        return ctx != nullptr ? 0 : -1;
    }
    ```

=== "C17"

    ```c
    int on_timer_tick(int unused_fd, void *ctx) {
        (void)unused_fd; // Boilerplate required to silence -Wunused-parameter
        return ctx != NULL ? 0 : -1;
    }
    ```

### 3.14 Preprocessor Upgrades (`#embed`, `#elifdef`, `#elifndef`, `#warning`, `__VA_OPT__`)

=== "C23"

    ```c
    #if defined(__has_embed) && __has_embed("logo.bin")
    static const unsigned char LOGO_DATA[] = {
    #embed "logo.bin" limit(4096) suffix(, 0)
    };
    #endif

    #ifdef USE_FAST_PATH
        // fast path
    #elifdef USE_SAFE_PATH
        // safe path
    #elifndef DISABLE_FALLBACK
        #warning "Using default fallback implementation"
    #endif

    #define LOG(fmt, ...) fprintf(stderr, fmt "\n" __VA_OPT__(,) __VA_ARGS__)
    ```

=== "C17"

    ```c
    /* Required external xxd -i / Python build-step code generation */
    #include "generated_logo_data.h"

    #ifdef USE_FAST_PATH
        // fast path
    #elif defined(USE_SAFE_PATH)
        // safe path
    #elif !defined(DISABLE_FALLBACK)
        /* #warning was non-standard in C17 */
    #endif

    /* GNU ##__VA_ARGS__ extension needed for empty variadic invocations */
    #define LOG(fmt, ...) fprintf(stderr, fmt "\n", ##__VA_ARGS__)
    ```

### 3.15 All 7 Standard C23 `[[attributes]]`

C23 standardizes C++-compatible `[[...]]` attribute syntax and seven standard attributes (full semantics in [Attributes (C23)](../reference/language/attributes.md)):

| Attribute | Target | Purpose |
| :--- | :--- | :--- |
| `[[deprecated]]` / `[[deprecated("msg")]]` | Declarations, types, enumerators | Emit compiler diagnostic on use. |
| `[[fallthrough]]` | Null statement (`;`) inside `switch` | Declare intentional `case` fallthrough. |
| `[[maybe_unused]]` | Variables, params, functions, types | Suppress unused-entity compiler warnings. |
| `[[nodiscard]]` / `[[nodiscard("msg")]]` | Functions, `struct`/`union`/`enum` | Warn if caller ignores return value. |
| `[[noreturn]]` | Function declarations | Replaces deprecated `_Noreturn`; UB if function returns. |
| `[[reproducible]]` | Function types | Function is *effectless* and *idempotent* (same inputs + observable state $\Rightarrow$ same outputs). |
| `[[unsequenced]]` | Function types | Stricter than `[[reproducible]]`: *stateless* and *independent* (pure math/value function; freely hoistable/CSE-able). |

=== "C23"

    ```c
    [[nodiscard("must free handle")]]
    void *acquire_handle([[maybe_unused]] int flags);

    [[noreturn]] void fatal_abort(const char *msg);

    [[unsequenced]] int square(int x) { return x * x; }
    ```

=== "C17"

    ```c
    #include <stdnoreturn.h>

    /* Compiler-specific extensions required */
    __attribute__((warn_unused_result))
    void *acquire_handle(__attribute__((unused)) int flags);

    noreturn void fatal_abort(const char *msg);

    __attribute__((const)) int square(int x) { return x * x; }
    ```

### 3.16 Unicode Identifiers (UAX #31), Identical `cvr`-Qualified Arrays & Tag Compatibility

- **UAX #31 Identifiers**: Source identifiers follow Unicode Standard Annex #31 (`XID_Start` / `XID_Continue`), eliminating homoglyph/control-character ambiguity and matching C++23.
- **Identical `cvr`-Qualified Array Types**: Applying `const`, `volatile`, or `restrict` to an array type applies cleanly to its element type without creating distinct incompatible types across typedefs and `typeof`.
- **Enhanced Tag Compatibility**: Two identical `struct`, `union`, or `enum` definitions with the same tag and matching members are now compatible **within the same translation unit** (vital for header-generated macro types and `typeof`).

### 3.17 Relaxed Variadic Functions (`f(...)` & Single-Arg `va_start`)

=== "C23"

    ```c
    #include <stdarg.h>

    /* No named parameter required before '...' */
    int sum_terminated(...) {
        va_list ap;
        va_start(ap); // Second parameter is now optional!
        int total = 0;
        for (int v = va_arg(ap, int); v != 0; v = va_arg(ap, int)) {
            total += v;
        }
        va_end(ap);
        return total;
    }
    ```

=== "C17"

    ```c
    #include <stdarg.h>

    /* Required at least one named parameter before '...' */
    int sum_terminated(int first, ...) {
        va_list ap;
        va_start(ap, first); // Named last parameter mandatory
        int total = first;
        for (int v = va_arg(ap, int); v != 0; v = va_arg(ap, int)) {
            total += v;
        }
        va_end(ap);
        return total;
    }
    ```

---

## 4. New C23 Standard Library Features Checklist <span class="c-badge c23">C23</span>

| Feature / Header | Key Symbols | Description & Benefit |
| :--- | :--- | :--- |
| **`<stdbit.h>`** *(new header)* | `__STDC_ENDIAN_NATIVE__`, `stdc_leading_zeros`, `stdc_trailing_zeros`, `stdc_count_ones`, `stdc_bit_width`, `stdc_has_single_bit`, `stdc_bit_ceil`, `stdc_bit_floor` | 14 type-generic + typed bit-manipulation functions and portable endianness macros replacing `__builtin_clz`/`__builtin_popcount`. See [Bit Manipulation & Checked Int](../reference/library/numerics-bit-checked.md). |
| **`<stdckdint.h>`** *(new header)* | `ckd_add(&res, a, b)`, `ckd_sub(&res, a, b)`, `ckd_mul(&res, a, b)` | Overflow-checked arithmetic macros returning `bool` (`true` on wrap-around overflow) with defined wrapped output. |
| **Explicit zeroing** (`<string.h>`) | `memset_explicit(dest, ch, count)` | Guaranteed not to be removed by dead-store elimination (DSE) — use for zeroing keys, tokens, and passwords. |
| **POSIX string adoption** (`<string.h>`) | `memccpy`, `strdup`, `strndup` | Standardized heap string duplication and delimiter-bounded byte copy. |
| **Thread-safe time** (`<time.h>`) | `gmtime_r`, `localtime_r`, `timegm`, `timespec_getres` | Reentrant UTC/local conversions, UTC inverse `timegm`, and clock resolution query. |
| **UTF-8 support** (`<uchar.h>`) | `char8_t`, `mbrtoc8`, `c8rtomb` | Dedicated UTF-8 character type (`unsigned char`) and restartable conversion functions. |
| **Locale-safe float formatting** (`<stdlib.h>`) | `strfromd`, `strfromf`, `strfroml`, `strfromd32`/`64`/`128` | Lightweight floating-point-to-string formatting without pulling in all of `snprintf`. |
| **Sized deallocation & alignment** (`<stdlib.h>`) | `free_sized`, `free_aligned_sized`, `memalignment` | Lets modern allocators avoid metadata lookups on `free`; `memalignment(ptr)` returns pointer alignment power-of-2. |
| **Unreachable hint** (`<stddef.h>`) | `unreachable()` | Invokes UB if executed; replaces `__builtin_unreachable()` / `__assume(0)` for optimizer hints. |
| **Extended `printf` / `scanf` specifiers** (`<stdio.h>`, `<inttypes.h>`) | `%b`/`%B`, `%wN`/`%wfN`, `%H`/`%D`/`%DD`, `PRIbN`/`SCNbN` | Binary integer formatting (`%b`), exact-width length modifiers (`%w32d`, `%wf64u`), and decimal float modifiers (`%H`, `%D`, `%DD`). |
| **Const-preserving search** (`<string.h>`, `<wchar.h>`, `<stdlib.h>`) | `QChar` / `QVoid` generics: `strchr`, `memchr`, `strstr`, `strpbrk`, `strrchr`, `bsearch`, `wcs*` | Return `const T *` when passed `const T *` and `T *` when passed `T *`. |
| **Expanded freestanding library** | `<stdbit.h>`, `<stdckdint.h>`, partial `<string.h>`, `<stdlib.h>` (`abs`, `strtol`), `<stddef.h>` (`unreachable`) | Dramatically richer guaranteed library surface for kernels, bootloaders, and embedded firmware (`__STDC_HOSTED__ == 0`). |
| **Header version macros** | `__STDC_VERSION_STDINT_H__`, `__STDC_VERSION_STDBIT_H__`, `__STDC_VERSION_STDLIB_H__`, etc. (`202311L`) | Fine-grained per-header C23 feature detection when compiler and libc upgrade independently. |

---

## 5. Step-by-Step C23 Migration Audit Playbook

Follow this low-risk rollout sequence to upgrade an existing C11/C17 codebase to C23 without breaking production builds:

### Step 1 — Pre-Migration Audit Under C17

Turn on compatibility warnings **while still compiling in C17 mode** to flush out future keyword collisions, K&R prototypes, trigraphs, and deprecated calls:

```bash
# GCC / Clang C17 pre-flight audit
gcc -std=c17 -Wall -Wextra -Wpedantic \
    -Wc23-compat \
    -Wstrict-prototypes \
    -Wold-style-definition \
    -Wtrigraphs \
    -Wdeprecated-declarations \
    -Wdiscarded-qualifiers \
    -c src/*.c
```

### Step 2 — Grep for Dangerous Runtime & Macro Patterns

```bash
# 1. Audit every realloc() call for potential size == 0 UB
grep -rn "realloc(" src/ include/

# 2. Find preprocessor checks on newly promoted keywords
grep -rnE "#\s*(if|ifdef|ifndef).*\b(static_assert|thread_local|alignas|alignof|bool|true|false)\b" src/ include/

# 3. Locate deprecated headers and unsafe time functions
grep -rnE "(stdnoreturn\.h|_Noreturn|\basctime\s*\(|\bctime\s*\(|ATOMIC_VAR_INIT)" src/ include/
```

### Step 3 — Enable C23 in Build Systems

=== "CMake (3.21+)"

    ```cmake
    set(CMAKE_C_STANDARD 23)
    set(CMAKE_C_STANDARD_REQUIRED ON)
    set(CMAKE_C_EXTENSIONS OFF)

    target_compile_options(my_target PRIVATE
        $<$<C_COMPILER_ID:GNU,Clang,AppleClang>:
            -Wall -Wextra -Wpedantic -Wstrict-prototypes -Wdeprecated-declarations
        >
    )
    ```

=== "Makefile"

    ```makefile
    CC      := gcc
    CFLAGS  := -std=c23 -O2 -Wall -Wextra -Wpedantic \
               -Wstrict-prototypes -Wdeprecated-declarations \
               -fsanitize=address,undefined
    ```

### Step 4 — Verify with a Complete Idiomatic C23 Smoke Program

```c
/* Compile with: gcc -std=c23 -Wall -Wextra -Wpedantic c23_smoke.c -o c23_smoke */
#include <stdckdint.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum AllocKind : uint8_t {
    ALLOC_STACK = 0,
    ALLOC_HEAP  = 1,
};

constexpr size_t SECRET_BYTES = 32;

[[nodiscard]] static void *safe_realloc(void *ptr, size_t new_size) {
    if (new_size == 0) {
        free(ptr);
        return nullptr;
    }
    return realloc(ptr, new_size);
}

[[unsequenced]] static uint64_t next_bucket_bytes(uint64_t count, uint64_t elem_size) {
    uint64_t total = 0;
    if (ckd_mul(&total, count, elem_size)) {
        return 0; // Overflow safely detected!
    }
    return total;
}

int main(void) {
    static_assert(sizeof(enum AllocKind) == 1);

    uint8_t secret[SECRET_BYTES] = {}; // Empty C23 zero-init
    secret[0] = 0b1010'0101u;

    auto bytes = next_bucket_bytes(1'024u, 64u);
    auto buf = (unsigned char *)safe_realloc(nullptr, (size_t)bytes);
    if (buf == nullptr) {
        return EXIT_FAILURE;
    }

    const char *msg = "c23:ready";
    const char *colon = strchr(msg, ':'); // Preserves const char * in C23!
    printf("Flag: 0b%08b | Alloc: %zu bytes | Suffix: %s\n",
           secret[0], (size_t)bytes, colon != nullptr ? colon + 1 : "none");

    memset_explicit(secret, 0, sizeof secret);
    buf = (unsigned char *)safe_realloc(buf, 0); // Safely frees & returns nullptr
    return buf == nullptr ? EXIT_SUCCESS : EXIT_FAILURE;
}
```

---

## Related References

- [ISO C Standards History & Evolution](index.md)
- [Compiler & Libc Support Matrix](compiler-support.md)
- [Keywords (C89–C23)](../reference/language/keywords.md) · [Attributes (C23)](../reference/language/attributes.md) · [Preprocessor & `#embed`](../reference/language/preprocessor.md)
- [Bit Manipulation & Checked Arithmetic (`<stdbit.h>`, `<stdckdint.h>`)](../reference/library/numerics-bit-checked.md)
- [cppreference.com — C23 Standard Overview](https://en.cppreference.com/w/c/23)
