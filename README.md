# Arduino Life
This is another clone of John Conway's Game of Life for arduino created mostly
for the [Freetronics dot matrix display](http://www.freetronics.com/dmd).

Life.cpp also compiles in visual studio for debugging and is flexible enough
to work with any screen using a rendering function.

# Things i normally wouldn't do

## Use bytes and chars for everything
The atmega328 which is at the core of my Uno based board is an 8 bit 
microcontroller. As far as i know instead of doing fast single instruction
maths 16 bit integers are maniplated with compiler magic using many 
instructions. So where a byte is enough that is what i have used.

## Weird static allocation

The Life class takes a bit map as the argument to it's constructor. This is
due to the dilemma of not having memory management. The class could have 
declared the bitmap itself however it would then need to be modified for 
every application which would lead to fragmented versions.
