#include <stdlib.h>

#include "hw1.h"
#include "debug.h"

#ifdef _STRING_H
#error "Do not #include <string.h>. You will get a ZERO."
#endif

#ifdef _STRINGS_H
#error "Do not #include <strings.h>. You will get a ZERO."
#endif

#ifdef _CTYPE_H
#error "Do not #include <ctype.h>. You will get a ZERO."
#endif

int main(int argc, char **argv)
{
    unsigned short mode;

    mode = validargs(argc, argv);
    if (mode == 0)
    {
        EXIT_FAILURE;
    }

    debug("Mode: 0x%X", mode);

    if(mode & 0x8000)
    {
        USAGE(*argv, EXIT_SUCCESS);
    }
    else if (mode & 0x4000)
    {
        if(mode & 0x2000)
        {
            fprintf(stdout, "%s\n", "Fractionated Morse Decryption");
            decrypt_morse(mode);
            return EXIT_SUCCESS;
        }
        else
        {
            fprintf(stdout, "%s\n", "Fractionated Morse Encryption");
            encrypt_morse(mode);
            return EXIT_SUCCESS;
        }
    }
    else
    {
        if (mode & 0x2000)
        {
            //printf("%s\n", "Polybius Decryption");
            decrypt_polybius(mode);
            return EXIT_SUCCESS;
        }
        else if (mode & 0x00AA)
        {
            //printf("%s\n", "Polybius Encryption");
            encrypt_polybius(mode);
            return EXIT_SUCCESS;
        }
        else
        {
            return EXIT_FAILURE;
        }
    }
}

/*
 * Just a reminder: All non-main functions should
 * be in another file not named main.c
 */