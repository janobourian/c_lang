#include <assert.h>
#include <stdatomic.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Type-safe C23 SWAP macro using typeof_unqual */
#define SWAP(a, b)                         \
    do {                                   \
        typeof_unqual(a) _tmp = (a);       \
        (a) = (b);                         \
        (b) = _tmp;                        \
    } while (0)

/* C11/C23 _Generic type-name dispatcher */
#define TYPE_NAME(x) _Generic((x),         \
    int32_t: "int32_t",                    \
    uint64_t: "uint64_t",                  \
    double: "double",                      \
    default: "other"                       \
)

struct Record {
    int32_t id;
    const char *label;
};

/* Overflow-safe comparator: never use (a->id - b->id)! */
static int compare_records(const void *lhs, const void *rhs) {
    const struct Record *a = lhs;
    const struct Record *b = rhs;
    return (a->id > b->id) - (a->id < b->id);
}

int main(void) {
    int32_t x = 10;
    int32_t y = 25;
    SWAP(x, y);
    assert(x == 25 && y == 10);

    uint64_t big = 99u;
    double pi = 3.14159;
    assert(strcmp(TYPE_NAME(x), "int32_t") == 0);
    assert(strcmp(TYPE_NAME(big), "uint64_t") == 0);
    assert(strcmp(TYPE_NAME(pi), "double") == 0);

    struct Record table[] = {
        { .id = 42,      .label = "gamma" },
        { .id = -100000, .label = "alpha" },
        { .id = 7,       .label = "beta"  },
        { .id = INT32_MAX, .label = "omega" },
    };
    constexpr size_t count = sizeof(table) / sizeof(table[0]);

    qsort(table, count, sizeof(table[0]), compare_records);
    assert(table[0].id == -100000);
    assert(table[3].id == INT32_MAX);

    const struct Record key = { .id = 42, .label = nullptr };
    const struct Record *found = bsearch(&key, table, count, sizeof(table[0]), compare_records);
    assert(found != nullptr && strcmp(found->label, "gamma") == 0);

    /* C11/C23 <stdatomic.h> lock-free counter & CAS */
    _Atomic uint64_t counter = 0u;
    atomic_fetch_add_explicit(&counter, 5u, memory_order_relaxed);
    uint64_t expected = 5u;
    bool exchanged = atomic_compare_exchange_strong_explicit(
        &counter, &expected, 12u,
        memory_order_acq_rel, memory_order_acquire
    );
    assert(exchanged == true);
    assert(atomic_load_explicit(&counter, memory_order_acquire) == 12u);

    printf("[03_generic_containers_and_qsort] found=%s atomic=%llu types=(%s,%s,%s)\n",
           found->label,
           (unsigned long long)atomic_load(&counter),
           TYPE_NAME(x), TYPE_NAME(big), TYPE_NAME(pi));
    return 0;
}
