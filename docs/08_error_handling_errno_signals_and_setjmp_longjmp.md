# Module 08: Error Handling, <errno.h>, Signals & Nonlocal Jumps
**Domain:** errno, perror, strerror_r, assert.h, POSIX Signals & setjmp/longjmp
**Target Level:** Intermediate to Advanced Systems Developer
**Status:** ✅ Completed

---

## 1. High-Level Overview
In C, error handling is fundamentally explicit. Unlike languages with exception unwinding runtimes, C systems rely on:
1. **Return Codes & `<errno.h>`**: System calls and standard library functions return sentinel error values (e.g. `-1` or `NULL`) and set the thread-local global integer `errno` to indicate the specific error condition (`EINTR`, `EAGAIN`, `ENOMEM`, `ENOENT`).
2. **Thread-Safe Error Strings**: Using `strerror_r` (reentrant) rather than unsafe legacy `strerror` (non-reentrant static buffer).
3. **Runtime Assertions (`<assert.h>`)**: Verifying invariant preconditions during development, disabled in release builds via `#define NDEBUG`.
4. **POSIX Signals (`<signal.h>`)**: Handling asynchronous kernel interruptions (`SIGINT`, `SIGTERM`, `SIGPIPE`, `SIGSEGV`) via `sigaction`.
5. **Nonlocal Jumps (`<setjmp.h>`)**: Performing low-level context saves (`setjmp`) and non-local stack unwinding jumps (`longjmp`) for emergency panic recovery.

### 👔 Executive Summary (For Managers & Non-Technical Stakeholders)
* **Business Purpose**: Ensures that when unexpected system failures happen (like network disconnects, disk full errors, or invalid user actions), the software reports the problem clearly and recovers automatically without crashing.
* **How It Works**: Checks the success status of every single operation, translating low-level error codes into clear diagnostic logs and executing clean recovery runbooks.
* **Key Business Value & Use Cases**: Delivers 99.999% high-availability uptime, reduces customer support ticket resolution times, and simplifies site reliability operations.

---

## 2. POSIX Signal Trapping & Async-Signal-Safety

```
Kernel Signal Dispatch:
[ OS Kernel Event (e.g. Ctrl+C / SIGINT) ]
                    |
                    v
[ Interrupted Main Thread State Saved ]
                    |
                    v
[ Asynchronous Signal Handler Function (sigaction) ]
  (Strict Requirement: Must ONLY invoke Async-Signal-Safe Functions: write, _exit)
                    |
                    v
[ Set atomic volatile sig_atomic_t flag = 0 ]
                    |
                    v
[ Resume Main Thread -> Perform Clean Graceful Shutdown ]
```

---

## 3. Hands-On Walkthrough: Thread-Safe Error Reporting with `strerror_r`
### Step 1: Implement Robust File Opener with Detailed Error Logging
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int safe_open_file(const char *filepath) {
    int fd = open(filepath, O_RDONLY);
    if (fd == -1) {
        char err_buf[256];
        #if (_POSIX_C_SOURCE >= 200112L) && ! _GNU_SOURCE
        strerror_r(errno, err_buf, sizeof(err_buf));
        #else
        strerror_r(errno, err_buf, sizeof(err_buf));
        #endif
        fprintf(stderr, "Failed to open file '%s': [Error %d] %s\\n", filepath, errno, err_buf);
        return -1;
    }
    return fd;
}
```

---

## 4. Pure CLI Commands
### 1. Compile Error Handling Suite
Compile with POSIX standards enabled:
```bash
gcc -Wall -Wextra -O2 -D_POSIX_C_SOURCE=200809L \
    -o bin/error_demo \
    src/errors.c
```

---

## References

### Official Documentation
* [ISO C17 Standard Section 7.5 (<errno.h>)](https://www.iso.org/) - Error number specification.
* [K. N. King Chapter 24 (Error Handling)](http://knking.com/) - Comprehensive error patterns.
* [POSIX.1-2017 sigaction Specification](https://pubs.opengroup.org/) - Async-signal-safe standards.
* [SEI CERT C: Error Handling Rules (ERR30-C to ERR32-C)](https://wiki.sei.cmu.edu/) - Safe error checking.
* [Linux errno(3) Manual Page](https://man7.org/linux/man-pages/man3/errno.3.html) - Standard system error codes.

### Authoritative Web Pages, Blogs & Tutorials
* [Julia Evans: What Happens When a Signal Arrives?](https://jvns.ca/) - Illustrated signal dispatching.
* [Eli Bendersky: setjmp and longjmp in C](https://eli.thegreenplace.net/) - Non-local jumps and coroutines.
* [Red Hat Developer: Thread-Safe Error Reporting in C](https://developers.redhat.com/blog/) - strerror_r patterns.
* [Datadog Engineering: Tracking Error Rates and SIGPIPE Handling](https://www.datadoghq.com/blog/) - Production alerting.
* [FinOps Foundation: Incident Reduction and SRE Cost Governance](https://www.finops.org/) - Reducing operational overhead.

---

## FinOps & Resource Cost Governance in Systems Programming

*Financial Operations (FinOps) in systems programming focuses on maximizing CPU instruction efficiency, minimizing heap memory footprint, eliminating memory leaks, and optimizing server bin-packing.*

### 1. Compute & Memory Efficiency at Scale
- **Low Memory Footprint** – Writing optimized C services with sub-5MB RSS memory footprints allows deploying 20x-50x more application replicas per cloud virtual host compared to runtime-heavy languages (JVM, Python).
- **CPU Instruction Cache Optimization** – Structuring data layout (Structure of Arrays vs Array of Structures) to maximize CPU L1/L2 cache hit ratios drastically reduces CPU clock cycles per transaction, cutting required cloud vCPU core counts.

### 2. Eliminating Infrastructure Waste
- **Zero-Copy I/O** – Utilizing `mmap` and `sendfile` bypasses kernel-to-userspace data copying, slashing CPU utilization and lowering network processing latency on high-throughput cloud instances.
- **Memory Leak Prevention** – Continuous integration memory sanitization (AddressSanitizer and Valgrind) prevents gradual memory bloat that triggers artificial node scale-out and emergency restart outages.

### 3. Server Bin-Packing & Cloud Sizing
- **Right-Sizing Compute Fleets** – Precise predictability in C memory and CPU consumption allows engineering teams to size production containers tightly without arbitrary safety buffers.
- **Spot / Preemptible Fleet Execution** – Ultra-fast binary startup times (<2 milliseconds) make mission-critical C services ideal for low-cost spot and preemptible cloud instance fleets.
