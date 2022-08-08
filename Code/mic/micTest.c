#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "mic.pio.h"
#include "dac.pio.h"
#include "mic.h"
void sendTimer(const int16_t MAX_VALUE_I2S, const double C_PERIOD_MS, PIO pio, uint sm, absolute_time_t *current)
{
    uint32_t leftRightData;
    int32_t right;
    int32_t left;
    if (absolute_time_diff_us(*current, get_absolute_time()) < (int64_t)(((C_PERIOD_MS * 1000) / 2)))
    {
        right = (MAX_VALUE_I2S << 16u) & 0xFFFF0000;
        left = MAX_VALUE_I2S & 0x0000FFFF;
        leftRightData = (uint32_t)right | (uint32_t)left;
        pio_sm_put_blocking(pio, sm, leftRightData);
    }
    else if (absolute_time_diff_us(*current, get_absolute_time()) < (int64_t)(((C_PERIOD_MS * 1000))))
    {
        right = -((MAX_VALUE_I2S << 16u) & 0xFFFF0000);
        left = -MAX_VALUE_I2S & 0x0000FFFF;
        leftRightData = (uint32_t)right | (uint32_t)left;
        pio_sm_put_blocking(pio, sm, leftRightData);
    }
    else
    {
        *current = get_absolute_time();
    }
}
// void initMic(PIO *pio, uint *sm)
// {
//     *pio = pio0;
//     uint offset = pio_add_program(*pio, &mic_program);
//     *sm = pio_claim_unused_sm(*pio, true);
//     mic_program_init(*pio, *sm, offset, 18, 26);
//     pio_sm_set_enabled(pio, sm, false);
//     pio_sm_clear_fifos(pio, sm);
//     pio_sm_restart(pio, sm);
//     pio_sm_set_enabled(pio, sm, true);
// }
// int32_t getDataBlocking(PIO pio, uint sm)
// {
//     int32_t lrDataFull = 0;
//     int32_t lrData = 0;
//     lrDataFull = pio_sm_get_blocking(pio, sm);
//     lrData = lrDataFull & 0xFFFF0000;
//     return lrData;
// }
int main()
{
    stdio_init_all();
    for (size_t i = 0; i < 5; i++)
    {
        printf("%d\n", i);
        sleep_ms(1000);
    }
    PIO pio;
    uint sm;
    initMic(&pio, &sm);

    PIO pio_dac = pio1;
    uint offset_dac = pio_add_program(pio_dac, &dac_program);
    uint sm_dac = pio_claim_unused_sm(pio_dac, true);
    dac_program_init(pio_dac, sm_dac, offset_dac, 13, 6);
    int32_t lrData = 0;

    absolute_time_t printCurrent = get_absolute_time();
    while (true)
    {
        // sendTimer(MAX_VALUE_I2S, C_PERIOD_MS, pio_dac, sm_dac, &current);
        lrData = getSingleSampleBlocking(pio, sm);
        pio_sm_put_blocking(pio_dac, sm_dac, lrData / 1024);
        if (printTimer(&printCurrent, 10 * 1000))
        {
            printf("data is: %d\t%08X\n", lrData, lrData);
            printCurrent = get_absolute_time();
        }
    }

    return 0;
}
