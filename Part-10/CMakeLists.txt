cmake_minimum_required(VERSION 3.13)
#include(pico_sdk_import.cmake)

project(adps_protocol C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()

add_executable(servo_proximity)

pico_generate_pio_header(servo_proximity ${CMAKE_CURRENT_LIST_DIR}/i2c.pio)

pico_enable_stdio_usb(servo_proximity 1)
pico_enable_stdio_uart(servo_proximity 0)

target_sources(servo_proximity PRIVATE
        servo_proximity.c
        registers.h
        apds_registers.h
        )

target_link_libraries(servo_proximity PRIVATE pico_stdlib hardware_pio)
pico_add_extra_outputs(servo_proximity)

# add url via pico_set_program_url
# example_auto_set_url(servo_proximity)
