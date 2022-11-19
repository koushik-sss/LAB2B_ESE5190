#include <stdio.h>
#include <stdlib.h>

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"

#include "registers.h"
#include "apds_registers.h"
#include "hardware/adafruit_qtpy_rp2040.h"

#define PIN_SDA 24
#define PIN_SCL 25

// The GPIO pin to which the servo motor is connected
#define SERVO_PIN 28

// The minimum angle of the servo motor
#define SERVO_MIN_ANGLE 0

// The maximum angle of the servo motor
#define SERVO_MAX_ANGLE 180

// The minimum pulse width of the servo motor
#define SERVO_MIN_PULSE_WIDTH 1000

// The period of the pulse train
#define SERVO_PERIOD 20000

// The number of pulses in the pulse train
#define SERVO_NUM_PULSES 100

// function to initialize the servo motor
void servo_init() {
    // Initialize the GPIO pin
    gpio_init(SERVO_PIN);
    gpio_set_dir(SERVO_PIN, GPIO_OUT);

    // Initialize the PIO state machine
    uint offset = pio_add_program(pio0, &servo_program);
    servo_program_init(pio0, 0, offset, SERVO_PIN, SERVO_NUM_PULSES, SERVO_PERIOD);
}

// function to set the angle of the servo motor
void servo_set_angle(uint angle) {
    // Calculate the pulse width corresponding to the angle
    uint pulse_width = SERVO_MIN_PULSE_WIDTH + (angle * (SERVO_MAX_PULSE_WIDTH - SERVO_MIN_PULSE_WIDTH)) / (SERVO_MAX_ANGLE - SERVO_MIN_ANGLE);

    // Set the pulse width
    servo_program_set_pulse_width(pio0, 0, pulse_width);
}

//function to set the pulse width of the servo motor
void servo_set_pulse_width(uint pulse_width) {
    // Set the pulse width
    servo_program_set_pulse_width(pio0, 0, pulse_width);
}

//function to set the period of the pulse train
void servo_set_period(uint period) {
    // Set the period
    servo_program_set_period(pio0, 0, period);
}

//function to set the number of pulses in the pulse train
void servo_set_num_pulses(uint num_pulses) {
    // Set the number of pulses
    servo_program_set_num_pulses(pio0, 0, num_pulses);
}

//function to set the GPIO pin to which the servo motor is connected
void servo_set_pin(uint pin) {
    // Set the GPIO pin
    servo_program_set_pin(pio0, 0, pin);
}

bool reserved_addr(uint8_t addr) {
    return (addr & 0x78) == 0 || (addr & 0x78) == 0x78;
}

void config_adps(PIO pio, uint sm){

    uint8_t txbuf[2] = {0};

    // Config the Cotrol Register.
    txbuf[0] = ADPS_CONTROL_ONE_REGISTER;
    txbuf[1] = ADPS_CONTROL_ONE_AGAIN;
    pio_i2c_write_blocking(pio, sm, ADPS_ADDRESS, txbuf, 2);

    // Enable Ambient Light and Proximity Sensor
    txbuf[0] = ADPS_ENABLE_REGISTER;
    txbuf[1] = ADPS_ENABLE_PON | ADPS_ENABLE_AEN | ADPS_ENABLE_PEN;
    pio_i2c_write_blocking(pio, sm, ADPS_ADDRESS, txbuf, 2);
}

void adps_read(PIO pio, uint sm, uint8_t reg_addr, uint8_t *rxbuf, uint num_bytes) {
    // Read from `reg_addr`.
    pio_i2c_write_blocking(pio, sm, ADPS_ADDRESS, &reg_addr, 1);  
    pio_i2c_read_blocking(pio, sm, ADPS_ADDRESS, rxbuf, num_bytes);
}

int main() {
    stdio_init_all();
    //Initialize servo motor
    servo_init();
    // Set the angle of the servo motor
    servo_set_angle(90);
    // Set the pulse width of the servo motor
    servo_set_pulse_width(1500);
    // Set the period of the pulse train
    servo_set_period(20000);
    // Set the number of pulses in the pulse train
    servo_set_num_pulses(100);
    // Set the GPIO pin to which the servo motor is connected
    servo_set_pin(28);
  
    PIO pio = pio0;
    uint sm = 0;
    uint offset = pio_add_program(pio, &i2c_program);
    i2c_program_init(pio, sm, offset, PIN_SDA, PIN_SCL);
   
    printf("Starting PIO I2C ADPS9960 Interface\n");
    
    // Start I2C commuinication.
    /* pio_i2c_start(pio, sm); */

    // Configure the ADPS Sensor.
    config_adps(pio, sm);
    pio_gpio_init(pio,PIN_A1);
    gpio_set_dir(PIN_A1, GPIO_OUT);
    while(1) {
        
        // Check the status register, to know if we can read the values
        // from the ALS and Proximity engine.
        uint8_t rxbuf[1] = {0};
        adps_read(pio, sm, STATUS_REGISTER, rxbuf, 1);
        adps_read(pio, sm, ID_REGISTER, rxbuf, 1);

        // Use the mask to check if our Proximity and color data is ready to be read.
        uint8_t data_arr[8] = {0};
        if ((rxbuf[0] & STATUS_REGISTER_PVALID) == STATUS_REGISTER_PVALID) {
            adps_read(pio, sm, PROXIMITY_DATA_REGISTER, data_arr, 1);
            printf("The Proximity Data : %d\n", data_arr[0] - 230);
        } 
        if ((data_arr[0]-230)==1){
          //rotate the servo motor from 0 to 180 degrees
          for (uint angle = 0; angle <= 180; angle++) {
              servo_set_angle(angle);
              sleep_ms(20);
          }

          //rotate the servo motor from 180 to 0 degrees
          for (uint angle = 180; angle >= 0; angle--) {
              servo_set_angle(angle);
              sleep_ms(20);
          }
        }
        sleep_ms(500); 
    }
    return 0;
}
