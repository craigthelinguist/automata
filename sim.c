
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
   WINDOW *window_menu = newwin(World_Height(world) + 2, World_Width(world) + 2, 0, 0);
   
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
redraw_board (WINDOW *window, WORLD *world)
{

   // Clear the window.
   wclear(window);

   // Draw the border.
   wattron(window, COLOR_PAIR(COL_BORDER));
   wborder(window, '|', '|', '-', '-', '+', '+', '+', '+');
   wattroff(window, COLOR_PAIR(COL_BORDER));

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
         mvwaddch(window, row + 1, col + 1, c);
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



