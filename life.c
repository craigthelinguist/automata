

#include "life.h"

#define INDEX(r,c) r*World_Height(world)+c

typedef enum { DEAD, ALIVE } Cell;

int NeighboursAlive (WORLD *world, uint16_t row, uint16_t col);
int RightOf (WORLD *world, uint16_t row, uint16_t col);
int LeftOf (WORLD *world, uint16_t row, uint16_t col);
int TopOf (WORLD *world, uint16_t row, uint16_t col);
int BottomOf (WORLD *world, uint16_t row, uint16_t col);

int TopLeftOf (WORLD *world, uint16_t row, uint16_t col);
int TopRightOf (WORLD *world, uint16_t row, uint16_t col);
int BottomLeftOf (WORLD *world, uint16_t row, uint16_t col);
int BottomRightOf (WORLD *world, uint16_t row, uint16_t col);

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
         int idx = INDEX(row, col);
         int cell = World_CellAt(world, row, col);

         // Under or over population kills a live cell.
         if (cell && neighbours < 2)
            new_board[idx] = DEAD;
         else if (cell && neighbours > 3)
            new_board[idx] = DEAD;
         else if (cell)
            new_board[idx] = ALIVE;
         else if (!cell && neighbours == 3)
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
   if (TopOf(world, row, col)) count++;
   if (BottomOf(world, row, col)) count++;
   if (TopLeftOf(world, row, col)) count++;
   if (TopRightOf(world, row, col)) count++;
   if (BottomLeftOf(world, row, col)) count++;
   if (BottomRightOf(world, row, col)) count++;
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

int TopOf (WORLD *world, uint16_t row, uint16_t col)
{
   if (row > 0)
      return World_CellAt(world, row - 1, col);
   char wraparound = World_WrapsVertically(world);
   return !wraparound ? DEAD : World_CellAt(world, World_Height(world) - 1, col);
}

int BottomOf (WORLD *world, uint16_t row, uint16_t col)
{
   if (row < World_Height(world) - 1)
      return World_CellAt(world, row + 1, col);
   char wraparound = World_WrapsVertically(world);
   return !wraparound ? DEAD : World_CellAt(world, 0, col);
}

int TopLeftOf (WORLD *world, uint16_t row, uint16_t col)
{
   if (row > 0 && col > 0)
      return World_CellAt(world, row - 1, col - 1);
   char wraparound_top = World_WrapsVertically(world);
   char wraparound_side = World_WrapsHorizontally(world);

   if ((!row && !wraparound_top)
       || (!col && !wraparound_side))
      return DEAD;

   int newrow = row == 0 ? World_Height(world) - 1 : row - 1;
   int newcol = col == 0 ? World_Width(world) - 1 : col - 1;
   return World_CellAt(world, newrow, newcol);

}

int TopRightOf (WORLD *world, uint16_t row, uint16_t col)
{
   if (row > 0 && col < World_Width(world) - 1)
      return World_CellAt(world, row - 1, col + 1);
   char wrap_top = World_WrapsVertically(world);
   char wrap_side = World_WrapsHorizontally(world);
   
   if ((!row && !wrap_top)
       || (col == World_Width(world) - 1 && !wrap_side))
      return DEAD;

   uint16_t newrow = row == 0 ? World_Height(world) - 1 : row - 1;
   uint16_t newcol = col == World_Width(world) - 1 ? 0 : col + 1;
   return World_CellAt(world, newrow, newcol);

}

int BottomLeftOf (WORLD *world, uint16_t row, uint16_t col)
{
   if (row < World_Height(world) - 1 && col > 0)
      return World_CellAt(world, row + 1, col - 1);
   char wrap_top = World_WrapsVertically(world);
   char wrap_side = World_WrapsHorizontally(world);

   if ((!col && !wrap_side)
       || (row == World_Height(world) - 1 && !wrap_top))
      return DEAD;

   uint16_t newrow = row == World_Height(world) - 1 ? 0 : row + 1;
   uint16_t newcol = col == 0 ? World_Width(world) - 1 : col - 1;
   return World_CellAt(world, newrow, newcol);
}

int BottomRightOf (WORLD *world, uint16_t row, uint16_t col)
{
   if (row < World_Height(world) - 1 && col < World_Width(world) - 1)
      return World_CellAt(world, row + 1, col + 1);
   char wrap_top = World_WrapsVertically(world);
   char wrap_side = World_WrapsHorizontally(world);

   if ((col == World_Width(world) - 1 && !wrap_side)
       || (row == World_Height(world) - 1 && !wrap_top))
      return DEAD;

   uint16_t newrow = row == World_Height(world) - 1 ? 0 : row + 1;
   uint16_t newcol = col == World_Width(world) - 1 ? 0 : col + 1;
   return World_CellAt(world, newrow, newcol);


}




