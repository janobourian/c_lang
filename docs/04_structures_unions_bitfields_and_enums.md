# Module 04: Structures, Unions, Bitfields & Enumerations
**Domain:** Struct Alignment, Padding, Bitfields, Tagged Unions & Memory-Mapped Structs
**Target Level:** Intermediate to Advanced Systems Developer
**Status:** ✅ Completed

---

## 1. High-Level Overview
In C, **Structures (`struct`)**, **Unions (`union`)**, **Bitfields**, and **Enumerations (`enum`)** allow developers to create custom composite data types that match application domain models and exact hardware register maps.

A critical systems concept is **Structure Memory Alignment & Padding**: CPU hardware architectures access memory on natural 4-byte or 8-byte boundaries. Compilers insert hidden padding bytes between fields to maintain alignment, which can inflate memory footprints unless fields are sorted in descending order of size. 

Unions allow multiple members to share the same physical memory space, enabling **Tagged Unions (Variant Types)** and low-level protocol parsing.

### 👔 Executive Summary (For Managers & Non-Technical Stakeholders)
* **Business Purpose**: Allows complex business data (like customer portfolios, telemetry packets, and financial transactions) to be structured cleanly inside computer memory.
* **How It Works**: Bundles multiple related data points into a single composite digital record, aligning fields so the computer hardware processes them instantly.
* **Key Business Value & Use Cases**: Cuts application memory waste by up to 40%, enables high-speed binary communication with external hardware, and enforces clean software architecture.

---

## 2. Struct Memory Padding & Packing Mechanics

```
Unoptimized Struct (24 Bytes due to padding):
+--------------------+--------------------+--------------------+--------------------+
| uint8_t flag (1B)  | Padding (7B)       | uint64_t id (8B)   | uint32_t val (4B)  |
+--------------------+--------------------+--------------------+--------------------+
| Padding (4B)       |
+--------------------+

Optimized Struct (16 Bytes - Sorted by descending size):
+--------------------+--------------------+--------------------+--------------------+
| uint64_t id (8B)   | uint32_t val (4B)  | uint8_t flag (1B)  | Padding (3B)       |
+--------------------+--------------------+--------------------+--------------------+
```

---

## 3. Hands-On Walkthrough: Tagged Union for Multi-Type Messages
### Step 1: Implement an Enterprise Tagged Union
```c
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

typedef enum {
    MSG_TYPE_HEARTBEAT,
    MSG_TYPE_TRANSACTION,
    MSG_TYPE_ALERT
} MessageType;

typedef struct {
    MessageType type;
    union {
        struct {
            uint64_t timestamp_ms;
        } heartbeat;

        struct {
            uint64_t account_id;
            int64_t amount_cents;
        } transaction;

        struct {
            uint32_t error_code;
            char message[32];
        } alert;
    } payload;
} NetworkMessage;

void dispatch_message(const NetworkMessage *msg) {
    if (!msg) return;

    switch (msg->type) {
        case MSG_TYPE_HEARTBEAT:
            printf("💓 Heartbeat: %lu ms\\n", msg->payload.heartbeat.timestamp_ms);
            break;
        case MSG_TYPE_TRANSACTION:
            printf("💳 Transaction: Acc %lu | Amount: $%ld.%02ld\\n", 
                   msg->payload.transaction.account_id,
                   msg->payload.transaction.amount_cents / 100,
                   msg->payload.transaction.amount_cents % 100);
            break;
        case MSG_TYPE_ALERT:
            printf("⚠️ Alert [%u]: %s\\n", msg->payload.alert.error_code, msg->payload.alert.message);
            break;
    }
}
```

---

## 4. Pure CLI Commands
### 1. Inspect Struct Size and Offsets
Compile and verify memory footprint:
```bash
gcc -Wall -Wextra -O2 \
    -o bin/struct_demo \
    src/structs.c
```

---

## References

### Official Documentation
* [ISO C17 Standard Section 6.7.2.1 (Structure and Union Specifiers)](https://www.iso.org/) - Standard struct rules.
* [K. N. King Chapter 16 (Structures, Unions, and Enumerations)](http://knking.com/) - Comprehensive guide.
* [SEI CERT C: Structure Padding Security Guidelines](https://wiki.sei.cmu.edu/) - Information leak prevention.
* [GCC Type Attributes: `__attribute__((packed))`](https://gcc.gnu.org/onlinedocs/) - Compiler packing.
* [Intel IA-32 Architecture Optimization Manual](https://www.intel.com/) - Struct memory alignment rules.

### Authoritative Web Pages, Blogs & Tutorials
* [Eli Bendersky: Memory Layout of C Structures](https://eli.thegreenplace.net/) - Alignment and padding.
* [Dan Luu: Struct Packing and Alignment Costs](https://danluu.com/) - Real-world benchmarks.
* [Martin Thompson: Mechanical Sympathy - Struct Alignment](https://mechanical-sympathy.blogspot.com/) - Cache line optimization.
* [Datadog Engineering: Monitoring Memory Allocation by Struct Type](https://www.datadoghq.com/blog/) - Telemetry.
* [FinOps Foundation: Data Structure Optimization in Cloud Systems](https://www.finops.org/) - Infrastructure efficiency.

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
