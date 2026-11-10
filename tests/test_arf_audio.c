#include "../src/ArfAudio.h"
#include <assert.h>
#include <stdio.h>

int main(void) {
    const char packet[] = {'A', 'R', 'F', '1', 1, 2, 3, 4, 5, 6, 7, 8, 'o', 'p', 'u', 's'};
    const char* payload = packet;
    uint64_t timestamp = 0;
    int length = sizeof(packet), i;
    assert(ArfParseAudio(&payload, &length, &timestamp));
    assert(timestamp == UINT64_C(0x0102030405060708));
    assert(length == 4 && memcmp(payload, "opus", 4) == 0);
    for (i = -1; i <= 12; i++) {
        payload = packet;
        length = i;
        assert(!ArfParseAudio(&payload, &length, &timestamp));
        assert(timestamp == 0 && payload == packet && length == i);
    }
    {
        char invalid[sizeof(packet)];
        memcpy(invalid, packet, sizeof(packet));
        invalid[0] = 'X';
        payload = invalid;
        length = sizeof(invalid);
        assert(!ArfParseAudio(&payload, &length, &timestamp));
        memcpy(invalid, packet, sizeof(packet));
        memset(invalid + 4, 0, 8);
        payload = invalid;
        length = sizeof(invalid);
        assert(!ArfParseAudio(&payload, &length, &timestamp));
    }
    puts("ARF audio framing tests passed");
    return 0;
}
