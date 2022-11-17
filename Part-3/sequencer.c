/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <stdlib.h>

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2812.pio.h"
#include "hardware/adafruit_qtpy_rp2040.h"

#define IS_RGBW true
#define NUM_PIXELS 150

#ifdef PICO_DEFAULT_WS2812_PIN
#define WS2812_PIN PICO_DEFAULT_WS2812_PIN
#else
// default to pin 2 if the board doesn't have a default WS2812 pin defined
#define WS2812_PIN 2
#endif
#define PICO_DEFAULT_WS2812_POWER_PIN 11
#define BOOT_PIN 21

static inline void put_pixel(uint32_t pixel_grb) {
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b) {
    return
            ((uint32_t) (r) << 8) |
            ((uint32_t) (g) << 16) |
            (uint32_t) (b);
}
int replayans;
int replayip() {
    int replay_input;
    printf("Do you want to replay the recording? y/n \n");
    replay_input = getchar();
    if (replay_input == 'y'){
        return 1;
    }
    if (replay_input == 'n'){
        return 0;
    }
    else{
        return 5;
    }
}
int colorans;
int setneopixcolor() {
    int color_input;
    printf("Select the color for the NeoPixel : r/g/b \n");
    color_input = getchar();
    if (color_input == 'r'){
        return 1;
    }
    if (color_input == 'g'){
        return 2;
    }
    if (color_input == 'b'){
        return 3;
    }
    else{
        return 5;
    }
}
int brightans;
int setneopixbrightness() {
    int bright_input;
    printf("Select the brightness for the NeoPixel : l/m/h \n");
    bright_input = getchar();
    if (bright_input == 'l'){
        return 1;
    }
    if (bright_input == 'm'){
        return 2;
    }
    if (bright_input == 'h'){
        return 3;
    }
    else{
        return 5;
    }    
}

