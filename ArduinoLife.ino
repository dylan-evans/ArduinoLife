#include <SPI.h>
#include <DMD.h>
#include <TimerOne.h>

#include "Life.h"

byte framespace[128];
Life grid(framespace, 32, 16);
DMD dmd(1, 1);

void scanDMD()
{
    dmd.scanDisplayBySPI();
}

void renderPixel(byte x, byte y, bool set)
{
    if(set)
        dmd.writePixel(x, y, GRAPHICS_NORMAL, true);
    else
        dmd.writePixel(x, y, GRAPHICS_INVERSE, true);
}

void setup()
{
    Timer1.initialize(5000);
    Timer1.attachInterrupt(scanDMD);
    Serial.begin(9600);

    dmd.clearScreen(true);
    //randomSeed(42);
    srand(analogRead(1));
    
    grid.randomize();
}

void loop()
{
    //dmd.clearScreen(true);
    grid.render(renderPixel);
    delay(100);
    if(!grid.step())
    {
        for(int i = 0; i < 3; i++)
        {
            // Flash the screen
            dmd.clearScreen(true);
            delay(150);
            grid.render(renderPixel);
            delay(300);
        }    
        grid.randomize();
    }
}

