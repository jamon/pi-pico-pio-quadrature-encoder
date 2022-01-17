Raspberry Pi Pico PIO Quadrature Encoder

Works with C/C++ via pico-sdk, arduino, and micropython.  Examples are provided for all but arduino.

Pass A and B pins to PIO program and it will keeps track of quadrature position in state machine's "x" register.

To read the current value, force exec an "in" command from "x" register, then read the X value (see example in main.c).

The value of x is an absolute value of the encoder (initialized to 0 on startup).  You can do relative measurements for a menu by capturing the value when the menu starts, but it doesn't support limits currently, so probably the best workaround would be to take the absolute value of the encoder modulo the length of your menu to select determine the position.

# How to use
* Include [quadrature.pio](src/quadrature.pio) in your project
* Include headers
```c
#include "hardware/pio.h"
#include "quadrature.pio.h"
```
* Define your QUADRATURE Encoder pins
```c
#define QUADRATURE_A_PIN 2
#define QUADRATURE_B_PIN 3
```
* Initialize a PIO state machine
```c
PIO pio = pio0;
uint offset = pio_add_program(pio, &quadrature_program);
uint sm = pio_claim_unused_sm(pio, true);
quadrature_program_init(pio, sm, offset, QUADRATURE_A_PIN, QUADRATURE_B_PIN);
 ```
* Read the current value of the encoder from the state machine
```c
pio_sm_exec_wait_blocking(pio, sm, pio_encode_in(pio_x, 32));
uint x = pio_sm_get_blocking(pio, sm);
```

## Zeroing/resetting position

```c
pio_sm_exec(pio, sm, pio_encode_set(pio_x, 0));
```

## Switch/Button

Not really applicable for this kind of library, but I'm going to drop this here in case it's useful.  If you want to handle button presses via IRQ:

```c
#define QUADRATURE_SW_PIN 4
PIO pio = pio0;
uint offset, sm;
void quadrature_sw_callback(uint gpio, uint32_t events) {
    if(gpio == QUADRATURE_SW_PIN) pio_sm_exec(pio, sm, pio_encode_set(pio_x, 0));
}
int main() {
    stdio_init_all();

    offset = pio_add_program(pio, &quadrature_program);
    sm = pio_claim_unused_sm(pio, true);

    quadrature_program_init(pio, sm, offset, QUADRATURE_A_PIN, QUADRATURE_B_PIN);

    gpio_set_irq_enabled_with_callback(QUADRATURE_SW_PIN, GPIO_IRQ_EDGE_FALL, true, &quadrature_sw_callback);

    // rest of your logic
}
```
# Python Example
See [python/quadrature.py](python/quadrature.py).

# Example
See [src/main.c](src/main.c) for an example that reads the value once per second and outputs it.   

# License
Licensed under the [MIT License](LICENSE)