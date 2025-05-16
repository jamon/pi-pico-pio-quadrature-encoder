//  SPDX-FileCopyrightText: 
//2022 Jamon Terrell <github@jamonterrell.com>
//2024 Arda Alıcı <ardayaozel@hotmail.com>
//  SPDX-License-Identifier: MIT


#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "quadrature.pio.h"

#define QUADRATURE_A_PIN 2
#define QUADRATURE_B_PIN 3
int main()
{
    stdio_init_all();

    PIO encoderPIO = pio0;

    uint offsetA = pio_add_program(encoderPIO, &quadratureA_program);
    uint smA = pio_claim_unused_sm(encoderPIO, true);

    uint offsetB = pio_add_program(encoderPIO, &quadratureB_program);
    uint smB = pio_claim_unused_sm(encoderPIO, true);

    quadratureA_program_init(encoderPIO, smA, offsetA, QUADRATURE_A_PIN, QUADRATURE_B_PIN);
    quadratureB_program_init(encoderPIO, smB, offsetB, QUADRATURE_A_PIN, QUADRATURE_B_PIN);

    while (true)
    {
        sleep_ms(1000);

        pio_sm_exec_wait_blocking(encoderPIO, smA, pio_encode_in(pio_x, 32));
        pio_sm_exec_wait_blocking(encoderPIO, smB, pio_encode_in(pio_x, 32));

        int32_t countA = pio_sm_get_blocking(encoderPIO, smA);
        int32_t countB = pio_sm_get_blocking(encoderPIO, smB);

        int32_t x = countA + countB;
        
        printf("%d\n", x);
    }
}
