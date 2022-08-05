#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "mic.pio.h"
#include "dac.pio.h"
// /**
//  * @brief this adds data to the end of a uint32_t array
//  *
//  * @param data data to insert
//  * @param arr array containing data
//  * @param size current size of the array - passed as reference
//  * @param capacity total size of array
//  * @return true - the add was successful
//  * @return false - the add was unsuccessful - array was full;
//  */
// bool addArray(uint32_t data, uint32_t *arr, size_t *size, size_t capacity)
// {
//     if (*size < capacity)
//     {
//         arr[*size++] = data;
//         return true;
//     }
//     else
//     {
//         return false;
//     }
// }
void sendTimer(const int16_t MAX_VALUE_I2S, const double C_PERIOD_MS, PIO pio, uint sm, absolute_time_t* current)
{
    // pio_sm_put_blocking(pio, sm, leftRightData);
    // sleep_ms(C_PERIOD_MS / 2);
    // absolute_time_t pioTime;
    uint32_t leftRightData;
    int32_t right;
    int32_t left;
    if (absolute_time_diff_us(*current, get_absolute_time()) < (int64_t)(((C_PERIOD_MS * 1000) / 2)))
    {
        right = (MAX_VALUE_I2S << 16u) & 0xFFFF0000;
        left = MAX_VALUE_I2S & 0x0000FFFF;
        leftRightData = (uint32_t)right | (uint32_t)left;
        // printf("Sending positive %X\n", leftRightData);
        // pioTime = get_absolute_time();
        pio_sm_put_blocking(pio, sm, leftRightData);
        // printf("Pio time is %lld\n", (absolute_time_diff_us(pioTime, get_absolute_time())));
        // printf("Time used is: %lld\n", (absolute_time_diff_us(current, get_absolute_time())));
    } else if (absolute_time_diff_us(*current, get_absolute_time()) < (int64_t)(((C_PERIOD_MS * 1000)))) {
        right = -((MAX_VALUE_I2S << 16u) & 0xFFFF0000);
        left = -MAX_VALUE_I2S & 0x0000FFFF;
        leftRightData = (uint32_t)right | (uint32_t)left;
        // pio_sm_put_blocking(pio, sm, leftRightData);
        // sleep_ms(C_PERIOD_MS / 2);
        pio_sm_put_blocking(pio, sm, leftRightData);
        // printf("Sending negative %X\n", leftRightData);
        // pioTime = get_absolute_time();
        // printf("Pio time is %lld\n", (absolute_time_diff_us(pioTime, get_absolute_time())));
        // printf("Time used is: %lld\n", (absolute_time_diff_us(current, get_absolute_time())));
    } else {
        *current = get_absolute_time();
    }
}
bool printTimer(absolute_time_t* current, int64_t waitTime_us) {
    if (absolute_time_diff_us(*current, get_absolute_time()) > waitTime_us)
    {
        return true;
    } else {
        return false;
    }
}
int main()
{
    stdio_init_all();
    for (size_t i = 0; i < 5; i++)
    {
        printf("%d\n", i);
        sleep_ms(1000);
    }
    PIO pio = pio0;
    uint offset = pio_add_program(pio, &mic_program);
    uint sm = pio_claim_unused_sm(pio, true);
    mic_program_init(pio, sm, offset, 18, 26);

    PIO pio_dac = pio1;
    uint offset_dac = pio_add_program(pio_dac, &dac_program);
    uint sm_dac = pio_claim_unused_sm(pio_dac, true);
    dac_program_init(pio_dac, sm_dac, offset_dac, 13, 6);
    int32_t lrDataFull = 0;
    int32_t lrData = 0;
    // const double MAX_FREQUENCY = (double)44100 / 2;
    // const double MAX_PERIOD = 1 / MAX_FREQUENCY;
    // const double MAX_PERIOD_US = MAX_PERIOD * 1000000;
    // size_t size = 0;
    // provide half a second of buffer
    // const size_t CAPACITY = 44100 / 2;
    // uint32_t buffer[CAPACITY];
    // const int16_t MAX_VALUE_I2S = (32767);
    // const double C_PERIOD_MS = ((double)1 / 261.63) * 1000;
    // absolute_time_t current = get_absolute_time();
    absolute_time_t printCurrent = get_absolute_time();

    pio_sm_set_enabled(pio, sm, false);
    pio_sm_clear_fifos(pio, sm);
    pio_sm_restart(pio, sm);
    pio_sm_set_enabled(pio, sm, true);
    while (true)
    {
        // sendTimer(MAX_VALUE_I2S, C_PERIOD_MS, pio_dac, sm_dac, &current);
        lrDataFull = pio_sm_get_blocking(pio, sm);
        lrData = lrDataFull & 0xFFFF0000;
        lrDataFull >>= 14;
        pio_sm_put_blocking(pio_dac, sm_dac, lrData / 1024);
        if (printTimer(&printCurrent, 10*1000)) {
            printf("data is: %d\t%08X\n", lrDataFull, lrDataFull);
            printCurrent = get_absolute_time();
        }
    }
        
    return 0;
}
