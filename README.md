Raspberry Pi Pico PIO Quadrature Encoder

Pass A and B pins to PIO program and it will keeps track of quadrature position in state machine's "x" register.

To read the current value, force exec an "in" command from "x" register, then read the X value (see example in main.c).

Licensed under the [MIT License](LICENSE)