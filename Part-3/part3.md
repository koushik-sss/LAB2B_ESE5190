# Sequencer
## The Sequencer maps the 'BOOT' button strokes to the 'Neopixel LED' output
- A recorded sequence of the BOOT button presses is taken in an array which is basically the RAM of the QtPy RP2040.
- The sequence is replayed back on the Neopixel LED by mapping the BOOT PIN to the Neopixel LED POW PIN
- The recording of the sequence of BOOT button strokes is stored on the laptop using the Python Serial Library
- A recording from the laptop is played on the RP2040 

https://user-images.githubusercontent.com/69215958/202358621-4df6bdfc-44e3-44d9-9295-43dbbbecd811.mp4


  Recorded BOOT presses plays on the Neopixel LED : "_ _ _ ... _ _ _"
