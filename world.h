
#ifndef World_H
#define World_H
   #include <stdint.h>
   #include <stdlib.h>
#endif

typedef struct world WORLD;
typedef void (*AutomataFunc)(struct world *world);

void
World_NewWorld (struct world *world,
               uint16_t width, uint16_t height,
               char horizontal_wrap, char vertical_wrap,
               AutomataFunc automata_rules);

void
World_DelWorld (WORLD *world);

int
World_CellAt (WORLD *world,
             uint16_t row, uint16_t col);

void
World_SetCell (struct world *world, int row, int col, int newValue);

void
World_Iterate (WORLD *world);

char
World_WrapsVertically (struct world *world);

char
World_WrapsHorizontally (struct world *world);

uint16_t
World_Height (struct world *world);

uint16_t
World_Width (struct world *world);



