#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/timer.h"
#include "hardware/gpio.h"

#include "quadrature_encoder.pio.h"

// attaching encoder pins
#define A_PIN 11
#define B_PIN 12

// attaching stepper motor pins
#define STEP_PIN 5
#define DIR_PIN 6

//values for calculating speed
#define timer_delay 40
#define steps_per_revolution 5000

// timer variables
uint32_t timer_cnt = 0;     // timer current value
uint32_t timer_lim = 1;     // timer upper limit value

// position maitaining values
int encoder_value = 0;      // current encoder reading
int target_value = 0;       // current goal value
int read_value = 0;         // value which was read by uC
float current_value = 0.0;  // current position value - for smoothing
float previous_value = 0.0; // previous position value - for smoothing
float division = 0.99;           // smooting value
int position_error = 0;     // current error in position
float histeresis = 2.0;     // geting rid of shaky movements with histeresis

// error - speed charakcteristic 
float error_max = 1000.0;   // max value of error
float error_min = 3.0;      // min value of error
float time_max = 1000.0;    // max value of duty cycle
float time_min = 1.0;       // min value of duty cycle
float a = 1.0;              //
float b = 0.0;              // f(x) = a * (x - b) + c
float c = -0.1;             //

// variable defining need of movement
bool on = true;

// defining proper kP gain
const float kP_reg = 10.0;

// structure with regulator stuff
struct Regulator
{
    // current PID values
    float P;
    float I;
    float D;

    // proper gains 
    float kP;
    float kI;
    float kD;

    // output and |output|
    float sum;
    float abs_sum;
};

// defining regulator struct
struct Regulator regulator = {1.0, 0.0, 0.0, kP_reg, 0.0, 0.0, 0.0};


//setting up encoder readin via PIO
const uint PIN_AB = A_PIN;
const uint sm = 0;
PIO pio = pio0;


// function responsible for IRQ handling
bool repeating_timer_callback_PTO(struct repeating_timer *t)
{
    // smoothing the target value
    current_value = (1.0 - division) * (float)read_value + division * previous_value;
    previous_value = current_value;
    target_value = (int)current_value;
    
    // reading of current position  via DMA
    encoder_value = quadrature_encoder_get_count(pio, sm);

    // defining current error basen on current and target positions
    position_error = target_value - encoder_value;

    // calcultion of P value based on error
    regulator.P = position_error * regulator.kP;

    // regulator.sum = regulator.P + regulator.I + regulator.D; // I and D = 0
    regulator.sum = regulator.P;
    regulator.abs_sum = abs(regulator.sum);

    //setting proper timer limit based on regulator output
    if (regulator.abs_sum > error_max) 
    {
        // max speed when sum excedees max error
        timer_lim = (int)time_min;
    } 
    else if (regulator.abs_sum < error_min) 
    {
        // min speed for small errors
        timer_lim = (int)time_max;
    } 
    else 
    {
        // linear time, not linear speed
        //timer_lim = (int)(a * regulator.abs_sum + b);
        
        // linear speed, not linear time
        timer_lim = (int)(a / (regulator.abs_sum - b) + c);
    }
    
    // setting proper direcion based on regulator sum
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
    
    // creating actual squarewave
    if (timer_cnt >= timer_lim)
    {
        timer_cnt = 0;
    }
    else 
    {
        timer_cnt += 1;
    }
    // making step
    if (on)
    {
        if ((float)timer_cnt >= (0.5 * (float)timer_lim))
        {
            gpio_put(STEP_PIN, 0);
        } 
        else
        {
            gpio_put(STEP_PIN, 1);
        }  
    }
    
    return true;
}


int main() {

    stdio_init_all();
    
    // encoder PIO initialisation
    uint offset = pio_add_program(pio, &quadrature_encoder_program);
    quadrature_encoder_program_init(pio, sm, offset, PIN_AB, 0);
    
    // linear speed, not linear time
    b = (time_min * error_max - time_max * error_min + c * (error_min - error_max)) / (time_min - time_max);
    a = (time_min - c) * (error_max - b);


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
    add_repeating_timer_us(-timer_delay, repeating_timer_callback_PTO, NULL, &timer);
    
    char incoming_char;
    char char_array[20];
    char speed_arr[10];
    char position_arr[10];

    memset(char_array, 0, sizeof(char_array));
    memset(position_arr, 0, sizeof(position_arr));
    memset(speed_arr, 0, sizeof(speed_arr));

    int char_index = 0;
    int x_index = 0;    
    int n_index = 0;
    int position_value = 0;
    float speed_value = 0.0;
    
    
    while (1) 
    {
       
       incoming_char = getchar();
       char_array[char_index] = incoming_char;
       if (incoming_char == 'n')
       {
           // find 'x' position
           char *ptr1 = strchr(char_array, 'x');
           x_index = ptr1 - char_array;

           // find 'n' position
           char *ptr2 = strchr(char_array, 'n');
           n_index = ptr2 - char_array;

           // make substring for position
           memcpy(position_arr, &char_array[0], x_index);

           // make substring for speed
           memcpy(speed_arr, &char_array[x_index + 1], n_index - x_index - 1);
           
           position_value = atoi(position_arr);
           speed_value = atof(speed_arr);

           printf("pos: %d  spe: %f", position_value, speed_value);
    
           
           // resetting everything
           char_index = 0;
           memset(char_array, 0, sizeof(char_array));
           memset(position_arr, 0, sizeof(position_arr));
           memset(speed_arr, 0, sizeof(speed_arr));
       }
       else
       {
           char_index += 1;
       }

      
       
       //data = getchar();
       //printf("%d\n", encoder_value);
       //printf("odczytane: %d enkoder: %d err: %d regulator-sum: %f loop-time: %d a: %f b: %f \n", target_value, encoder_value, position_error, regulator.sum, timer_lim, a, b);

    }
}
