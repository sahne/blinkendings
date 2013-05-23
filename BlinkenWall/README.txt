1) copy library to Sketchbook directory -> libraries
2) reopen Arduino IDE
3) import library


This library depends on the TI Tlc5940 library.
For installation instructions please visit

http://playground.arduino.cc/learning/TLC5940

newer version might require other and/or additional
header-files to be included.

After installing Tlc5940 make sure to edit tlc_config.h

set NUM_TLCS to 18 and TLC_CHANNEL_TYPE to uint16_t

For more information on how to create a animation
please read the comments in BlinkenWall.h

see examples/ for more inforamtion and exmaples.
