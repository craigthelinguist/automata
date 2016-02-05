
#ifndef World_H
#define World_H
   #include <stdint.h>
   #include <stdlib.h>
#endif

typedef struct world WORLD;
typedef void (*AutomataFunc)(WORLD *world);

int
World_SizeOf ();

void
World_New (WORLD *world,
           uint16_t width, uint16_t height,
           char horizontal_wrap, char vertical_wrap);

void
World_Del (WORLD *world);

int
World_CellAt (WORLD *world,
             uint16_t row, uint16_t col);

void
World_SetRules (WORLD *world, AutomataFunc func);

void
World_SetCell (WORLD *world, int row, int col, int newValue);

void
World_Iterate (WORLD *world);

char
World_WrapsVertically (WORLD *world);

char
World_WrapsHorizontally (WORLD *world);

uint16_t
World_Height (WORLD *world);

uint16_t
World_Width (WORLD *world);



