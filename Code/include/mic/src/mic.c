#include "mic.h"
void initMic(PIO* pio, uint *sm) {
    *pio = pio0;
    uint offset = pio_add_program(*pio, &mic_program);
    *sm = pio_claim_unused_sm(*pio, true);
    mic_program_init(*pio, *sm, offset, 18, 26);
    pio_sm_set_enabled(*pio, *sm, false);
    pio_sm_clear_fifos(*pio, *sm);
    pio_sm_restart(*pio, *sm);
    pio_sm_set_enabled(*pio, *sm, true);
}
int32_t getSingleSampleBlocking(PIO pio, uint sm) {
    int32_t lrDataFull = pio_sm_get_blocking(pio, sm);
    int32_t lrData = lrDataFull & 0xFFFF0000;
    return lrData;
}
