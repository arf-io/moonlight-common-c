#pragma once
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

// Parse the negotiated prefix after decryption/FEC recovery, before Opus decode.
// The original GameStream payload remains untouched unless negotiation succeeded.
static inline bool ArfParseAudio(const char** data, int* length, uint64_t* timestamp) {
    int i;
    *timestamp = 0;
    if (*length <= 12 || memcmp(*data, "ARF1", 4) != 0) return false;
    for (i = 0; i < 8; i++) *timestamp = (*timestamp << 8) | (uint8_t)(*data)[4 + i];
    if (*timestamp == 0) return false;
    *data += 12;
    *length -= 12;
    return true;
}
