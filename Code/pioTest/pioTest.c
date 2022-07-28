#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "test.pio.h"
// TODO: functions for different output modes
// TODO: different configurations in pio
// TODO: more debug info?
void sendLoop(const int16_t MAX_VALUE_I2S, const double C_PERIOD_MS, PIO pio, uint sm)
{
    int32_t right = (MAX_VALUE_I2S << 16u) & 0xFFFF0000;
    int32_t left = MAX_VALUE_I2S & 0x0000FFFF;
    const double SAMPLING_PERIOD_MS = (double)1 / 44100 * 1000;
    for (size_t i = 0; i < (size_t)((C_PERIOD_MS / SAMPLING_PERIOD_MS) / 2); i++)
    {
        pio_sm_put_blocking(pio, sm, (right | left));
        printf("Sending positive %X\n", (right | left));
    }
    right = -((MAX_VALUE_I2S << 16u) & 0xFFFF0000);
    left = -MAX_VALUE_I2S & 0x0000FFFF;
    for (size_t i = 0; i < (size_t)((C_PERIOD_MS / SAMPLING_PERIOD_MS) / 2); i++)
    {
        pio_sm_put_blocking(pio, sm, (right | left));
        printf("Sending negative %X\n", (right | left));
    }
}
void sendTimer(const int16_t MAX_VALUE_I2S, const double C_PERIOD_MS, PIO pio, uint sm)
{
    int32_t right = (MAX_VALUE_I2S << 16u) & 0xFFFF0000;
    int32_t left = MAX_VALUE_I2S & 0x0000FFFF;
    left = 0x00000000;
    uint32_t leftRightData = (uint32_t)right | (uint32_t)left;
    // pio_sm_put_blocking(pio, sm, leftRightData);
    // sleep_ms(C_PERIOD_MS / 2);
    pio_sm_put_blocking(pio, sm, leftRightData);
    absolute_time_t current = get_absolute_time();
    // absolute_time_t pioTime;
    while (absolute_time_diff_us(current, get_absolute_time()) < (int64_t)(((C_PERIOD_MS * 1000) / 2)))
    {
        printf("Sending positive %X\n", leftRightData);
        // pioTime = get_absolute_time();
        pio_sm_put_blocking(pio, sm, leftRightData);
        // printf("Pio time is %lld\n", (absolute_time_diff_us(pioTime, get_absolute_time())));
        // printf("Time used is: %lld\n", (absolute_time_diff_us(current, get_absolute_time())));
    }
    right = -((MAX_VALUE_I2S << 16u) & 0xFFFF0000);
    left = -MAX_VALUE_I2S & 0x0000FFFF;
    left = 0x00000000;
    leftRightData = (uint32_t)right | (uint32_t)left;
    // pio_sm_put_blocking(pio, sm, leftRightData);
    // sleep_ms(C_PERIOD_MS / 2);
    pio_sm_put_blocking(pio, sm, leftRightData);
    current = get_absolute_time();
    // current problem: I think autopull pulls partially and not the whole 32 bit value
    // it's almost like it's sending only the left channel
    // the first 3 bits do change
    while (absolute_time_diff_us(current, get_absolute_time()) < (int64_t)(((C_PERIOD_MS * 1000) / 2)))
    {
        printf("Sending negative %X\n", leftRightData);
        // pioTime = get_absolute_time();
        pio_sm_put_blocking(pio, sm, leftRightData);
        // printf("Pio time is %lld\n", (absolute_time_diff_us(pioTime, get_absolute_time())));
        // printf("Time used is: %lld\n", (absolute_time_diff_us(current, get_absolute_time())));
    }
}
int main()
{
    stdio_init_all();
    const int16_t MAX_VALUE_I2S = (32767);
    PIO pio = pio0;
    uint offset = pio_add_program(pio, &test_program);
    uint sm = pio_claim_unused_sm(pio, true);
    // pin(s) configurated here: want din to gp18, wsel gp26, bclk to gp27
    // problem: wsel is gp27, bclk is gp26
    test_program_init(pio, sm, offset, 18, 26);
    // A C note is 261.63hz or a period of 0.003822191645s
    const double C_PERIOD_MS = ((double)1 / 261.63) * 1000;
    // TODO: The fancier way of doing the audio output is to continue sending the same data until a timer goes off to change to a value. This timer will be equal to the sampling frequency.
    // Building on the sampling frequency, the clock speed, therefore can be calculated with 1/sampling_frequency - or sampling period - and the time for the pio to complete - which is based on the clock speed.
    // The pio must complete just before the sampling period and there are ~32 cycles in the pio, so the frequency of the pio must be 32/(sampling period)
    // sampling frequency should be constant and related to the bitrate of the audio
    // bitrate = sample rate(!!!) * bit depth(bits of data) * 2(number of channels)
    // standard bitrate should be 44100 Hz for CD, some experimenting can be done, but note that the data is not compressed at all, so lower bitrates may drastically decrease audio quality because less important bits are not removed/optimized
    // This should change for higher quality audio or to save bandwidth
    // pio frequency = 1.4112 MHz
    // pico frequency = 125Mhz
    // divider value = 88.57709751
    // put the values twice in input because of using mono audio - 16 bits for left, 16 bits for right
    while (true)
    {
        sendTimer(MAX_VALUE_I2S, C_PERIOD_MS, pio, sm);
    }
    return 0;
}
