
Tom's Constant Current Load
tom.biskupic@gmail.com

The source code here implements a constant current dummy load based on a MOSFET and a power resistor.

The code uses a Hitachi LCD for display, buttons and a shaft encoder for input. A Microchip MCP4921 is used as a DAC to set the current output and 4.096V voltage reference keeps it all stable.

The code supports pulse and sawtooth modes.

There is some INL compensation in the LoadControl class. You will have to customise this for your hardware.

This code uses this library for controlling the DAC from here
https://github.com/exscape/electronics/tree/master/Arduino/Libraries/DAC_MCP49xx

This code uses this shaft encoder library for reading the encoder
http://www.pjrc.com/teensy/td_libs_Encoder.html

See here for more details on the circuit, software design and general operation
http://wahz.blogspot.com.au/2015/03/dummy-load.html

Software is provided as-is with no warranty.

The software can be copyed as a whole or in part with no limitations.

Enjoy!

