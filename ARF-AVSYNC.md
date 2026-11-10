# Optional ARF media clock, version 1

An ARF Moonlight client requests `arfavsync=1` in its paired HTTPS launch/resume request. It enables this extension **only** if the successful response contains `<arfAvSync>1</arfAvSync>`. Otherwise it must call `LiSetArfAvSyncEnabled(false)` and retain standard GameStream behavior. The setter runs before `LiStartConnection`, never during streaming.

For an acknowledged stream, audio data contains a 12-byte prefix **inside** its existing encrypted/FEC-protected payload: four ASCII bytes `ARF1`, then an unsigned big-endian 64-bit host steady-clock timestamp in microseconds, then the normal Opus packet. The timestamp identifies the first PCM sample, not packet arrival or encode completion. An invalid/missing prefix is treated as packet loss. Loss-concealment callbacks report a zero timestamp.

The host uses that same steady-clock epoch for video RTP timestamps at 90 kHz, modulo 2^32. The client reads `LiGetArfAudioHostTimeUs()` only inside the synchronous audio playback callback. It maps the audio timestamp onto the local device playback clock and unwraps video RTP near that anchor. The API does not assert that native device latency is known perfectly; hardware synchronization must be measured separately.

Stock peers never receive this payload. The extension changes no existing callback signature or struct layout. Client applications must not request it unless linked against this fork and prepared to interpret the capture-time video clock.

Regression test:

```sh
cc -std=c99 -Wall -Wextra -Werror -fsanitize=address,undefined tests/test_arf_audio.c -o /tmp/test-arf-audio
/tmp/test-arf-audio
```

Alternatively configure CMake with `-DARF_BUILD_TESTS=ON` and run `ctest --output-on-failure` from the build directory.
