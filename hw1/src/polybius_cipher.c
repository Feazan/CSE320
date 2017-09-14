#include <stdlib.h>
#include <debug.h>
#include "hw1.h"

void insert(char *target, char *source, int length);
int key_length(const char *key_arg);
void key_insert_to_table(char *the_table ,const char *the_key);
int contains(const char *str, char char_to_check);
void alpha_after_key_insert(const char *key_for_contains, char *the_table ,char *the_alphabet, int key_length, int alpha_length);
void insert_nulls(char *a_table, int where_to_start);
void user_input_encrypt(char *the_polybius_encryption_table, int the_cols);
void user_input_decrypt(char *the_polybius_decryption_table, int table_column);
int encrypt_rows(char *polybius_encryption_table, char p, int cols);
int encrypt_columns(char *polybius_encryption_table, char p, int cols);

void encrypt_polybius(unsigned short mode)
{
    int length_of_key = 0;
    int length_of_table = 0;
    // Parse the columns and rows from the mode
    unsigned short rows = mode & 0x00F0;
    unsigned short columns = mode & 0x000F;
    rows = (mode >> 4) & 0xF;
    length_of_table = rows * columns;

    // Lets just assume no key was passed
    if (key == NULL)
    {
        insert(polybius_table, polybius_alphabet, length_of_table);
        insert_nulls(polybius_table, length_of_table);
    }
    else
    {
        // Find the length
        length_of_key = key_length(key);

        // Insert the key into the table
        key_insert_to_table(polybius_table, key);

        // Need method for not inserting the rest of the table
        alpha_after_key_insert(key, polybius_table, polybius_alphabet, length_of_key, length_of_table);
        insert_nulls(polybius_table, length_of_table);
    }
    // Now I think I should get the users input
    user_input_encrypt(polybius_table, columns);
}

void decrypt_polybius(unsigned short mode)
{
    int length_of_key = 0;
    int length_of_table = 0;
    // Parse the columns and rows from the mode
    unsigned short rows = mode & 0x00F0;
    unsigned short columns = mode & 0x000F;
    rows = (mode >> 4) & 0xF;
    length_of_table = rows * columns;

    // Reconstructing the table
    if (key == NULL)
    {
        insert(polybius_table, polybius_alphabet, length_of_table);
        insert_nulls(polybius_table, length_of_table);
    }
    else
    {
        // Find the length
        length_of_key = key_length(key);

        // Insert the key into the table
        key_insert_to_table(polybius_table, key);

        // Need method for not inserting the rest of the table
        alpha_after_key_insert(key, polybius_table, polybius_alphabet, length_of_key, length_of_table);
        insert_nulls(polybius_table, length_of_table);
    }

    user_input_decrypt(polybius_table, columns);
}

// Copy the string from the alphabet to the source for a particular row*column length
void insert(char *target, char *source, int length)
{
    int i  = 0;
    while(i < length)
    {
        *target = *source;
        source++;
        target++;
        i++;
    }
}

// Inserts key into the alphabet
void key_insert_to_table(char *target, const char *source)
{
    while(*source != '\0')
    {
        *target = *source;
        source++;
        target++;
    }
}

// Find the length of key
int key_length(const char *key_arg)
{
    int the_length = 0;
    while (*key_arg != '\0')
    {
        the_length++;
        key_arg++;
    }
    return the_length;
}

// Cointains function
int contains(const char *str, char char_to_check)
{
    while(*str != '\0')
    {
        if(char_to_check == *str)
            return 1;
        else
            str++;
    }

    return 0;
}

// Insert alphabet into table with key
void alpha_after_key_insert(const char *key_for_contains, char *the_table ,char *the_alphabet, int key_length, int alpha_length)
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

// Method for inserting nulls into the table
void insert_nulls(char *a_table, int where_to_start)
{
    for(int i = where_to_start; i < 257; i++)
    {
        *(a_table + i) = '\0';
    }

}


