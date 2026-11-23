#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <locale.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct TelemetryFrame {
    uint32_t seq;
    double temperature_c;
};

[[nodiscard]]
static bool parse_strict_i64(const char *text, int64_t *out) {
    if (text == nullptr || *text == '\0' || out == nullptr) {
        return false;
    }
    errno = 0;
    char *end = nullptr;
    long long value = strtoll(text, &end, 10);
    if (errno == ERANGE || end == text || *end != '\0') {
        return false;
    }
    *out = (int64_t)value;
    return true;
}

int main(void) {
    /* 1. Strict integer parsing with errno + endptr validation */
    int64_t parsed = 0;
    assert(parse_strict_i64("123456", &parsed) && parsed == 123456);
    assert(!parse_strict_i64("123abc", &parsed));
    assert(!parse_strict_i64("", &parsed));

    /* 2. Locale verification ("C" locale guarantees '.' decimal point) */
    const char *loc = setlocale(LC_ALL, "C");
    assert(loc != nullptr);
    const struct lconv *lc = localeconv();
    assert(lc != nullptr && strcmp(lc->decimal_point, ".") == 0);

    /* 3. Safe snprintf truncation check */
    char msg[64] = {};
    int written = snprintf(msg, sizeof(msg), "seq=%u temp=%.2fC", 7u, 23.75);
    assert(written > 0 && (size_t)written < sizeof(msg));

    /* 4. Binary file I/O round-trip using tmpfile() */
    FILE *fp = tmpfile();
    assert(fp != nullptr);

    const struct TelemetryFrame tx = { .seq = 7u, .temperature_c = 23.75 };
    assert(fwrite(&tx, sizeof(tx), 1u, fp) == 1u);
    rewind(fp);

    struct TelemetryFrame rx = {};
    assert(fread(&rx, sizeof(rx), 1u, fp) == 1u);
    assert(rx.seq == tx.seq && rx.temperature_c == tx.temperature_c);
    assert(fclose(fp) == 0);

    /* 5. High-resolution timestamp + UTC formatting */
    struct timespec ts = {};
    assert(timespec_get(&ts, TIME_UTC) == TIME_UTC);

    char iso_utc[32] = {};
    const struct tm *utc_tm = gmtime(&ts.tv_sec);
    assert(utc_tm != nullptr);
    size_t len = strftime(iso_utc, sizeof(iso_utc), "%Y-%m-%dT%H:%M:%SZ", utc_tm);
    assert(len > 0u);

    printf("[04_hardened_io_and_time] msg=\"%s\" utc=%s nsec=%ld\n",
           msg, iso_utc, ts.tv_nsec);
    return 0;
}
