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
float error_max = 1000.0;
int time_max = 1000.0;
int position_error = 0;
float histeresis = 4.0;
float a = 1;
float b = 0;
bool on = true;

struct Regulator
{
    float P;
    float I;
    float D;

    float kP;
    float kI;
    float kD;

    float sum;
    float abs_sum;
};

struct Regulator regulator = {1.0, 0.0, 0.0, 50.0, 0.0, 0.0, 0.0};


//setting up encoder readin via PIO
const uint PIN_AB = A_PIN;
const uint sm = 0;
PIO pio = pio0;

float abs (float in)
{
    if (in <= 0.0) 
    {
        return -in;
    }
    else
    {
        return in;
    }
}



bool repeating_timer_callback_PTO(struct repeating_timer *t)
{
    // reading of current position  via DMA
    encoder_value = quadrature_encoder_get_count(pio, sm);

    // defining current error basen on current and target positions
    position_error = target_value - encoder_value;

    // calcultion of P value based on error
    regulator.P = position_error * regulator.kP;

    // regulator.sum = regulator.P + regulator.I + regulator.D; // I and D =0
    regulator.sum = regulator.P;
    regulator.abs_sum = abs(regulator.sum);

    //setting proper timer limit based on regulator output
    if (regulator.abs_sum > error_max)
    {
        timer_lim = 2;
    }
    else if (regulator.abs_sum < histeresis)
    {
        timer_lim = 1000;
        // printf("suma: %f abs(suma): %f hist: %f \n", regulator.sum, abs(regulator.sum), histeresis);
    }
    else 
    {
        timer_lim = (int)(a * regulator.abs_sum + b);
    }
    

    if (regulator.sum > histeresis)
    {
        gpio_put(DIR_PIN, 1);
        on = true;
    }
    else if (regulator.sum < -histeresis)
    {
        gpio_put(DIR_PIN, 0);
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
    
    timer_cnt += 1;
    
    return true;
}


int main() {

    stdio_init_all();

    uint offset = pio_add_program(pio, &quadrature_encoder_program);
    quadrature_encoder_program_init(pio, sm, offset, PIN_AB, 0);
    
    // calculaing interpolation due to histeresis
    a = (time_max - 2) / (histeresis - error_max);
    b = 2 - a * error_max;
    


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
       
           
       
       printf("odczytane: %d enkoder: %d err: %d regulator-sum: %f loop-time: %d a: %f b: %f \n", target_value, encoder_value, position_error, regulator.sum, timer_lim, a, b);
       
       
    }
}
