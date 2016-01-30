
#include "sim.h"

#define COL_BORDER 50

WINDOW * init_gui();
void init_colours();

void
init_colours()
{
   init_pair(50, COLOR_YELLOW, COLOR_BLACK);
   init_pair(0, COLOR_WHITE, COLOR_BLACK);
   init_pair(1, COLOR_GREEN, COLOR_BLACK);
}

WINDOW *
init_gui (WORLD *world)
{

   // Establish ncurses.
   initscr();

   // Hide cursor and cursor feedback.
   noecho();
   curs_set(FALSE);

   // Enable function keys to be registered by ncurses.
   keypad(stdscr, TRUE);

   // Enable non-blocking input.
   timeout(0);   

   // Enable colours.
   if (has_colors() == FALSE) {
      fprintf(stderr, "No colour - no exiting.");
      exit(1);   
   }
   start_color();
   init_colours();

   // Set up window.
   WINDOW *window_menu = newwin(World_Height(world) + 2, World_Width(world) + 2, 0, 0);
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
draw_board (WINDOW *window, WORLD *world)
{

   wattron(window, COLOR_PAIR(COL_BORDER));
   wborder(window, '|', '|', '-', '-', '+', '+', '+', '+');
   wattroff(window, COLOR_PAIR(COL_BORDER));

   for (int row = 0; row < World_Height(world); row++) {
      for (int col = 0; col < World_Width(world); col++) {

         // Get cell.
         int cell = World_CellAt(world, row, col);

         // Set color and character according to cell.
         wattron(window, COLOR_PAIR(cell));
         char toPrint;
         if (!cell) toPrint = '.';
         else toPrint = 'o';

         // Print cell, turn off colour.
         mvwaddch(window, row + 1, col + 1, toPrint); 
         wattroff(window, COLOR_PAIR(cell));

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

   while (1) {
      draw_board(window, world);
      usleep(500000);
      World_Iterate(world);  
   }

   end_gui(window);

   return 0;

   argErr:
      fprintf(stderr, "Must pass in file to read.\n");
      return 1;
}



