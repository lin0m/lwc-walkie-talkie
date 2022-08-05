#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "dac.pio.h"
void initDac(PIO* pio, uint* sm);
void sendDac(PIO pio_dac, uint sm_dac, int32_t lrData);