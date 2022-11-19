The code is modified to enable the RP2040 to read the value from the APDS9960 via the PIO. We took the help of Ruturaj's submission to understand the code better.

1) We first set the color integration time and configure the control register. Later, the light and proximity sensors are enabled, after which we read from the address and write the corresponding data, stored in the uint16_t representation.
The mask is later used to check if the prox and color values can be transmitted, after which they are printed on the console.

Output:


https://github.com/koushik-sss/LAB2B_ESE5190/blob/main/Part9/WhatsApp%20Video%202022-11-19%20at%2000.21.49.mp4?raw=true
