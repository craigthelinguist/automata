

#include "life.h"

#define INDEX(r,c) r*World_Height(world)+c

typedef enum { DEAD, ALIVE } Cell;


int NeighboursAlive (WORLD *world, uint16_t row, uint16_t col);
int RightOf (WORLD *world, uint16_t row, uint16_t col);
int LeftOf (WORLD *world, uint16_t row, uint16_t col);
int AboveOf (WORLD *world, uint16_t row, uint16_t col);
int BelowOf (WORLD *world, uint16_t row, uint16_t col);


void
GameOfLife (WORLD *world)
{

   // World parameters.
   uint16_t width = World_Width(world);
   uint16_t height = World_Height(world);

   // Allocate memory to store state of new board.
   char *new_board = malloc(sizeof (int) * width * height);

   // Figure out state of the new world.
   for (int row = 0; row < height; row++) {
      for (int col = 0; col < width; col++) {

         // Get info about current cell.
         int neighbours = NeighboursAlive(world, row, col);
         int cell = World_CellAt(world, row, col);
         int idx = INDEX(row, col);

         // Under or over population kills the cell.
         if (cell && (neighbours == 1 || neighbours == 4))
            new_board[idx] = DEAD;

         // Three neighbours give life to a dead cell.
         if (!cell && neighbours == 3)
            new_board[idx] = ALIVE;

      }
   }

   // Update the simulation.
   for (int row = 0; row < height; row++) {
      for (int col = 0; col < width; col++) {
         int idx = INDEX(row, col);
         World_SetCell(world, row, col, new_board[idx]);
      }
   }

   // Free memory we allocated.
   free(new_board);

}

int NeighboursAlive (WORLD *world, uint16_t row, uint16_t col)
{

   // Note this depends on alive cells having a non-zero integer value.
   int count = 0;
   if (RightOf(world, row, col)) count++;
   if (LeftOf(world, row, col)) count++;
   if (AboveOf(world, row, col)) count++;
   if (BelowOf(world, row, col)) count++;
   return count;
}

int RightOf (WORLD *world, uint16_t row, uint16_t col)
{
   if (col < World_Width(world) - 1)
      return World_CellAt(world, row, col + 1);   
   char wraparound = World_WrapsHorizontally(world);
   return !wraparound ? DEAD : World_CellAt(world, row, 0);
}

int LeftOf (WORLD *world, uint16_t row, uint16_t col)
{
   if (col > 0)
      return World_CellAt(world, row, col - 1);
   char wraparound = World_WrapsHorizontally(world);
   return !wraparound ? DEAD : World_CellAt(world, row, World_Width(world) - 1);
}

int AboveOf (WORLD *world, uint16_t row, uint16_t col)
{
   if (row > 0)
      return World_CellAt(world, row - 1, col);
   char wraparound = World_WrapsVertically(world);
   return !wraparound ? DEAD : World_CellAt(world, World_Height(world) - 1, col);
}

int BelowOf (WORLD *world, uint16_t row, uint16_t col)
{
   if (row < World_Height(world) - 1)
      return World_CellAt(world, row + 1, col);
   char wraparound = World_WrapsHorizontally(world);
   return !wraparound ? DEAD : World_CellAt(world, 0, col);
}


