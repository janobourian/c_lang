---
title: "Tier 1 · Core Architecture & Memory (Modules 00 – 04)"
description: "Process address space, System V ABI, pointer arithmetic, bounds-safe strings, struct alignment, unions, and bitfields."
icon: lucide/cpu
---

# Tier 1 · Core Architecture & Memory (`00` – `04`)

Bridge from basic syntax to how C executes on real silicon: virtual address segments, ABI stack frames, pointer arithmetic, cache-line stride efficiency, memory-safe string handling, and exact struct/union layouts.

<div class="grid cards" markdown>

- **00 · Foundations & Process Memory Model**

    ELF memory segments (Text/Data/BSS/Heap/Stack), `<stdint.h>` exact-width types, endianness, storage duration, and Undefined Behavior.

    [:octicons-arrow-right-24: Open Module 00](00-foundations-memory-model.md)

- **01 · Functions, ABI & Stack Frames**

    System V AMD64 calling convention, activation records, stack canaries, variadic functions, and `inline` linkage.

    [:octicons-arrow-right-24: Open Module 01](01-functions-abi-stack.md)

- **02 · Arrays, Pointers & Arithmetic**

    Array-to-pointer decay, scaled pointer arithmetic, row-major matrix traversal, and 64-byte CPU cache lines.

    [:octicons-arrow-right-24: Open Module 02](02-arrays-pointers-arithmetic.md)

- **03 · Strings & Bounds Safety**

    Null-terminated byte arrays, CWE-120 buffer overflow defense, `snprintf`, `strtok_r`, and zero-copy text parsers.

    [:octicons-arrow-right-24: Open Module 03](03-strings-bounds-safety.md)

- **04 · Structs, Unions, Bitfields & Enums**

    Natural alignment, struct padding optimization, `offsetof`, `alignas`, tagged variant unions, bitfield pitfalls, and C23 typed enums.

    [:octicons-arrow-right-24: Open Module 04](04-structs-unions-bitfields-enums.md)

</div>

**Previous Tier:** [Tier 0 · Absolute Foundations](../pre-tier/index.md) · **Next Tier:** [Tier 2 · Runtime & Preprocessor](../tier-2-runtime/index.md)
