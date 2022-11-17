/*

Create a REPL to let you read and write RP2040 registers from a console. You should be able to:
1) select any 32-bit address to read/write (even if not a valid RP2040 address)
2) read/write any 32-bit value to this address
3) read/write using any of the atomic bit-setting aliases and a 32-bit mask
4) THe rp2040 is connected to the computer via a USB serial port. You can use the USB serial port to send commands to the RP2040 and receive responses.
*/


#include "pico/stdlib.h"
#include "hardware/regs/addressmap.h"
#include "hardware/regs/sio.h"
#include "hardware/regs/xosc.h"

#include "hardware/irq.h"
#include "hardware/structs/sio.h"
#include "hardware/structs/xosc.h"

#include "hardware/uart.h"
#include "hardware/gpio.h"
#include "hardware/regs/uart.h"
#include<stdio.h>

// function select any 32-bit address to read/write (even if not a valid RP2040 address) from console
uint32_t read_write_address(uint32_t address) {
    printf("\n Enter address to read/write: ");
    printf("\n Example, if you want to access the INTENSET register, enter 0x50200004");
    scanf("%x", &address);
    printf("\n Address: %x\n", address);
    return address;
}

// function to ask the user for read/write operation

uint32_t read_write_operation(uint32_t address) {
    char operation;
    printf("\n Enter operation to perform (r/w): \n");
    scanf(" %c", &operation);
    if (operation == 'r') {
        printf("\n Value at address %x: %x\n", address, *(volatile uint32_t *)address);
    } else if (operation == 'w') {
        uint32_t value;
        printf("\n Enter value to write: ");
        scanf("%x", &value);
        *(volatile uint32_t *)address = value;
        printf("\n Value at address %x: %x\n", address, *(volatile uint32_t *)address);
    } else {
        printf("\n Invalid operation\n");
    }
    return address;
}

// function to ask the user for read/write operation using any of the atomic bit-setting aliases and a 32-bit mask

uint32_t read_write_operation_mask(uint32_t address) {
    char operation;
    printf("\n Enter operation to perform (r/w): ");
    scanf(" %c", &operation);
    if (operation == 'r') {
        uint32_t mask;
        printf("Enter mask: ");
        scanf("%x", &mask);
        printf("\n Value at address %x: %x\n", address, *(volatile uint32_t *)address & mask);
    } else if (operation == 'w') {
        uint32_t value;
        printf("\n Enter value to write: ");
        scanf("%x", &value);
        uint32_t mask;
        printf("\n Enter mask: ");
        scanf("%x", &mask);
        *(volatile uint32_t *)address = (*(volatile uint32_t *)address & ~mask) | (value & mask);
        printf("\n Value at address %x: %x\n", address, *(volatile uint32_t *)address);
    } else {
        printf("\n Invalid operation\n");
    }
    return address;
}


int main() {
    printf("\n Hello, world!\n");
    stdio_init_all();
    printf("\n Hello, world!\n");
    printf("\n Hello, world!\n");

    uint32_t address = 0;
    while (true) {
        address = read_write_address(address);
        if (address == 0x40000000) {
            address = read_write_operation_mask(address);
        } else {
            address = read_write_operation(address);
        }
    }
}
