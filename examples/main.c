//  SPDX-FileCopyrightText: 2022 Jamon Terrell <github@jamonterrell.com>
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

    while (true)
    {
        sleep_ms(50);

        volatile int32_t countA = pio_sm_get_blocking(encoderPIO, smA);
        volatile int32_t countB = pio_sm_get_blocking(encoderPIO, smB);

        volatile int32_t totalEncCount = countA + countB;

        printf("%d\n", totalEncCount);
    }
}