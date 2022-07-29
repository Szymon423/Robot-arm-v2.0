/**
 * Copyright (c) 2021 pmarques-dev @ github
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/timer.h"
#include "hardware/gpio.h"


#include "quadrature_encoder.pio.h"

#define A_PIN 11
#define B_PIN 12

#define STEP_PIN 5
#define DIR_PIN 6


//
// ---- quadrature encoder interface example
//
// the PIO program reads phase A/B of a quadrature encoder and increments or
// decrements an internal counter to keep the current absolute step count
// updated. At any point, the main code can query the current count by using
// the quadrature_encoder_*_count functions. The counter is kept in a full
// 32 bit register that just wraps around. Two's complement arithmetic means
// that it can be interpreted as a 32-bit signed or unsigned value, and it will
// work anyway.
//
// As an example, a two wheel robot being controlled at 100Hz, can use two
// state machines to read the two encoders and in the main control loop it can
// simply ask for the current encoder counts to get the absolute step count. It
// can also subtract the values from the last sample to check how many steps
// each wheel as done since the last sample period.
//
// One advantage of this approach is that it requires zero CPU time to keep the
// encoder count updated and because of that it supports very high step rates.
//

uint32_t timer_cnt = 0;
uint32_t timer_lim = 20;
int encoder_value = 0;

//setting up encoder readin via PIO
// Base pin to connect the A phase of the encoder.
// The B phase must be connected to the next pin

const uint PIN_AB = A_PIN;
PIO pio = pio0;
const uint sm = 0;



bool repeating_timer_callback_PTO(struct repeating_timer *t)
{
    int read = quadrature_encoder_get_count(pio, sm);
    if (read >= 4000)
    {
        gpio_put(DIR_PIN, 0);
    }
    else if (read <= 0)
    {
        gpio_put(DIR_PIN, 1);
    }
    
    if (timer_cnt >= timer_lim)
    {
        timer_cnt = 0;
    }
    
    if (timer_cnt >= (timer_lim / 2))
    {
        gpio_put(STEP_PIN, 0);
    } 
    else
    {
        gpio_put(STEP_PIN, 1);
    }
    
    
    timer_cnt += 1;
    
    return true;
}


int main() {


    

    stdio_init_all();

    uint offset = pio_add_program(pio, &quadrature_encoder_program);
    quadrature_encoder_program_init(pio, sm, offset, PIN_AB, 0);
    
    
    // encodrer pins
    gpio_init(A_PIN);
    gpio_init(B_PIN);
    
    gpio_set_dir(A_PIN, false);
    gpio_set_dir(B_PIN, false);
    
    gpio_pull_up(A_PIN);
    gpio_pull_up(B_PIN);
    
    
    //stepper motor pins
    gpio_init(STEP_PIN);
    gpio_init(DIR_PIN);
    
    gpio_set_dir(STEP_PIN, true);
    gpio_set_dir(DIR_PIN, true);
    
    gpio_put(STEP_PIN, 0);
    gpio_put(DIR_PIN, 1);
    
    //calling timer IRQ
    struct repeating_timer timer;
    add_repeating_timer_us(30, repeating_timer_callback_PTO, NULL, &timer);
    
    
 
    while (1) 
    {

        //encoder_value = quadrature_encoder_get_count(pio, sm);
        
        //printf("position_new  %8d\n", encoder_value);
        sleep_ms(200);
    }
}
