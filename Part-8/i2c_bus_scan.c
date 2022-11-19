#include <stdio.h>
#include <stdlib.h>

#include "pico/stdlib.h"
#include "pio_i2c.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"

#include "registers.h"
#include "apds_registers.h"
#include "hardware/adafruit_qtpy_rp2040.h"

#define PIN_SDA 22
#define PIN_SCL 23

bool reserved_addr(uint8_t addr) {
    return (addr & 0x78) == 0 || (addr & 0x78) == 0x78;
}

void config_adps(PIO pio, uint sm){

    uint8_t txbuf[2] = {0};

    // Set Color Integration time to `50` => 256 - 50 = 206 = 0xCE
    txbuf[0] = ATIME_REGISTER;
    txbuf[1] = (uint8_t)(0x81);
    pio_i2c_write_blocking(pio, sm, ADPS_ADDRESS, txbuf, 2);

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

    PIO pio = pio0;
    uint sm = 0;
    uint offset = pio_add_program(pio, &i2c_program);
    i2c_program_init(pio, sm, offset, PIN_SDA, PIN_SCL);
   
    printf("Starting PIO I2C ADPS9960 Interface\n");
    
    // Start I2C commuinication.
    /* pio_i2c_start(pio, sm); */

    // Configure the ADPS Sensor.
    config_adps(pio, sm);

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
        if ((rxbuf[0] & STATUS_REGISTER_AVALID) == STATUS_REGISTER_AVALID) {
            adps_read(pio, sm, RGBC_DATA_REGISTER_CDATAL, data_arr, 8);
            uint16_t c_val = (data_arr[1] << 8 | data_arr[0]); 
            uint16_t r_val = (data_arr[3] << 8 | data_arr[2]); 
            uint16_t g_val = (data_arr[5] << 8 | data_arr[4]); 
            uint16_t b_val = (data_arr[7] << 8 | data_arr[6]); 
            printf("The Color Data : (%d, %d, %d, %d)\n", r_val, g_val, b_val, c_val);
        }

        sleep_ms(500); 
    }
 
    return 0;
}
