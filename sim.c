
#include "sim.h"

#define COL_BORDER 50
#define COL_ALIVE 1
#define COL_DEAD 2

WINDOW * init_gui();
void init_colours();

void
init_colours()
{
   init_pair(COL_BORDER, COLOR_YELLOW, COLOR_BLACK);
   init_pair(COL_ALIVE, COLOR_GREEN, COLOR_BLACK);
   init_pair(COL_DEAD, COLOR_WHITE, COLOR_BLACK);
}

WINDOW *
init_gui (WORLD *world)
{

   // Establish ncurses.
   initscr();

   // Hide cursor and cursor feedback.
   noecho();
   curs_set(FALSE);

   // Enable colours.
   if (has_colors() == FALSE) {
      fprintf(stderr, "No colour - no exiting.");
      exit(1);   
   }
   start_color();
   init_colours();

   // Set up window.
   uint64_t width, height;
   getmaxyx(stdscr, height, width);
   WINDOW *window_menu = newwin(height, width, 0, 0);
   
   // Enable non-blocking input.
   wtimeout(window_menu, 0);

   // Done.
   return window_menu; 

}

void
end_gui (WINDOW *window)
{
   wclear(window);
   delwin(window);
   endwin();
}

void
my_draw_border (WINDOW *window, uint16_t left, uint16_t top, WORLD *world)
{
   
   wattron(window, COLOR_PAIR(COL_BORDER));

   uint64_t width = World_Width(world);
   uint64_t height = World_Height(world);

   uint64_t right = left + width + 1;
   uint64_t bottom = top + height + 1;
   
   // Top of border.
   for (uint64_t col = left; col <= right; col++) {
      if (col == left || col == right)
         mvwaddch(window, top, col, '+');
      else
         mvwaddch(window, top, col, '-');
   }
   
   // Bottom of border.
   for (uint64_t col = left; col <= right; col++) {
      if (col == left || col == right)
         mvwaddch(window, bottom, col, '+');
      else
         mvwaddch(window, bottom, col, '-');
   }

   // Left of border.
   for (uint64_t row = top; row <= bottom; row++) {
      if (row == top || row == bottom)
         mvwaddch(window, row, left, '+');
      else
         mvwaddch(window, row, left, '|');
   }

   // Right of border.
   for (uint64_t row = top; row <= bottom; row++) {
      if (row == top || row == bottom)
         mvwaddch(window, row, right, '+');
      else
         mvwaddch(window, row, right, '|');
   }

   wattroff(window, COLOR_PAIR(COL_BORDER));

}

void
redraw_board (WINDOW *window, WORLD *world)
{

   // Clear the window.
   wclear(window);

   // Offsets to place it into the middle.
   uint16_t width, height;
   getmaxyx(window, height, width);
   uint16_t left = width / 4;
   uint16_t top = height / 4;
   my_draw_border(window, left, top, world);

   // Draw the board.
   for (uint16_t row = 0; row < World_Height(world); row++) {
      for (uint16_t col = 0; col < World_Width(world); col++) {

         // Get cell.
         int cell = World_CellAt(world, row, col);
         char c = '.';
         int color = COL_DEAD;

         // If cell is alive, set color and char appropriately.
         if (cell != 0) {
            c = 'o';
            color = COL_ALIVE;
         }

         // Print cell.
         wattron(window, COLOR_PAIR(color));
         mvwaddch(window, top + col + 1, left + row + 1, c);
         wattroff(window, COLOR_PAIR(color));

      }   
   }

   wrefresh(window);

}

int
main (int argc, char **argv)
{

   // Check the file exists.
   if (argc != 2) goto argErr;
   char *fname = argv[1];

   // Load the world.
   WORLD *world = Parse_World(fname);

   // Specify rules for this automaton.
   World_SetRules(world, &GameOfLife);

   WINDOW *window = init_gui(world);

   long UPDATE_DELAY = 250000;

   while (1) {
      redraw_board(window, world);
      usleep(UPDATE_DELAY);
      World_Iterate(world);  
   }

   end_gui(window);

   return 0;

   argErr:
      fprintf(stderr, "Must pass in file to read.\n");
      return 1;
}



