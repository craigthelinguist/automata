
#include "parser.h"

// Helper macros.
// ======================================================================

#define SKIP skip(contents,idx,len);
#define NUM parse_num(contents,idx,len);
#define PARSE_ERR(msg) parse_err(contents,idx,len,msg);



// Function declarations.
// ======================================================================

void parse_err (char *contents, int *idx, long len, char *msg);
void skip (char *contents, int *idx, long len);
int parse_num (char *contents, int *idx, long len);
void init_world (WORLD *world, char *contents, long len);



// Public functions.
// ======================================================================

WORLD *
Parse_World (char *fname)
{
   
   // Check file exists.
   if (access(fname, F_OK) == -1) goto ioErr;
   printf("loading %s\n", fname);

   // Open file, figure out length.
   char *buffer = NULL;
   long length;
   FILE *f = fopen (fname, "rb");
   if (!f) goto ioErr;
   fseek(f, 0, SEEK_END);
   length = ftell(f);
   fseek(f, 0, SEEK_SET);
   buffer = malloc (length + 1);
   if (!buffer) goto ioErr;

   // Copy file contents into a string and parse the program.
   fread (buffer, sizeof(char), length, f);
   buffer[length] = '\0';

   // Initialise data structure.
   WORLD *world = malloc (World_SizeOf());
   init_world(world, buffer, length);
   return world;

   ioErr:
      fprintf(stderr, "Error loading file: %s.\n", fname);
      abort();

}


// Private functions.
// ======================================================================

void parse_err (char *contents, int *idx, long len, char *msg)
{
   fprintf(stderr, "Error parsing file: %s\n", msg);
   abort();
}

void skip (char *contents, int *idx, long len)
{
   int index = *idx;
   while (index < len && isspace(contents[index]))
      index++;
   *idx = index;
}

int parse_num (char *contents, int *idx, long len)
{
   SKIP;
   int index = *idx;
   int end_index = index;
   while (end_index < len && isdigit(contents[end_index]))
      end_index++;
   if (end_index == index)
      PARSE_ERR("Expected a number but didn't get one.");
   int num = atoi(contents + index);
   *idx = end_index;
   SKIP;
   return num;

}

void init_world (WORLD *world, char *contents, long len)
{

   // Initialise parsing index.
   int *idx = malloc(sizeof(int));
   *idx = 0;

   // Parse header.
   int width = NUM;
   int height = NUM;

   World_New (world, width, height, 1, 1);

   // Parse board.
   for (uint16_t row = 0; row < height; row++) {
      int i = *idx;
      for (uint16_t col = 0; col < width; col++) {

         // Check there is something to parse.
         if (i >= len || !isdigit(contents[i]))
            PARSE_ERR("Error parsing file.\n");

         // Load into world.
         int cell = contents[i] - 48; // convert ASCII value to int
         World_SetCell(world, row, col, cell);

         // Increment index.
         i++;
      }
      *idx = i;
      SKIP; // Skip end of line character.
   }

   // Deallocate.
   free(idx);

}










