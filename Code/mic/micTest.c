#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "mic.pio.h"

/**
 * @brief this adds data to the end of a uint32_t array
 *
 * @param data data to insert
 * @param arr array containing data
 * @param size current size of the array - passed as reference
 * @param capacity total size of array
 * @return true - the add was successful
 * @return false - the add was unsuccessful - array was full;
 */
bool addArray(uint32_t data, uint32_t *arr, size_t *size, size_t capacity)
{
    if (*size < capacity)
    {
        arr[*size++] = data;
        return true;
    }
    else
    {
        return false;
    }
}
int main()
{
    stdio_init_all();
    sleep_ms(10000);
    PIO pio = pio0;
    uint offset = pio_add_program(pio, &mic_program);
    uint sm = pio_claim_unused_sm(pio, true);
    mic_program_init(pio, sm, offset, 18, 10);
    uint32_t lrData;
    printf("program initialized\n");
    // size_t size = 0;
    // provide half a second of buffer
    // const size_t CAPACITY = 44100 / 2;
    // uint32_t buffer[CAPACITY];
    while (true)
    {
        lrData = pio_sm_get_blocking(pio, sm);
        printf("data is: %08X\n", lrData);
        sleep_ms(10);
        // if unsuccessful, wait until buffer is not full
        // if (!addArray(lrData, buffer, &size, CAPACITY))
        // {
        //     sleep_ms(1);
        // }
    }
    return 0;
}
