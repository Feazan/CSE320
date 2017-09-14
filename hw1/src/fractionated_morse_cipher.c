#include <stdlib.h>
#include "hw1.h"

void my_insert(char *target, const char *source, int where_to_start);
int my_array_length(const char *key_str);


void encrypt_morse(unsigned short mode)
{
    fprintf(stdout, "%s\n", "INSIDE Fractionated Morse Encryption");
    char c;
    int start = 0;
    // Read a character from stdin
    while((c = getchar()) != EOF)
    {
        if(c == '\n')
            fprintf(stdout, "\n");
        else if (c == 32)
        {

            fprintf(stdout, " ");
        }
        else if (c == '\t')
            fprintf(stdout, "\t");
        else
        {
            // Get its index from the morse table
            int i = c -33;
            //printf("%s\n", *(morse_table + i));
            // Insert the morse code into the buffer (polybius table)
            my_insert(polybius_table, *(morse_table + i), start);
            start = my_array_length(*(morse_table + i));

            // Insert the x
            my_insert(polybius_table, "x", start);
            printf("%s\n", polybius_table);

        }
    }

}

void decrypt_morse(unsigned short mode)
{
    fprintf(stdout, "%s\n", "INSIDE Fractionated Morse Decryption");
}




void my_insert(char *target, const char *source, int where_to_start)
{
    while(*source != '\0')
    {
        *(target + where_to_start) = *source;
        source++;
        target++;
    }
}

int my_array_length(const char *key_str)
{
    int num_elements = 0;
    while(*key_str != '\0')
    {
        num_elements++;
        key_str++;
    }
    return num_elements;
}