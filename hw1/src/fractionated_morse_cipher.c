#include <stdlib.h>
#include "hw1.h"

void my_insert(char *target, const char *source);
int buffer_length_met(char *the_buffer);
void fm_alpha_after_key_insert(const char *key_for_contains, char *the_table ,const char *the_alphabet, int key_length, int alpha_length);
void insert_at_end(char to_enter);
int index_of_x(char* str, char c);

int index_to_place_next = 0;

void encrypt_morse(unsigned short mode)
{
    long space = 0;
    char *temp_buff = (char*)&space;
    char c;
    // Read a character from stdin
    while((c = getchar()) != EOF)
    {
        if(c == '\n')
            fprintf(stdout, "\n");
        else if (c == ' ')
        {
            insert_at_end('x');
        }
        else if (c == '\t')
            fprintf(stdout, "\t");
        else
        {
            // Get its index from the morse table
            int i = c -33;
            /*
            Insert, then call function  to check whether the size
            meets the qualifications or not.

            if it returns 0 (false) insert again and check
            [MAYBE WHILE THE FUNCTION DOES NOT RETURN 0]
            if it returns 1 (true)
                    call another function to parse out the first three
                    elements from the function  and do the printing.
            */

            //printf("%s\n", *(morse_table + i));
            // Insert the morse code into the buffer (polybius table)
            my_insert((polybius_table + index_to_place_next), *(morse_table + i));
            //index_to_place_next++;
            //my_insert((polybius_table + index_to_place_next), "x");
            insert_at_end('x');

            // Are there at least 3 characters
            int has_three = buffer_length_met(polybius_table);

            index_to_place_next = array_length(polybius_table);


        }
    }
    insert_at_end('x');

    //printf("%d\n", index_of_x(polybius_table, 'x'));
    //printf("%s\n", polybius_table);

    int length_of_key = key_length(key);
    int length_of_table = key_length(fm_alphabet);

    key_insert_to_table(fm_key, key);
    fm_alpha_after_key_insert(key, fm_key, fm_alphabet, length_of_key, length_of_table);

    printf("%s\n", fm_key);


}

void decrypt_morse(unsigned short mode)
{
    //fprintf(stdout, "%s\n", "INSIDE Fractionated Morse Decryption");
}




void my_insert(char *target, const char *source)
{
    while(*source != '\0')
    {
        *target = *source;
        source++;
        target++;
    }
}

int buffer_length_met(char *the_buffer)
{
    if (array_length(the_buffer) < 3)
        return 0; // FALSE
    else
        return 1; // TRUE
}

void fm_alpha_after_key_insert(const char *key_for_contains, char *the_table ,const char *the_alphabet, int key_length, int alpha_length)
{
    int i = 0;
    for (i = key_length; i < alpha_length; i++)
    {
        if (!contains(key_for_contains, *(the_alphabet)))
        {
            *(the_table + i) = *the_alphabet;
        }
        else
        {
            --i;
        }
        the_alphabet++;
    }
}

void insert_at_end(char to_enter)
{
    int length = array_length(polybius_table);
    *(polybius_table + length) = to_enter;
    index_to_place_next++;
}

int index_of_x(char* str, char c)
{
    int x = 0;
    int idx = -1;
    while (*str++ != c) idx++;

    if (x != -1 )
    {
        return  idx +1;
    }

    return idx;
}


void three_morse(char *buffer)
{
    *(buffer) = *(polybius_table);
    *(buffer + 1) = *(polybius_table + 1);
    *(buffer + 2) = *(polybius_table + 2);
}
