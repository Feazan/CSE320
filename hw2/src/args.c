#include "debug.h"
#include "utf.h"
#include "wrappers.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int opterr;
int optopt;
int optind;
char *optarg;

state_t *program_state;

void
parse_args(int argc, char *argv[])
{
  int i;
  char option;
  char *joined_argv;

  joined_argv = join_string_array(argc, argv);
  info("argc: %d argv: %s", argc, joined_argv);
  // Something funky going on here
  //free(joined_argv);

  program_state = Calloc(1, sizeof(state_t));
  for (i = 0; optind < argc; ++i) {
    debug("%d opterr: %d\n", i, opterr);
    debug("%d optind: %d\n", i, optind);
    debug("%d optopt: %d\n", i, optopt);
    debug("%d argv[optind]: %s", i, argv[optind]);
    if ((option = getopt(argc, argv, "+e:")) != -1) {
      switch (option) {
        case 'e': {
          info("\nEncoding Argument: %s", optarg);
          if ((program_state->encoding_to = determine_format(optarg)) == 0)
            goto errorcase;
          break;
        }
        case '?': {
          if (optopt != 'h')
            fprintf(stderr, KRED "-%c is not a supported argument\n" KNRM,
                    optopt);
          // Removed the quotes around errorcase
          // Removed case from front of it and [0]
        errorcase:
          USAGE(argv[0]);
          exit(0);
        }
        default: {
          break;
        }
      }
    }
    elsif(argv[optind] != NULL)
    {
      if (program_state->in_file == NULL) {
        program_state->in_file = argv[optind];
      }
      elsif(program_state->out_file == NULL)
      {
        program_state->out_file = argv[optind];
      }
      optind++;
    }
  }
  // Something funky going on here
  free(joined_argv);
}

format_t
determine_format(char *argument)
{
  if (strcmp(argument, "UTF16LE") == 0)
    return UTF16LE;
  if (strcmp(argument, "UTF16BE") == 0)
    return UTF16BE;
  if (strcmp(argument, "UTF8") == 0)
    return UTF8;
  return 0;
}

char*
bom_to_string(format_t bom){
  switch(bom){
    case UTF8: return STR_UTF8;
    case UTF16BE: return STR_UTF16BE;
    case UTF16LE: return STR_UTF16LE;
  }
  return "UNKNOWN";
}

char*
join_string_array(int count, char *array[])
{
  //char charArray[count];
  char *ret = NULL;
  int i;
  int len = 0;
  int str_len = 0;
  int cur_str_len = 0;
  str_len = array_size(count, array);

  // not allocating memory for *ret
  ret = malloc(str_len + 1);
  // to use str_len
  //printf("%d\n", str_len);

  // Removed the & from ret = &charArray

  for (i = 0; i < count; i++) {
    cur_str_len = strlen(array[i]);
    //info("THIS IS THE VALUE OF CURR_STR_LEN %d", curr_str_len);
    memecpy(ret + len, array[i], cur_str_len);
    len += cur_str_len;
    memecpy(ret + len, " ", 1);
    len += 1;
  }

  //free(ret);
  return ret;
}

int
array_size(int count, char *array[])
{
  int i, sum = 1; /* NULL terminator */
  for (i = 0; i < count; ++i) {
    sum += strlen(array[i]);
    ++sum; /* For the spaces */
  }
  return sum+1;
}

void
print_state()
{
  // Comment out errorcase
//errorcase:
  if (program_state == NULL) {
    error("program_state is %p", (void*)program_state);
    exit(EXIT_FAILURE);
  }
  info("program_state {\n"
         "  format_t encoding_to = 0x%X;\n"
         "  format_t encoding_from = 0x%X;\n"
         "  char *in_file = '%s';\n"
         "  char *out_file = '%s';\n"
         "};\n",
         program_state->encoding_to, program_state->encoding_from,
         program_state->in_file, program_state->out_file);
}
