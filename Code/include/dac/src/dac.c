#include "dac.h"
void initDac(PIO* pio, uint* sm) {
    *pio = pio1;
    uint offset = pio_add_program(*pio, &dac_program);
    *sm = pio_claim_unused_sm(*pio, true);
    dac_program_init(*pio, *sm, offset, 13, 6);
}
void sendDac(PIO pio_dac, uint sm_dac, int32_t lrData) {
    pio_sm_put_blocking(pio_dac, sm_dac, lrData);
}