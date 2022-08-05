#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "mic.pio.h"
int32_t getSingleSampleBlocking(PIO pio, uint sm);
void initMic(PIO* pio, uint *sm);