
#include "world.h"

// Struct and function declarations.
// ================================================================================

struct world {
   uint16_t width;
   uint16_t height;
   int *contents;
   char horizontal_wrap;
   char vertical_wrap;
   AutomataFunc automata_func;   
};


void Bounds_Check (struct world *world, uint16_t row, uint16_t col);


// Public functions.
// ================================================================================


int
World_SizeOf ()
{
   return sizeof (struct world);
}

void
World_New (struct world *world,
           uint16_t width, uint16_t height,
           char horizontal_wrap, char vertical_wrap)
{
   world->width = width;
   world->height = height;
   world->horizontal_wrap = horizontal_wrap;
   world->vertical_wrap = vertical_wrap;
   world->contents = malloc(sizeof(int) * width * height);
}

void
World_SetRules (struct world *world, AutomataFunc func)
{
   world->automata_func = func;
}

void
World_Del (struct world *world)
{
   free(world->contents);
   free(world);
}

inline int
World_CellAt (struct world *world,
              uint16_t row, uint16_t col)
{
   Bounds_Check(world, row, col);
   uint16_t width = world->width;
   return world->contents[row*width + col];
}

#include <stdio.h>

inline void
World_SetCell (struct world *world, int row, int col, int newValue)
{
   Bounds_Check(world, row, col);
   uint16_t width = world->width;
   world->contents[row*width + col] = newValue;
}

inline void
World_Iterate (struct world *world)
{
   AutomataFunc iter = world->automata_func;
   (*iter)(world);
}

inline char
World_WrapsHorizontally (struct world *world)
{
   return world->horizontal_wrap;
}

inline char
World_WrapsVertically (struct world *world)
{
   return world->vertical_wrap;
}

inline uint16_t
World_Width (struct world *world)
{
   return world->width;
}

inline uint16_t
World_Height (struct world *world)
{
   return world->height;
}



// Internal functions.
// ================================================================================

inline void
Bounds_Check (struct world *world,
              uint16_t row, uint16_t col)
{
   if (row < 0 || col < 0 || row >= world->height || col >= world->width)
      abort();
}

