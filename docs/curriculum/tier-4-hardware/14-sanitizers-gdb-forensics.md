---
title: "Module 14: C Safety Sanitizers (ASan, UBSan, TSan) & GDB Core Dump Forensics"
description: "ASan, UBSan, TSan, shadow memory, core dumps, and GDB post-mortem forensics."
icon: lucide/bug
---

# Module 14: C Safety Sanitizers (ASan, UBSan, TSan) & GDB Core Dump Forensics

!!! info "C23 Reference Companion"
    Pair this deep-dive curriculum module with the live **ISO C23 (`en.cppreference.com/c`) Reference**:

    - [Undefined Behavior & As-If Rule](../../reference/language/basic-concepts.md)
    - [Technical Specs (TS 17961, TR 24772-3, TS 6010)](../../standards/technical-specifications.md)

**Track:** C Systems Programming, Memory Engineering & High-Performance Computing
**Category:** Dynamic Memory Sanitizers, Shadow Memory Architecture, Core Dumps & GDB Forensics
**Standard Identifier:** `DOC-STD-UNIVERSAL-2026`
**Status:** ✅ Completed

---

## 📑 Table of Contents

1. [High-Level Overview & Executive Summary](#1-high-level-overview-executive-summary)
2. [AddressSanitizer (ASan) Architecture & Shadow Memory Mechanics](#2-addresssanitizer-asan-architecture-shadow-memory-mechanics)
3. [UndefinedBehaviorSanitizer (UBSan) & Undefined Behavior Traps](#3-undefinedbehaviorsanitizer-ubsan-undefined-behavior-traps)
4. [ThreadSanitizer (TSan) & Vector Clock Data Race Detection](#4-threadsanitizer-tsan-vector-clock-data-race-detection)
5. [Linux Core Dump Generation & System Configuration](#5-linux-core-dump-generation-system-configuration)
6. [GDB (GNU Debugger) Post-Mortem Crash Forensics](#6-gdb-gnu-debugger-post-mortem-crash-forensics)
7. [Certification & Engineering Essentials (MISRA-C / CERT-C Cheat Sheet)](#7-certification-engineering-essentials-misra-c-cert-c-cheat-sheet)
8. [Comparative Analysis Matrix: Dynamic Analysis & Sanitizer Tools](#8-comparative-analysis-matrix-dynamic-analysis-sanitizer-tools)
9. [Performance & Hardware Resource Optimization](#9-performance-hardware-resource-optimization)
10. [Step-by-Step Production Lab: Automated Sanitizer Suite & GDB Crash Triage](#10-step-by-step-production-lab-automated-sanitizer-suite-gdb-crash-triage)
11. [Pure CLI / Command Interface](#11-pure-cli-command-interface)
12. [Advanced Architecture & Edge-Case Failure Modes](#12-advanced-architecture-edge-case-failure-modes)
13. [Detailed Sub-Components & Subsystems](#13-detailed-sub-components-subsystems)
14. [References (The 5+5 Rule)](#14-references-the-55-rule)
15. [Universal FinOps & Hardware Cost Governance](#15-universal-finops-hardware-cost-governance)

---

## 1. High-Level Overview & Executive Summary

Developing zero-defect, mission-critical systems in C requires deploying automated **Dynamic Analysis Sanitizers** inside every Continuous Integration (CI) test pipeline and mastering **Post-Mortem Linux Core Dump Forensics**.

Rather than relying on non-deterministic black-box testing, modern compiler suites (Clang/LLVM and GCC) inject runtime instrumentation directly into compiled binaries:

1. **AddressSanitizer (ASan)**: Intercepts all memory loads/stores using a **Shadow Memory Byte Map** to catch Heap/Stack Buffer Overflows, Use-After-Free (UAF), and Double-Free errors at the exact instruction of occurrence.
2. **UndefinedBehaviorSanitizer (UBSan)**: Traps signed integer overflows, misaligned pointer dereferences, null pointer access, and out-of-bounds bit shifts.
3. **ThreadSanitizer (TSan)**: Monitors multi-threaded memory access patterns via vector clocks to catch unsynchronized data races.
4. **GDB Core Dump Forensics**: Extracts backtraces, register contents (`%rip`, `%rsp`, `%rax`), and local stack variable states from Linux kernel crash dumps.

```text
┌────────────────────────────────────────────────────────────────────────────────┐
│               ADDRESSSANITIZER (ASan) SHADOW MEMORY ARCHITECTURE               │
├────────────────────────────────────────────────────────────────────────────────┤
│ Virtual Memory Address Space ($2^{48}$ on x86-64):                             │
│ ┌────────────────────────────────────────────────────────────────────────────┐ │
│ │ APPLICATION MEMORY (0x000000000000 .. 0x7FFFFFFFFFFF)                      │ │
│ │ [ Byte 0 ][ Byte 1 ][ Byte 2 ][ Byte 3 ][ Byte 4 ][ Byte 5 ][ Byte 6 ][ 7 ] │ │
│ └──────────────────────────────────────┬─────────────────────────────────────┘ │
│                                        │                                       │
│                                        ▼ Direct Bitwise Scale & Offset Mapping:│
│                                 `Shadow = (App_Addr >> 3) + 0x7FFF8000`        │
│                                        │                                       │
│ ┌──────────────────────────────────────┴─────────────────────────────────────┐ │
│ │ ASAN SHADOW MEMORY (1 Byte of Shadow describes 8 Bytes of App Memory):      │ │
│ │ - `0x00`: All 8 bytes are addressable and safe                               │ │
│ │ - `1..7`: First $k$ bytes are addressable; remainder unaddressable           │ │
│ │ - `0xFD`: Heap Freed Region (Use-After-Free Trap!)                           │ │
│ │ - `0xFA`: Heap Left Redzone (Buffer Overflow Trap!)                          │ │
│ │ - `0xF1`: Stack Left Redzone (Stack Buffer Overflow Trap!)                   │ │
│ └────────────────────────────────────────────────────────────────────────────┘ │
└────────────────────────────────────────────────────────────────────────────────┘
```

### 👔 Executive Summary (For Managers & Non-Technical Stakeholders)

* **Business Purpose**: Guarantees software resilience and cybersecurity compliance by catching 100% of memory corruption vulnerabilities and software bugs before software ever reaches production.
* **How It Works**: Uses advanced compiler security scanners to instrument test builds, instantly failing automated CI pipelines if a developer introduces a memory leak, race condition, or buffer overflow.
* **Key Business Value & ROI**: Prevents multi-million-dollar cybersecurity data breach liabilities, slashes production crash incidents to zero, and cuts developer debugging triage time by 80%.

---

## 2. AddressSanitizer (ASan) Architecture & Shadow Memory Mechanics

AddressSanitizer maps every 8 bytes of application memory to **1 byte of Shadow Memory**:

$$\text{Shadow Address} = \left(\frac{\text{App Address}}{8}\right) + \text{Offset}$$

```text
┌────────────────────────────────────────────────────────────────────────────────┐
│                     ASAN SHADOW POISON VALUES & MEANINGS                       │
├───────────────────┬────────────────────────────────────────────────────────────┤
│ Shadow Byte Value │ Memory State & Security Trap Meaning                       │
├───────────────────┼────────────────────────────────────────────────────────────┤
│ **`0x00`**        │ **Clean / Safe**: All 8 bytes are fully addressable.       │
├───────────────────┼────────────────────────────────────────────────────────────┤
│ **`0xFA`**        │ **Heap Redzone**: Memory padding surrounding heap chunk;   │
│                   │ accessing triggers **Heap Buffer Overflow** alert!         │
├───────────────────┼────────────────────────────────────────────────────────────┤
│ **`0xFD`**        │ **Heap Freed Memory**: Memory previously passed to `free()`;│
│                   │ accessing triggers **Use-After-Free (UAF)** alert!         │
├───────────────────┼────────────────────────────────────────────────────────────┤
│ **`0xF1` / `0xF3`**│ **Stack Redzone**: Memory padding between stack variables; │
│                   │ accessing triggers **Stack Buffer Overflow** alert!        │
├───────────────────┼────────────────────────────────────────────────────────────┤
│ **`0xF8`**        │ **Global Redzone**: Padding between global static vars.    │
└───────────────────┴────────────────────────────────────────────────────────────┘
```

---

## 3. UndefinedBehaviorSanitizer (UBSan) & Undefined Behavior Traps

UndefinedBehaviorSanitizer instruments source code to abort execution upon detecting ISO C Undefined Behavior:

* **Signed Integer Overflow**: `int32_t c = a + b;` where $a+b > \text{INT32\_MAX}$.
* **Null Pointer Dereference**: Accessing `*ptr` when `ptr == NULL`.
* **Misaligned Pointer Access**: Accessing `uint32_t *` at an odd memory address.
* **Invalid Bit Shifts**: `x << 32` on a 32-bit integer, or shifting by a negative value.

---

## 4. ThreadSanitizer (TSan) & Vector Clock Data Race Detection

ThreadSanitizer instruments every memory read, memory write, and synchronization call (`pthread_mutex_lock`, atomic operations) to maintain internal **Vector Clocks**. If two threads access the same memory location concurrently with at least one write and **no happens-before synchronization relationship**, TSan halts execution and prints the exact call stacks of both conflicting threads!

---

## 5. Linux Core Dump Generation & System Configuration

When a Linux process crashes from a fatal signal (`SIGSEGV`, `SIGBUS`, `SIGABRT`), the OS kernel writes a snapshot of the process address space to disk as a **Core Dump**.

### Linux Kernel Core Dump Setup

```bash
# Enable unlimited core dump size
ulimit -c unlimited

# Configure kernel core dump pattern to /tmp/core.`<program>`.<pid>
sudo sysctl -w kernel.core_pattern="/tmp/core.%e.%p"
```

---

## 6. GDB (GNU Debugger) Post-Mortem Crash Forensics

```text
┌────────────────────────────────────────────────────────────────────────────────┐
│               GDB POST-MORTEM FORENSIC TRIAGE COMMAND CHEAT SHEET              │
├───────────────────┬────────────────────────────────────────────────────────────┤
│ GDB Command       │ Action & Forensic Output                                   │
├───────────────────┼────────────────────────────────────────────────────────────┤
│ `bt full`         │ Prints complete backtrace with all local variable values.  │
├───────────────────┼────────────────────────────────────────────────────────────┤
│ `info registers`  │ Inspects CPU hardware registers (`%rip`, `%rsp`, `%rax`).  │
├───────────────────┼────────────────────────────────────────────────────────────┤
│ `thread apply all`│ Prints call backtraces for **all threads** in the process. │
│ `bt`              │                                                            │
├───────────────────┼────────────────────────────────────────────────────────────┤
│ `x/16gx $rsp`     │ Examines 16 64-bit quadwords directly from call stack.     │
├───────────────────┼────────────────────────────────────────────────────────────┤
│ `disassemble /m`  │ Displays machine assembly interleaved with C source lines. │
└───────────────────┴────────────────────────────────────────────────────────────┘
```

---

## 7. Certification & Engineering Essentials (MISRA-C / CERT-C Cheat Sheet)

* ⚠️ **CERT-C MSC41-C**: Never compile production or test code without automated dynamic memory verification tools.
* 🔒 **CERT-C MEM30-C**: Do not access freed memory (Enforce ASan checks in all unit and integration test suites).
* ⚙️ **The ASan Flag**: Always pass `-fsanitize=address,undefined -fno-omit-frame-pointer -g` in test configurations.
* ⚠️ **TSan Mutually Exclusive**: Never combine `-fsanitize=address` and `-fsanitize=thread` in the same build (They use incompatible shadow memory models!).

---

## 8. Comparative Analysis Matrix: Dynamic Analysis & Sanitizer Tools

| Tool | CPU Overhead | Memory Overhead | Detection Domain | False Positives |
| :--- | :--- | :--- | :--- | :--- |
| **AddressSanitizer (ASan)** | **$2\times$ (Fast!)** | **$2\times$** | **Buffer Overflow, UAF, Leaks** | **0% (Deterministic)** |
| **UBSan** | **$1.2\times$ (Minimal)** | **0%** | **Undefined Behavior, Overflow** | **0%** |
| **ThreadSanitizer (TSan)** | $5\times$ | $5\times$ | **Multi-Thread Data Races** | **0%** |
| **Valgrind (Memcheck)** | $20\times$ (Very Slow) | $3\times$ | Uninstrumented binaries | Very Low |

---

## 9. Performance & Hardware Resource Optimization

```text
┌────────────────────────────────────────────────────────────────────────────────┐
│                         SANITIZER TUNING PLAYBOOK                              │
├────────────────────────────────────────────────────────────────────────────────┤
│ 1. Run unit test suites with `-fsanitize=address,undefined` on every git push. │
│ 2. Run concurrency integration suites with `-fsanitize=thread` separately.     │
│ 3. Always preserve frame pointers (`-fno-omit-frame-pointer`) for clean stacks.│
│ 4. Configure `ASAN_OPTIONS=detect_leaks=1:abort_on_error=1` in test runner.    │
│ 5. Configure automated core dump collection in production Kubernetes nodes.    │
└────────────────────────────────────────────────────────────────────────────────┘
```

---

## 10. Step-by-Step Production Lab: Automated Sanitizer Suite & GDB Crash Triage

### File Structure

* `src/forensic_demo.c`
* `scripts/gdb_triage.sh`

### Step 1: Author Memory Fault Trigger Application

```c
// src/forensic_demo.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct {
    uint64_t account_id;
    char username[16];
} UserRecord;

void trigger_use_after_free(void) {
    UserRecord *user = (UserRecord *)malloc(sizeof(UserRecord));
    if (!user) return;

    user->account_id = 9988;
    strncpy(user->username, "alice_admin", sizeof(user->username) - 1);

    // Free memory
    free(user);

    // Intentionally trigger Use-After-Free for Sanitizer / GDB demo
    #ifdef TRIGGER_BUG
    printf("Accessing freed user: %lu (%s)\n", user->account_id, user->username);
    #endif
}

int main(int argc, char **argv) {
    printf("Running Forensic Verification Engine...\n");
    trigger_use_after_free();
    (void)argc;
    (void)argv;
    return 0;
}
```

---

### Step 2: Author Automated GDB Forensic Triage Shell Script

```bash
#!/usr/bin/env bash
# scripts/gdb_triage.sh
set -euo pipefail

BIN_TARGET="bin/forensic_demo"
CORE_FILE="${1:-}"

if [[ -z "${CORE_FILE}" || ! -f "${CORE_FILE}" ]]; then
    echo "Usage: $0 <path-to-core-dump>"
    exit 1
fi

echo "=== AUTOMATED GDB POST-MORTEM FORENSIC REPORT ==="
gdb --batch \
    -ex "file ${BIN_TARGET}" \
    -ex "core-file ${CORE_FILE}" \
    -ex "echo \n--- CPU REGISTERS ---\n" \
    -ex "info registers" \
    -ex "echo \n--- THREAD BACKTRACES ---\n" \
    -ex "thread apply all bt full" \
    -ex "echo \n--- FAULTING INSTRUCTION ---\n" \
    -ex "disassemble /m" \
    -ex "quit"
```

---

## 11. Pure CLI / Command Interface

### 1. Compile with AddressSanitizer & LeakSanitizer

Compile with full runtime instrumentation:

```bash
gcc -std=c17 -Wall -Wextra -Werror -Wpedantic \
    -g -O1 -DTRIGGER_BUG \
    -fsanitize=address,undefined -fno-omit-frame-pointer \
    -o bin/forensic_demo \
    src/forensic_demo.c
```

### 2. Run Binary and Verify AddressSanitizer Heap-Use-After-Free Trap

Execute instrumented binary:

```bash
./bin/forensic_demo 2>&1 | head -n 30 || true
```

### 3. Compile Production Hardened Binary with Debug Symbols

Build optimized binary with debug symbols for production core dump analysis:

```bash
gcc -std=c17 -Wall -Wextra -O2 -g \
    -fno-omit-frame-pointer -fstack-protector-strong \
    -o bin/forensic_prod \
    src/forensic_demo.c
```

---

## 12. Advanced Architecture & Edge-Case Failure Modes

```text
┌────────────────────────────────────────────────────────────────────────────────┐
│                    SANITIZER FAILURE RECOVERY MATRIX                           │
├──────────────────────┬────────────────────────┬────────────────────────────────┤
│ Failure Scenario     │ Underlying Root Cause  │ Production Mitigation Runbook  │
├──────────────────────┼────────────────────────┼────────────────────────────────┤
│ **`ASan Heap UAF`**  │ Read memory via        │ Zero pointer upon `free()`:    │
│ **`Detected (Abort)`**| dangling pointer.      │ `#define SAFE_FREE(p) ...`     │
├──────────────────────┼────────────────────────┼────────────────────────────────┤
│ **`UBSan Integer`**  │ Mathematical sum       │ Use exact-width types and      │
│ **`Overflow Trap`**  │ exceeded signed bounds.│ `__builtin_add_overflow()`.    │
├──────────────────────┼────────────────────────┼────────────────────────────────┤
│ **`TSan Data Race`** │ Unsynchronized shared  │ Enclose variable in mutex or   │
│ **`Violation`**      │ memory read/write.     │ migrate to C11 atomic types.   │
├──────────────────────┼────────────────────────┼────────────────────────────────┤
│ **`Core Dump Trunc`**| `ulimit -c` capped     │ Set `ulimit -c unlimited` in   │
│ **`on Disk Full`**   │ maximum dump filesize. │ daemon systemd service unit.   │
└──────────────────────┴────────────────────────┴────────────────────────────────┘
```

---

## 13. Detailed Sub-Components & Subsystems

### 1. ASan Redzone Allocator Interceptor

* **Key Concepts**: Wraps `malloc()` calls to allocate extra 32-byte redzone buffers before and after every allocated block, poisoning them with `0xFA`.
* **CLI / Tool Snippet**:

```bash
ASAN_OPTIONS="help=1" ./bin/forensic_demo 2>&1 | head -n 20 || true
```

### 2. Linux Kernel `/proc/sys/kernel/core_pattern` Pipe Subsystem

* **Key Concepts**: Directs kernel core dumps directly into system diagnostic daemons (`systemd-coredump`) for automatic S3 artifact uploads.
* **CLI / Tool Snippet**:

```bash
cat /proc/sys/kernel/core_pattern 2>/dev/null || true
```

### 3. GDB Batch Mode Automation Engine (`gdb --batch`)

* **Key Concepts**: Non-interactive command runner allowing CI/CD scripts to extract backtraces and registers without opening an interactive terminal.
* **CLI / Tool Snippet**:

```bash
gdb --version | head -n 1
```

### 4. DWARF Debug Information Parser (`libdw` / `dwarfdump`)

* **Key Concepts**: Parses `.debug_info` and `.debug_line` ELF sections to map machine instruction addresses back to exact C source file line numbers.
* **CLI / Tool Snippet**:

```bash
objdump --dwarf=info bin/forensic_prod 2>/dev/null | head -n 30 || true
```

---

## 14. References (The 5+5 Rule)

### Official Documentation & Academic Specifications

1. [LLVM Clang AddressSanitizer (ASan) Architecture Manual](https://clang.llvm.org/docs/AddressSanitizer.html)
2. [LLVM UndefinedBehaviorSanitizer (UBSan) Specification](https://clang.llvm.org/docs/UndefinedBehaviorSanitizer.html)
3. [LLVM ThreadSanitizer (TSan) Data Race Manual](https://clang.llvm.org/docs/ThreadSanitizer.html)
4. [GDB (GNU Debugger) Official Reference Guide](https://sourceware.org/gdb/current/onlinedocs/gdb/)
5. [SEI CERT C Coding Standard: Dynamic Analysis Rules](https://wiki.sei.cmu.edu/)

### Authoritative Engineering Textbooks & Systems Deep Dives

1. [Konstantin Serebryany et al.: AddressSanitizer: A Fast Address Sanity Checker (USENIX ATC)](https://www.usenix.org/conference/atc12/technical-sessions/presentation/serebryany)
2. [Julia Evans: GDB Quick Reference and Linux Core Dump Debugging](https://jvns.ca/)
3. [Eli Bendersky: How AddressSanitizer Works: Shadow Memory Deep Dive](https://eli.thegreenplace.net/)
4. [Brendan Gregg: BPF Performance Tools: Profiling and Core Dumps](https://www.brendangregg.com/)
5. [Google Security Engineering: Eliminating Memory Safety Vulnerabilities with Dynamic Sanitizers](https://security.googleblog.com/)

---

## 15. Universal FinOps & Hardware Cost Governance

```text
┌────────────────────────────────────────────────────────────────────────────────┐
│                        SAFETY FINOPS SAVINGS MATRIX                            │
├──────────────────────────┬──────────────────────────┬──────────────────────────┤
│ Optimization Strategy    │ Technical Mechanism      │ Measurable FinOps ROI    │
├──────────────────────────┼──────────────────────────┼──────────────────────────┤
│ **Automated CI ASan**    │ Catches memory bugs      │ Eliminates \$4.4M average│
│                          │ before production deploy │ security breach incident │
├──────────────────────────┼──────────────────────────┼──────────────────────────┤
│ **GDB Core Automation**  │ Extracts instant stack   │ Cuts developer incident  │
│                          │ backtrace from crash dump│ triage time from 4h to 5m│
├──────────────────────────┼──────────────────────────┼──────────────────────────┤
│ **LeakSanitizer in CI**  │ Eliminates memory leaks  │ Prevents cloud server RAM│
│                          │ in automated test suites │ auto-scaling cost spikes │
├──────────────────────────┼──────────────────────────┼──────────────────────────┤
│ **TSan Data Race Zero**  │ Identifies race condition│ Eliminates catastrophic  │
│                          │ corrupting ledger states │ financial accounting bugs│
└──────────────────────────┴──────────────────────────┴──────────────────────────┘
```

### 1. Automated Sanitizer CI Testing vs Production Outage Economics

In an enterprise cloud software platform handling customer financial transactions:

* Undetected Use-After-Free and Heap Overflow vulnerabilities escaping into production trigger intermittent server crashes, corrupted account balances, and critical emergency security hotfixes ($~\$150,000\text{ engineering emergency labor} + \$500,000\text{ SLA credit penalties}$).
* Enforcing `-fsanitize=address,undefined` in CI catches 100% of memory errors on local pull requests before code is ever merged.
* **FinOps ROI**: Eliminates **\$650,000+ per incident in emergency remediation expenses**.

### 2. Automated GDB Core Dump Triage Labor Savings

* Manually reproducing complex race conditions without core dumps takes an average of 16 engineering hours per incident.
* Automated GDB core dump scripting extracts exact line numbers and variable values in 30 seconds.
* **FinOps ROI**: Saves **15.5 engineering hours per production incident ($~\$1,800 saved per incident)**.
