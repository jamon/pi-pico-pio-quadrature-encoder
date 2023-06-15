//  SPDX-FileCopyrightText: 2022 Jamon Terrell <github@jamonterrell.com>
//  SPDX-License-Identifier: MIT

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "quadrature.pio.h"

#define QUADRATURE_A_PIN 2
#define QUADRATURE_B_PIN 3
int main() {
    stdio_init_all();
    PIO pio = pio0;
    uint offset = pio_add_program(pio, &quadrature_program);
    uint sm = pio_claim_unused_sm(pio, true);
    quadrature_program_init(pio, sm, offset, QUADRATURE_A_PIN, QUADRATURE_B_PIN);
 
    while (true) {
        sleep_ms(1000);
        pio_sm_exec_wait_blocking(pio, sm, pio_encode_in(pio_x, 32));
        uint x = pio_sm_get_blocking(pio, sm);
        printf("%d\n", x);
    }
}