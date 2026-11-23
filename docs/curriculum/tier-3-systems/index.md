---
title: "Tier 3 · Toolchains, OS & Concurrency (Modules 09 – 13)"
description: "Compilation pipeline, ELF linking, UNIX syscalls, epoll/kqueue, zero-copy mmap, pthreads, C11/C23 atomics, and Berkeley sockets."
icon: lucide/network
---

# Tier 3 · Toolchains, OS & Concurrency (`09` – `13`)

Step directly into operating system kernels, scalable non-blocking network I/O, ELF binary linking, and multi-core thread synchronization.

<div class="grid cards" markdown>

- **09 · Toolchains, ELF & Makefiles**

    4-stage compilation pipeline, ELF sections, static archives `.a`, shared objects `.so`, GOT/PLT, Full RELRO, and Make automation.

    [:octicons-arrow-right-24: Open Module 09](09-toolchains-elf-makefiles.md)

- **10 · UNIX Syscalls & File Descriptors**

    User-to-kernel privilege transitions, 3-tier FD tables, `openat`, `pwrite`, `dup2`, and `fcntl` advisory record locking.

    [:octicons-arrow-right-24: Open Module 10](10-unix-syscalls-file-descriptors.md)

- **11 · `epoll`, `kqueue` & Zero-Copy `mmap`**

    C10K/C1M event demultiplexing, edge-triggering (`EPOLLET`), memory-mapped files (`mmap`/`msync`), and kernel `sendfile`.

    [:octicons-arrow-right-24: Open Module 11](11-io-multiplexing-epoll-kqueue-mmap.md)

- **12 · Concurrency, `pthreads` & Atomics**

    POSIX threads, mutexes, rwlocks, condition variables, `<stdatomic.h>` atomics, and acquire-release memory ordering.

    [:octicons-arrow-right-24: Open Module 12](12-concurrency-pthreads-atomics.md)

- **13 · Network Sockets & TLS**

    Berkeley sockets, non-blocking TCP servers, `SO_REUSEPORT`, `TCP_NODELAY`, binary TLV framing, and OpenSSL TLS integration.

    [:octicons-arrow-right-24: Open Module 13](13-network-sockets-tls.md)

</div>

**Previous Tier:** [Tier 2 · Runtime & Preprocessor](../tier-2-runtime/index.md) · **Next Tier:** [Tier 4 · Hardware, Forensics & Capstones](../tier-4-hardware/index.md)
