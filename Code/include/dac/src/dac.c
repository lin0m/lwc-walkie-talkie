#include "dac.h"
void initDac() {
    PIO pio_dac = pio1;
    uint offset_dac = pio_add_program(pio_dac, &dac_program);
    uint sm_dac = pio_claim_unused_sm(pio_dac, true);
    dac_program_init(pio_dac, sm_dac, offset_dac, 13, 6);
}
void sendDac(PIO pio_dac, uint sm_dac, int32_t lrData) {
    pio_sm_put_blocking(pio_dac, sm_dac, lrData);
}