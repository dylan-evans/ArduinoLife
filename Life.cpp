#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "Life.h"

#define getCell(L, A, X, Y)   ((L[((Y) * (A)) + ((X) / 8)] & (1 << ((X) % 8))) ? 1 : 0)
#define setCell(L, A, X, Y)   (L[((Y) * (A)) + ((X) / 8)] |= (1 << (X) % 8))
#define clearCell(L, A, X, Y) (L[((Y) * (A)) + ((X) / 8)] &= ~(1 << (X) % 8))


/**
 * Initialize the Life instance. Because of the limitations of the
 * microcontroller environment (no malloc) a pointer to the bitmap used
 * by the class must be passed in the constructor. The bitmap must contain
 * enough space to represent the screen twice.
 * 
 * @param framespace A pointer to the bitmap
 * @param width The width of the grid
 * @param height The height of the grid
 */
Life::Life(byte *framespace, byte width, byte height)
{
    this->width = width;
    this->height = height;
	this->align = width / 8 + (width % 8 ? 1 : 0);
    this->size = this->align * height;
	
    memset(framespace, '\0', this->size * 2);
    prev = framespace;
    current = framespace + this->size;
    hIndex = HISTORY_SIZE;
}

/**
 * Flip current frame to previous. The remaining frame is then reused as the
 * current frame and for efficiency is not cleared.
 */
void Life::flip()
{
    byte *tmp = prev;
    prev = current;
    current = tmp;
}

/**
 * Zero out the current bitmap.
 */
void Life::clear()
{
    memset(this->current, '\0', this->size);
}

/**
 * Set the current frame to random values.
 */
void Life::randomize()
{
    for(int i = 0; i < this->size; i++)
    {    
        this->current[i] = rand() % 0xFF;
    }
}

/**
 * Check each cell for changes and write the appropriate value using
 * the function passed as the argument.
 * @param all False to only draw cells which have changed.
 * @param renderPixel The function which performs the actual write.
 */
void Life::render(bool all, void (*renderPixel)(byte x, byte y, bool set))
{
    for(int row = 0; row < this->height; row++)
    {
        for(int col = 0; col < this->width; col++)
        {
			int val = getCell(this->current, this->align, col, row);
			if(all || getCell(this->prev, this->align, col, row) != val)
			{
				if(val)
					renderPixel(col, row, true);
				else
					renderPixel(col, row, false);
			}
        }
    } 
}

/**
 * Advance the simulation by one move.
 */
bool Life::step()
{
    int changes = 0;
    
    flip();
    
    for(int row = 0; row < this->height; row++)
    {
        for(int col = 0; col < this->width; col++)
        {
			byte alive = getCell(this->prev, this->align, col, row);
            switch(calculateCell(col, row))
            {
                case 3:
					// Cell becomes alive
                    if(!alive) changes++;
					setCell(this->current, this->align, col, row);
                    break;
                case 2:
					// Maintains the current state
                    if(alive)
                        setCell(this->current, this->align, col, row);
                    else
						clearCell(this->current, this->align, col, row);
                    break;
                default:
					// Clear the cell
                    if(alive) changes++;
                    clearCell(this->current, this->align, col, row);
            }
        }
    }
    if(loopCheck(changes))
    {
        return false;
    }
    return true;
}

/**
 * Calculate the number of neighbouring cells
 */
byte Life::calculateCell(byte col, byte row)
{
    byte tally = 0;
    for(char xmod = -1; xmod < 2; xmod++)
    {
        for(char ymod = -1; ymod < 2; ymod++)
        {
            char x = col + xmod;
            char y = row + ymod;
			if((!xmod && !ymod) || x < 0 || x >= this->width || y < 0 || y >= this->height) 
                continue;
            if(getCell(this->prev, this->align, x, y)) tally++;
        }
    }
    return tally;
}

/**
 * Check for a repeating pattern of changes.
 * @return True if a loop is found.
 */
bool Life::loopCheck(byte changes)
{
    hIndex--;
    history[hIndex] = changes;
    if(hIndex <= 0)
    {
        hIndex = HISTORY_SIZE; // V important
        for(byte range = 1; range < 4; range++)
        {
            int match = 0;
            for(byte i = 0; i < range; i++)
            {
                byte cur = history[i];
                if(cur == history[i+range] && cur == history[i+(range*2)]) 
					match++;
            }
            if(match == range) return true;
        }
    }
    return false;
}    
