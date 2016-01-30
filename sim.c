
#include "sim.h"

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

   return 1;

   argErr:
      fprintf(stderr, "Must pass in file to read.\n");
      return 0;
}



