#include <assert.h>
#include <limits.h>
#include <stdckdint.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#if defined(__has_include)
#  if __has_include(<stdbit.h>)
#    include <stdbit.h>
#    define HAVE_STDBIT_H 1
#  else
#    define HAVE_STDBIT_H 0
#  endif
#else
#  define HAVE_STDBIT_H 0
#endif

static unsigned int count_ones_u32(uint32_t x) {
#if HAVE_STDBIT_H
    return stdc_count_ones(x);
#else
    return (unsigned int)__builtin_popcount(x);
#endif
}

static unsigned int leading_zeros_u32(uint32_t x) {
#if HAVE_STDBIT_H
    return stdc_leading_zeros(x);
#else
    return x == 0u ? 32u : (unsigned int)__builtin_clz(x);
#endif
}

static bool has_single_bit_u32(uint32_t x) {
#if HAVE_STDBIT_H
    return stdc_has_single_bit(x);
#else
    return x != 0u && (x & (x - 1u)) == 0u;
#endif
}

static uint32_t bit_ceil_u32(uint32_t x) {
#if HAVE_STDBIT_H
    return stdc_bit_ceil(x);
#else
    if (x <= 1u) {
        return 1u;
    }
    return 1u << (32u - (unsigned int)__builtin_clz(x - 1u));
#endif
}

[[nodiscard]]
static void *checked_alloc_array(size_t count, size_t elem_size) {
    size_t total_bytes = 0;
    if (ckd_mul(&total_bytes, count, elem_size)) {
        return nullptr;
    }
    if (total_bytes == 0u) {
        return nullptr;
    }
    return calloc(1u, total_bytes);
}

int main(void) {
    /* 1. C23 <stdckdint.h> overflow-checked integer arithmetic */
    int32_t sum = 0;
    bool overflowed = ckd_add(&sum, INT32_MAX, 1);
    assert(overflowed == true);

    int32_t safe_sum = 0;
    assert(ckd_add(&safe_sum, 40'000, 2'000) == false);
    assert(safe_sum == 42'000);

    uint64_t diff = 0;
    assert(ckd_sub(&diff, 5u, 10u) == true);

    /* Overflowing allocation count must safely return nullptr */
    void *bad_buf = checked_alloc_array(SIZE_MAX / 2u + 2u, 4u);
    assert(bad_buf == nullptr);

    uint32_t *good_buf = checked_alloc_array(16u, sizeof(uint32_t));
    assert(good_buf != nullptr);
    free(good_buf);

    /* 2. C23 <stdbit.h> (with portable fallback when SDK omits stdbit.h) */
    constexpr uint32_t sample = 0b0000'0000'0000'0000'0000'0101'1010'0000u;
    assert(count_ones_u32(sample) == 4u);
    assert(leading_zeros_u32(sample) == 21u);
    assert(has_single_bit_u32(256u) == true);
    assert(has_single_bit_u32(0u) == false);
    assert(bit_ceil_u32(300u) == 512u);

    printf("[02_checked_arithmetic_and_bits] stdbit.h=%d ckd_sum=%d popcnt=%u clz=%u ceil(300)=%u\n",
           HAVE_STDBIT_H, safe_sum, count_ones_u32(sample),
           leading_zeros_u32(sample), bit_ceil_u32(300u));
    return 0;
}