// Take user input
void user_input_encrypt(char *the_polybius_encryption_table, int the_cols)
{
    char c;
    while((c = getchar()) != EOF)
    {
        if(c == '\n')
            fprintf(stdout, "\n");
        else if (c == 32)
            fprintf(stdout, " ");
        else if (c == '\t')
            fprintf(stdout, "\t");
        else
        {
            fprintf(stdout, "%X%X", encrypt_rows(the_polybius_encryption_table, c, the_cols),
                encrypt_columns(the_polybius_encryption_table, c, the_cols));
        }
    }
}

// Encryption method, takes char p for polybius
int encrypt_rows(char *polybius_encryption_table, char p, int cols)
{
    int i = 0;
    int encrypted_row = 0;
    while (*polybius_encryption_table != '\0')
    {
        if(p == *polybius_encryption_table)
        {
            encrypted_row = i/cols;
            return  encrypted_row;
        }
        else
        {
            polybius_encryption_table++;
            i++;
        }
    }
    return encrypted_row;
}

int encrypt_columns(char *polybius_encryption_table, char p, int cols)
{
    int i = 0;
    int encrypted_columns = 0;
    while (*polybius_encryption_table != '\0')
    {
        if(p == *polybius_encryption_table)
        {
            encrypted_columns = i%cols;
            return  encrypted_columns;
        }
        else
        {
            polybius_encryption_table++;
            i++;
        }
    }
    return encrypted_columns;
}

void user_input_decrypt(char *the_polybius_decryption_table, int table_column)
{
    int index_value = 0;
    int given_row = 0;
    int given_column = 0;
    int even_odd = 1;
    char c;
    while((c = getchar()) != EOF)
    {
        if(c == '\n')
        {
            fprintf(stdout, "\n");
            even_odd = 1;
        }
        else if (c == 32)
        {
            fprintf(stdout, " ");
            even_odd = 1;
        }
        else if (c == '\t')
        {
            fprintf(stdout, "\t");
            even_odd = 1;
        }
        else if ((even_odd % 2) != 0) // Meaning its odd
        {
            if (c == 'A')
            {
                given_row = 10;
                even_odd++;
            }
            else if (c == 'B')
            {
                given_row = 11;
                even_odd++;
            }
            else if (c == 'C')
            {
                given_row = 12;
                even_odd++;
            }
            else if (c == 'D')
            {
                given_row = 13;
                even_odd++;
            }
            else if (c == 'E')
            {
                given_row = 14;
                even_odd++;
            }
            else if (c == 'F')
            {
                given_row = 15;
                even_odd++;
            }
            else
            {
                given_row = (c - '0');
                even_odd++;
            }
        }
        else
        {
            if (c == 'A')
            {
                given_column = 10;
                even_odd++;
            }
            else if (c == 'B')
            {
                given_column = 11;
                even_odd++;
            }
            else if (c == 'C')
            {
                given_column = 12;
                even_odd++;
            }
            else if (c == 'D')
            {
                given_column = 13;
                even_odd++;
            }
            else if (c == 'E')
            {
                given_column = 14;
                even_odd++;
            }
            else if (c == 'F')
            {
                given_column = 15;
                even_odd++;
            }
            else
            {
                given_column = (c - '0');
                even_odd++;
            }

            index_value = (given_row * table_column) + given_column;
            fprintf(stdout, "%c", *(the_polybius_decryption_table + index_value));
        }
    }
}


/*
WITH KEY

MY OUTPUT -       37 53 39 03 04 05 90 92
EXPECTED OUTPUT - 37 53 39 03 04 05 90 92


*/


/*
WITHOUT KEY

MY OUTPUT -       375540201816A0A2
EXPECTED OUTPUT - 375540201816A0A2

TEST -----
HE AN NEW IPHONE X -- HA'S PONOUNE "EN," Y HE WAY, NO "EX" -- IS A PHONE OF FISS FO APPLE
HE AN NEW IPHONE X -- HA'S PONOUNE "EN," Y HE WAY, NO "EX" -- IS A PHONE OF FISS FO APPLE


!"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~


*/