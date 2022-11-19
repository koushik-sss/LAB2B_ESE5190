# Protoboard Circuit - Sequencer integration
## The servo motor is programmed using pio

- When a proximity reading of 1 is received by the QTPy RP2040 from the APDS9960 sensor, the servo motor connected to the A1 pin (GPIO28) is rotated to 180 degrees
- The servo motor is then brought back to initial state of 0 degrees
- If the proximity reading is 0, no action is taken

  Video of the working protoboard:

https://user-images.githubusercontent.com/69215958/202834385-9013beb2-22c9-495a-990c-33a64e721c2f.mp4
