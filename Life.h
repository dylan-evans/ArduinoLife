#ifndef _LIFE_H_
#define _LIFE_H_

#define byte unsigned char

#define HISTORY_SIZE 12

class Life 
{
    public:
        byte width;
        byte height;
        byte align;
        int size;

        Life(byte *framespace, byte width, byte height);
        void flip();
        void clear();
        void randomize();
        void render(bool all, void (*renderPixel)(byte x, byte y, bool set));
        bool step();
    
    private:
        byte *prev, *current;
        byte history[HISTORY_SIZE];
        byte hIndex;
    
        byte calculateCell(byte col, byte row);
        bool loopCheck(byte changes);
};

#endif /* _LIFE_H_ */
