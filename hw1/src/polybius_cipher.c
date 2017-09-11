#include <stdlib.h>
#include "hw1.h"

void string_copy(char *target, char *source);

void polybius(unsigned short mode)
{
    printf("MODE WAS RECIEVED INSIDE POLYBIUS_CIPHER.C %x\n", mode);
    // Lets just assume no key was passed
    string_copy(polybius_table, polybius_alphabet);
    printf("%s\n", polybius_table);

    // Now I think I should get the users input
    char *user_input = "";
    while (fgets(user_input, 64, stdin))
    {
        printf("User Input: %s\n", user_input);
    }



}

// Copy the string from the alphabet to the source
void string_copy(char *target, char *source)
{
    while(*source != '\0')
    {
        *target = *source;
        source++;
        target++;
    }
}