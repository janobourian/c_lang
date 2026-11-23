#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

static_assert(__STDC_VERSION__ >= 202311L);

enum LogLevel : uint8_t {
    LOG_DEBUG = 0,
    LOG_INFO  = 1,
    LOG_WARN  = 2,
    LOG_ERROR = 3,
};

static_assert(sizeof(enum LogLevel) == sizeof(uint8_t));

struct Packet {
    uint32_t magic;
    uint16_t length;
    enum LogLevel level;
};

[[nodiscard("ignoring parsed status hides protocol failures")]]
static int classify_code(int code) {
    int score = 0;
    switch (code) {
        case 0b0001:
            score += 10;
            [[fallthrough]];
        case 0b0010:
            score += 5;
            break;
        default:
            goto done;
    }

done:
    /* C23 allows declarations immediately after a label */
    int adjusted = score * 2;
    return adjusted;
}

/* C23 permits unnamed parameters in function definitions */
static void ignore_callback(int, [[maybe_unused]] void *ctx) {
    /* Intentional no-op callback */
}

int main(void) {
    constexpr uint32_t kMagic = 0xCAFE'BABEu;
    constexpr size_t kLimit   = 1'024u;
    constexpr uint8_t kMask   = 0b1010'0101u;

    static_assert(kMagic == 0xCAFEBABEu);
    static_assert(kLimit == 1024u);
    static_assert(kMask == 0xA5u);

    /* C23 empty initializer zero-initializes every member and padding */
    struct Packet pkt = {};
    assert(pkt.magic == 0u && pkt.length == 0u && pkt.level == LOG_DEBUG);

    pkt = (struct Packet){
        .magic  = kMagic,
        .length = 64u,
        .level  = LOG_INFO,
    };

    /* C23 auto type inference + typeof / typeof_unqual (storage class first) */
    auto const header_magic = pkt.magic;
    typeof_unqual(header_magic) mutable_copy = header_magic;
    mutable_copy ^= 0xFFu;

    /* C23 nullptr & nullptr_t */
    nullptr_t nil = nullptr;
    const struct Packet *ptr = nil;
    assert(ptr == nullptr);
    ptr = &pkt;

    int result = classify_code(0b0001);
    assert(result == 30);

    ignore_callback(42, nullptr);

    printf("[01_c23_core_language] magic=0x%08X copy=0x%08X score=%d sizeof(LogLevel)=%zu\n",
           ptr->magic, mutable_copy, result, sizeof(enum LogLevel));
    return 0;
}
