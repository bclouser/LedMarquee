
#include <stdint.h>
#include <stdbool.h>
#include "driverlib/uart.h"
#include "driverlib/rom_map.h"
#include "inc/hw_memmap.h"


static char *heap_end = 0;
extern unsigned long _heap_bottom;
extern unsigned long _heap_top;

// I kind of hate typedefs like this
typedef char* caddr_t;

caddr_t _sbrk(unsigned int incr)
{
    char *prev_heap_end;
    if (heap_end == 0) {
        heap_end = (caddr_t)&_heap_bottom;            //1
    }
    prev_heap_end = heap_end;                         //2
    if (heap_end + incr > (caddr_t)&_heap_top) {      //3
        return (caddr_t)0;                            //4
    }
    heap_end += incr;
    return (caddr_t) prev_heap_end;                   //5
}

int _close(int file){ return -1; }

int _fstat(int file){ return -1; }

int _isatty(int file){ return -1; }

int _lseek(int file, int ptr, int dir){ return -1; }

int _open(const char *name, int flags, int mode){ return -1; }

int _read(int file, char *ptr, int len){ return -1; }

int _write(int file, char *ptr, unsigned int len){
    unsigned int i;
    for(i = 0; i < len; i++){
        MAP_UARTCharPut(UART0_BASE, ptr[i]);
    }
    return i;
}