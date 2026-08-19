# Mission-Critical C Systems Programming Encyclopedia — Master Index

**Repository:** `maxine/c_lang`
**Domain:** Low-Level Systems Programming, POSIX Interfaces & High-Performance Runtimes
**Target Level:** Zero to Enterprise Mission-Critical Lead Architect
**Status:** ✅ Complete 18-Module Production-Grade Encyclopedia

---

## 📚 Complete Encyclopedia Module Index (From Zero to Master)

| Module | Core Topics & Hands-On Scope | Target Level | Document Link |
| :--- | :--- | :--- | :--- |
| **00. Foundations & Syntax** | C11/C17 syntax, fixed-width types (`stdint.h`), storage classes (`static`, `extern`), deterministic control flow | Zero to Beginner | [`00_c_foundations_syntax_types_and_control_flow.md`](00_c_foundations_syntax_types_and_control_flow.md) |
| **01. Functions & Stack** | Call stack frames, activation records, pass-by-value/pointer, stack canaries, recursion limits | Beginner | [`01_functions_program_structure_and_scope.md`](01_functions_program_structure_and_scope.md) |
| **02. Arrays & Pointers** | Pointer decay, pointer arithmetic, multidimensional row-major matrices, pointer-to-pointer indirection | Intermediate | [`02_arrays_pointers_and_pointer_arithmetic.md`](02_arrays_pointers_and_pointer_arithmetic.md) |
| **03. Strings & Bounds** | Null-terminated byte arrays, `<string.h>`, bounds-safe parsing (`snprintf`, `strlcpy`), buffer overflow mitigation | Intermediate | [`03_strings_string_h_and_bounds_safety.md`](03_strings_string_h_and_bounds_safety.md) |
| **04. Structs & Unions** | Struct alignment, padding, struct packing `__attribute__((packed))`, bitfields, tagged variant unions | Intermediate | [`04_structures_unions_bitfields_and_enums.md`](04_structures_unions_bitfields_and_enums.md) |
| **05. Dynamic Memory** | Heap management, `malloc`/`calloc`/`realloc`/`free`, defensive realloc, double-free prevention, doubly linked lists | Core Systems | [`05_dynamic_memory_allocation_and_linked_structures.md`](05_dynamic_memory_allocation_and_linked_structures.md) |
| **06. Preprocessor** | Macro expansion, stringification `#`, token pasting `##`, idempotent header guards, conditional compilation | Core Systems | [`06_the_c_preprocessor_macros_and_header_design.md`](06_the_c_preprocessor_macros_and_header_design.md) |
| **07. Standard Library** | `<stdio.h>` stream buffering, formatted I/O, `<stdlib.h>` `qsort`/`bsearch`, monotonic clocks in `<time.h>`, math | Core Systems | [`07_standard_c_library_deep_dive_stdio_stdlib_time_math.md`](07_standard_c_library_deep_dive_stdio_stdlib_time_math.md) |
| **08. Error Handling** | `<errno.h>` error checking, reentrant `strerror_r`, `<assert.h>`, POSIX signals `sigaction`, `setjmp`/`longjmp` | Core Systems | [`08_error_handling_errno_signals_and_setjmp_longjmp.md`](08_error_handling_errno_signals_and_setjmp_longjmp.md) |
| **09. Toolchains & Make** | GCC/Clang 4-stage pipeline, assembly generation, linkers, Makefiles, CMake, security hardening flags | Systems Developer | [`09_toolchains_compilation_pipeline_and_makefiles.md`](09_toolchains_compilation_pipeline_and_makefiles.md) |
| **10. UNIX Syscalls** | POSIX system calls (`open/read/write/close/lseek`), file descriptor tables, `fcntl` advisory locks | Systems Developer | [`10_system_programming_unix_syscalls_and_file_descriptors.md`](10_system_programming_unix_syscalls_and_file_descriptors.md) |
| **11. I/O Multiplexing** | Scalable `epoll` / `kqueue` event demultiplexing, zero-copy `mmap`, `msync`, direct kernel `sendfile` | Systems Engineer | [`11_scalable_io_multiplexing_epoll_kqueue_and_mmap.md`](11_scalable_io_multiplexing_epoll_kqueue_and_mmap.md) |
| **12. Concurrency** | POSIX threads (`pthreads`), mutexes, rwlocks, condition variables, C11 `<stdatomic.h>`, memory ordering fences | Concurrency Specialist | [`12_concurrency_pthreads_synchronization_and_c11_atomics.md`](12_concurrency_pthreads_synchronization_and_c11_atomics.md) |
| **13. Network Sockets** | Berkeley sockets, non-blocking TCP server, `setsockopt` (`SO_REUSEPORT`, `TCP_NODELAY`), binary packet parsing | Network Engineer | [`13_network_programming_berkeley_sockets_and_tls.md`](13_network_programming_berkeley_sockets_and_tls.md) |
| **14. Safety & Sanitizers** | Defensive C, AddressSanitizer (ASan), UBSan, ThreadSanitizer (TSan), Valgrind, GDB core dump debugging | Mission-Critical SRE | [`14_safety_sanitizers_asan_ubsan_tsan_and_gdb_forensics.md`](14_safety_sanitizers_asan_ubsan_tsan_and_gdb_forensics.md) |
| **15. Embedded & Kernel** | `volatile` keyword, memory-mapped I/O (MMIO), async-signal-safe `sigaction`, inline assembly (`__asm__`) | Embedded / Kernel | [`15_embedded_systems_mmio_volatile_and_inline_assembly.md`](15_embedded_systems_mmio_volatile_and_inline_assembly.md) |
| **16. Performance & SIMD** | Hardware cache hierarchy, AVX2/AVX-512 & ARM NEON vectorization, branch prediction, Profile-Guided Optimization (PGO) | Performance Architect | [`16_hardware_optimization_simd_avx512_neon_and_pgo.md`](16_hardware_optimization_simd_avx512_neon_and_pgo.md) |
| **17. Enterprise Projects** | Capstone: High-Throughput Epoll Web Server, Custom Arena Allocator for HFT, Industrial MMIO IoT Daemon | Enterprise Lead Master | [`17_real_world_enterprise_case_studies_and_hands_on.md`](17_real_world_enterprise_case_studies_and_hands_on.md) |

---

## 🛠️ Documentation Standards Applied Across All Guides

1. **👔 Executive Summary**: Non-technical explanation of business purpose, mechanics, and value for managers and teammates.
2. **Technical Deep Dives**: Comprehensive architecture explanations, consensus mechanics, and kernel-level primitives.
3. **Hands-On Step-by-Step Walkthroughs**: Reproducible labs for building, scaling, securing, and debugging workloads.
4. **Clean, Escaped CLI Snippets**: Formatted with trailing `\` line escapes, 4-space indentation, and zero in-code comments.
5. **Trustworthy Curated Sources**: Exactly 5 official documentation links + 5 authoritative engineering blogs per module.
6. **FinOps & Resource Governance**: 500+ word guidelines on compute instruction efficiency and memory rightsizing.
