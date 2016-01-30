
#ifndef PARSER_H
#define PARSER_H

   #include <ctype.h>
   #include <stdlib.h>
   #include <stdio.h>
   #include <ncurses.h>
   #include <unistd.h>
   #include "life.h"
   #include "world.h"

   WORLD *Parse_World (char *fname);

#endif

