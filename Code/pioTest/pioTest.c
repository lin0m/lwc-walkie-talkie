#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "test.pio.h"


int main()
{
    stdio_init_all();
    const size_t MAX_VALUE_I2S = 2^16;
    PIO pio = pio0;
    uint offset = pio_add_program(pio, &test_program);
    uint sm = pio_claim_unused_sm(pio, true);
    // pin(s) configurated here: want din to gp18, wsel gp19, clk to gp20 
        // in the future, the wsel and clk together can be set to arbitrary pins, but for now, they are consecutive
    test_program_init(pio, sm, offset, 18);
    // A C note is 261.63hz or a period of 0.003822191645ms
    const size_t C_PERIOD = 1/261.63;
    // TODO: The fancier way of doing the audio output is to continue sending the same data until a timer goes off to change to a value. This timer will be equal to the sampling frequency.
        // Building on the sampling frequency, the clock speed, therefore can be calculated with 1/sampling_frequency - or sampling period - and the time for the pio to complete - which is based on the clock speed.
            // The pio must complete just before the sampling period and there are ~32 cycles in the pio, so the frequency of the pio must be 32/(sampling period)
            // sampling frequency should be constant and related to the bitrate of the audio 
                // bitrate = sample rate(!!!) * bit depth(bits of data) * 2(number of channels)
                // standard bitrate should be 44100 kHz for CD, some experimenting can be done, but note that the data is not compressed at all, so lower bitrates may drastically decrease audio quality because less important bits are not removed/optimized
                // This should change for higher quality audio or to save bandwidth
    // put the values twice in input because of using mono audio - 16 bits for left, 16 bits for right
    while (true) {
        // For now, let's see what happens if there is a huge delay between data in and a super fast bclk
        pio_sm_put_blocking(pio, sm, (MAX_VALUE_I2S * (1 / 2) << 16u) | MAX_VALUE_I2S * (1 / 2));
        sleep_ms(C_PERIOD / 2);
        pio_sm_put_blocking(pio, sm, -((MAX_VALUE_I2S * (1 / 2) << 16u) | MAX_VALUE_I2S * (1 / 2)));
        sleep_ms(C_PERIOD / 2);
    }
    return 0;
}
