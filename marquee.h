#ifndef __MARQUEE_H__
#define __MARQUEE_H__

typedef enum
{
    e_dmux1 = 0,
    e_dmux2 = 1,
    e_dmux3,
    e_dmux4,
    e_dmux5,
    e_dmux6,
    e_dmux7,
    e_dmux8
}DmuxType;

typedef enum {
    e_row1 = 1,
    e_row2 = 2,
    e_row3 = 3,
    e_row4 = 4,
    e_row5 = 5,
    e_row6 = 6,
    e_row7 = 7,
    e_row8 = 8,
    e_row9 = 9
}RowNum;

void turnAllRowsOn();
void turnAllRowsOff();
void configureMarqueeLedPins();
void loopColumns(); // meant to be called repeatedly in a loop

#endif