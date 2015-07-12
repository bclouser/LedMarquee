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


// For rightside up
typedef enum {
    rowsOff = 0x000,
    row1 = 0x100,
    row2 = 0x080,
    row3 = 0x040,
    row4 = 0x020,
    row5 = 0x010,
    row6 = 0x008,
    row7 = 0x004,
    row8 = 0x002,
    row9 = 0x001,
    allRows = 0x1FF
}RowNum;


/*
// For upside down
typedef enum {
    rowsOff = 0x000,
    row1 = 0x001,
    row2 = 0x002,
    row3 = 0x004,
    row4 = 0x008,
    row5 = 0x010,
    row6 = 0x020,
    row7 = 0x040,
    row8 = 0x080,
    row9 = 0x100,
    allRows = 0x1FF
}RowNum;
*/


void putFrame(RowNum* buf);
void turnAllRowsOn();
void turnAllRowsOff();
void configureMarqueeLedPins();
void loopColumns(); // meant to be called repeatedly in a loop
void loopGrid();

#endif