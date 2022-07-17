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
    test_program_init(pio, sm, offset, PICO_DEFAULT_LED_PIN);
    // A C note is 261.63hz or a period of 0.003822191645
    const size_t C_PERIOD = 1/261.63;
    while (true) {
        pio_sm_put_blocking(pio, sm, MAX_VALUE_I2S * (1 / 2));
        sleep_ms(C_PERIOD / 2);
        pio_sm_put_blocking(pio, sm, MAX_VALUE_I2S * (1 / 2));
        sleep_ms(C_PERIOD / 2);
    }
    return 0;
}
