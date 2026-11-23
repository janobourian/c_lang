#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

/* C23 #embed binary resource inclusion with portable __has_embed guard */
#if defined(__has_embed)
#  if __has_embed("banner.txt")
static const unsigned char kEmbeddedBanner[] = {
#    embed "banner.txt" suffix(, 0)
};
#    define USED_C23_EMBED 1
#  endif
#endif

#ifndef USED_C23_EMBED
static const unsigned char kEmbeddedBanner[] = "fallback-banner";
#  define USED_C23_EMBED 0
#endif

/* C23 [[unsequenced]] / [[reproducible]] guarded via __has_c_attribute */
#if defined(__has_c_attribute)
#  if __has_c_attribute(unsequenced)
#    define C23_UNSEQUENCED [[unsequenced]]
#  else
#    define C23_UNSEQUENCED
#  endif
#else
#  define C23_UNSEQUENCED
#endif

/* In ISO C23 (N2956), [[unsequenced]] appertains to the function type (after ')'). */
static unsigned _BitInt(24) rgb888_pack(uint8_t r, uint8_t g, uint8_t b) C23_UNSEQUENCED {
    unsigned _BitInt(24) packed = ((unsigned _BitInt(24))r << 16)
                                | ((unsigned _BitInt(24))g << 8)
                                | (unsigned _BitInt(24))b;
    return packed;
}

int main(void) {
    /* C23 bit-precise integer types (_BitInt) and wb / uwb suffixes */
    constexpr unsigned _BitInt(24) kTeal = 0x06'B6'D4uwb;
    constexpr signed _BitInt(12) kDelta  = -123wb;

    unsigned _BitInt(24) packed = rgb888_pack(0x06u, 0xB6u, 0xD4u);
    assert(packed == kTeal);
    assert(kDelta == -123);

    /* Validate #embed payload */
    assert(sizeof(kEmbeddedBanner) > 10u);
    const char *text = (const char *)kEmbeddedBanner;
    assert(strstr(text, "C23") != nullptr);

    printf("[05_bitint_and_embed] embed=%d rgb24=0x%06X delta=%d bytes=%zu\n",
           USED_C23_EMBED, (unsigned int)packed, (int)kDelta, sizeof(kEmbeddedBanner));
    return 0;
}