int main() {
    //set_sys_clock_48();
    stdio_init_all();
  

    // todo get free sm
    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);

    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, IS_RGBW);

    int sequence[200]={};
    for (int i=0; i<200; i++) {
        sequence[i]=0;
    }
    const uint POW_PIN = PICO_DEFAULT_WS2812_POWER_PIN;
    gpio_init(POW_PIN);
    gpio_set_dir(POW_PIN, GPIO_OUT);

    int bootpinrecord = 0;

    while (1) {

        if (bootpinrecord==0){
            if(!(gpio_get(BOOT_PIN))==1){
                sleep_ms(200);
                for (int x=0; x<200; x++){
                printf("Enter BOOT_PIN value loop\n");
                uint bootval = !(gpio_get(BOOT_PIN));
                sleep_ms(50);
                    if (bootval==1){
                        sequence[x]=1;
                    }
                    else{
                        sequence[x]=0;
                    }
                }
                bootpinrecord = 1;
            }
            sleep_ms(200);
        }
        else if (bootpinrecord==1){
        recordplay:
            replayans = replayip();
            colorans = setneopixcolor();
            brightans = setneopixbrightness();
            if (replayans == 1){
                if (colorans==1){
                    if (brightans==1){
                        for (int a=0; a<200; a++){
                            printf("Entered POW_PIN set loop\n");
                            gpio_put(POW_PIN, sequence[a]);
                            printf("Neopixel sequences\n");
                            put_pixel(urgb_u32(0x32,0x00,0x00));
                            sleep_ms(50);
                        }
                    }
                    if (brightans==2){
                        for (int a=0; a<200; a++){
                            printf("Entered POW_PIN set loop\n");
                            gpio_put(POW_PIN, sequence[a]);
                            printf("Neopixel sequences\n");
                            put_pixel(urgb_u32(0xB4,0x00,0x00));
                            sleep_ms(50);
                        }                        
                    }
                    if (brightans==3){
                        for (int a=0; a<200; a++){
                            printf("Entered POW_PIN set loop\n");
                            gpio_put(POW_PIN, sequence[a]);
                            printf("Neopixel sequences\n");
                            put_pixel(urgb_u32(0xDC,0x00,0x00));
                            sleep_ms(50);
                        }                        
                    }
                    if (brightans==5){
                        for (int a=0; a<200; a++){
                            printf("Entered POW_PIN set loop\n");
                            gpio_put(POW_PIN, sequence[a]);
                            printf("Neopixel sequences\n");
                            put_pixel(urgb_u32(0xFF,0x00,0x00));
                            sleep_ms(50);
                        }                            
                    }
                }
                if (colorans==2){
                    if (brightans==1){
                        for (int a=0; a<200; a++){
                            printf("Entered POW_PIN set loop\n");
                            gpio_put(POW_PIN, sequence[a]);
                            printf("Neopixel sequences\n");
                            put_pixel(urgb_u32(0x00,0x32,0x00));
                            sleep_ms(50);
                        }
                    }
                    if (brightans==2){
                        for (int a=0; a<200; a++){
                            printf("Entered POW_PIN set loop\n");
                            gpio_put(POW_PIN, sequence[a]);
                            printf("Neopixel sequences\n");
                            put_pixel(urgb_u32(0x00,0xB4,0x00));
                            sleep_ms(50);
                        }                        
                    }
                    if (brightans==3){
                        for (int a=0; a<200; a++){
                            printf("Entered POW_PIN set loop\n");
                            gpio_put(POW_PIN, sequence[a]);
                            printf("Neopixel sequences\n");
                            put_pixel(urgb_u32(0x00,0xDC,0x00));
                            sleep_ms(50);
                        }                        
                    }
                    if (brightans==5){
                        for (int a=0; a<200; a++){
                            printf("Entered POW_PIN set loop\n");
                            gpio_put(POW_PIN, sequence[a]);
                            printf("Neopixel sequences\n");
                            put_pixel(urgb_u32(0x00,0xFF,0x00));
                            sleep_ms(50);
                        }                            
                    } 
                }
                if (colorans==3){
                    if (brightans==1){
                        for (int a=0; a<200; a++){
                            printf("Entered POW_PIN set loop\n");
                            gpio_put(POW_PIN, sequence[a]);
                            printf("Neopixel sequences\n");
                            put_pixel(urgb_u32(0x00,0x00,0x32));
                            sleep_ms(50);
                        }
                    }
                    if (brightans==2){
                        for (int a=0; a<200; a++){
                            printf("Entered POW_PIN set loop\n");
                            gpio_put(POW_PIN, sequence[a]);
                            printf("Neopixel sequences\n");
                            put_pixel(urgb_u32(0x00,0x00,0xB4));
                            sleep_ms(50);
                        }                        
                    }
                    if (brightans==3){
                        for (int a=0; a<200; a++){
                            printf("Entered POW_PIN set loop\n");
                            gpio_put(POW_PIN, sequence[a]);
                            printf("Neopixel sequences\n");
                            put_pixel(urgb_u32(0x00,0x00,0xDC));
                            sleep_ms(50);
                        }                        
                    }
                    if (brightans==5){
                        for (int a=0; a<200; a++){
                            printf("Entered POW_PIN set loop\n");
                            gpio_put(POW_PIN, sequence[a]);
                            printf("Neopixel sequences\n");
                            put_pixel(urgb_u32(0x00,0x00,0xFF));
                            sleep_ms(50);
                        }                            
                    }
                }
                if (colorans==5){
                    if (brightans==1){
                        for (int a=0; a<200; a++){
                            printf("Entered POW_PIN set loop\n");
                            gpio_put(POW_PIN, sequence[a]);
                            printf("Neopixel sequences\n");
                            put_pixel(urgb_u32(0xFF,0xB6,0xC1));
                            sleep_ms(50);
                        }
                    }
                    if (brightans==2){
                        for (int a=0; a<200; a++){
                            printf("Entered POW_PIN set loop\n");
                            gpio_put(POW_PIN, sequence[a]);
                            printf("Neopixel sequences\n");
                            put_pixel(urgb_u32(0xF3,0x61,0x96));
                            sleep_ms(50);
                        }                        
                    }
                    if (brightans==3){
                        for (int a=0; a<200; a++){
                            printf("Entered POW_PIN set loop\n");
                            gpio_put(POW_PIN, sequence[a]);
                            printf("Neopixel sequences\n");
                            put_pixel(urgb_u32(0xFF,0x10,0xF0));
                            sleep_ms(50);
                        }                        
                    }                    
                    if (brightans==5){
                        for (int a=0; a<200; a++){
                            printf("Entered POW_PIN set loop\n");
                            gpio_put(POW_PIN, sequence[a]);
                            printf("Neopixel sequences\n");
                            put_pixel(urgb_u32(0xC7,0x15,0x85));
                            sleep_ms(50);
                        }  
                    }
       
                }
  
            }
            if(replayans==0){
                printf("Recording is not played as per input!\n");
            }
            if(replayans==5){
                printf("Enter a valid input : 'y' or 'n' \n");
                goto recordplay;
            }
            bootpinrecord=0;
        
        goto recordplay;
        }
    }
}
