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



uint32_t timer_cnt = 0;
uint32_t timer_lim = 2;
int encoder_value = 0;
int target_value = 0;
int histeresis = 10;
bool on = true;


//setting up encoder readin via PIO
const uint PIN_AB = A_PIN;
const uint sm = 0;
PIO pio = pio0;




bool repeating_timer_callback_PTO(struct repeating_timer *t)
{
    encoder_value = quadrature_encoder_get_count(pio, sm);
    if (encoder_value > target_value + histeresis)
    {
        gpio_put(DIR_PIN, 0);
        on = true;
    }
    else if (encoder_value < target_value - histeresis)
    {
        gpio_put(DIR_PIN, 1);
        on = true;
    }
    else 
    {
        on = false;
    }
    
    
    if (timer_cnt >= timer_lim)
    {
        timer_cnt = 0;
    }
    if (on)
    {
        if (timer_cnt >= (timer_lim / 2))
        {
            gpio_put(STEP_PIN, 0);
        } 
        else
        {
            gpio_put(STEP_PIN, 1);
        }  
    }
    
    timer_cnt += 1;12
    
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
    add_repeating_timer_us(40, repeating_timer_callback_PTO, NULL, &timer);
    
    char data;
    
 
    while (1) 
    {
       
       data = getchar();
       
       target_value = 1000 * (data - '0');
       
           
       
       printf("odczytane: %d enkoder: %d \n", target_value, encoder_value);
       
       
    }
}
