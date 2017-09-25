#include "debug.h"
#include "utf.h"
#include "wrappers.h"
#include <stdlib.h>

int
main(int argc, char *argv[])
{
  int infile, outfile, in_flags, out_flags;
  parse_args(argc, argv);
  in_flags = O_RDONLY;
  out_flags = O_WRONLY | O_CREAT | O_TRUNC;
  infile = Open(program_state->in_file, in_flags);
  outfile = Open(program_state->out_file, out_flags);

  if(get_inode(infile) == get_inode(outfile))
  {
    return (EXIT_FAILURE);
  }
  else
  {
    check_bom();
    print_state();

    lseek(SEEK_SET, program_state->bom_length, infile); /* Discard BOM */
    get_encoding_function()(infile, outfile);
    if(program_state != NULL) {
      close(infile);
      close(outfile); //maybe infile (intptr_t)program_state
    }

    debug("EVERYTHING IS DONE");
    //I think this is how this works
    close(infile);
    close(outfile);

    free(program_state);

    return EXIT_SUCCESS;
  }
}
