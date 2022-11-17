# Slow Motion
## The Sequencer maps the 'BOOT' button strokes to the 'Neopixel LED' output and takes input from the user about speed of output required
- A recorded sequence of the BOOT button presses is taken in an array which is basically the RAM of the QtPy RP2040.
- An input of whether a slow replay(delay of 150ms)/fast replay(delay of 30ms) is needed is taken from the user
- The sequence is replayed back on the Neopixel LED by mapping the BOOT PIN to the Neopixel LED POW PIN based on the speed input
- The recording of the sequence of BOOT button strokes is stored on the laptop using the Python Serial Library
- A recording from the laptop is played on the RP2040 



  Recorded BOOT presses plays on the Neopixel LED : "_ _ _ ... _"
