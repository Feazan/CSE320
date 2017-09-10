#include <stdlib.h>
#include "hw1.h"

#ifdef _STRING_H
#error "Do not #include <string.h>. You will get a ZERO."
#endif

#ifdef _STRINGS_H
#error "Do not #include <strings.h>. You will get a ZERO."
#endif

#ifdef _CTYPE_H
#error "Do not #include <ctype.h>. You will get a ZERO."
#endif

#define MAXVALUE 10

int validargs_helper(char *arguments , char *str_to_check);
int int_validation(char *int_str);
int array_length(char *key_str);
int key_validation(char *key_str, int count);
int cipher_check(char *key_str, char cipher);


/**
 * @brief Validates command line arguments passed to the program.
 * @details This function will validate all the arguments passed to the program
 * and will return a unsigned short (2 bytes) that will contain the
 * information necessary for the proper execution of the program.
 *
 * IF -p is given but no (-r) ROWS or (-c) COLUMNS are specified this function
 * MUST set the lower bits to the default value of 10. If one or the other
 * (rows/columns) is specified then you MUST keep that value rather than assigning the default.
 *
 * @param argc The number of arguments passed to the program from the CLI.
 * @param argv The argument strings passed to the program from the CLI.
 * @return Refer to homework document for the return value of this function.
 */
unsigned short validargs(int argc, char **argv)
{
    // The local variable that will return final mode of operation
    unsigned short mode_of_operation = 0x0000;
    int optional_arg_pos;
    int k_found = 0;
    int r_found = 0;
    int c_found = 0;
    int p_found = 0;
    int f_found = 0;
    int row_value = 160;
    int col_value = 10;
    int number_of_elements = 0;
    int is_key_valid = 0;
    char cipher_mode;
    int cipher_check_int = 0;

    // If there are not enough arguments
    if (argc <= 1 || argc > MAXVALUE)
    {
        printf("%s\n", "Incorrect Number of Arguments");
        // Could possibly return EXIT_FAILURE message
        return 0x8000;
    }
    else
    {
        if (argc == 2 && !validargs_helper(*((argv+1)), "-h"))
        {
            return 0x0000;
        }
        // If  there were more than one argument passed
        if(argc >= 2)
        {
            // Highest order bit
            // This is the first argument after the name of the program
            if(validargs_helper(*((argv+1)), "-h"))
                return 0x8000;
            else if (validargs_helper(*((argv+1)), "-p"))
            {
                p_found++;
                //OR the second highest order bit with 0
                mode_of_operation |= 0x0000;
            }
            // This will check if there are more arguments to check
            else if (validargs_helper(*((argv+1)), "-f"))
            {
                f_found++;
                //OR the second highest order bit with 1
                mode_of_operation |= 0x4000;
            }
            else
            {
                return mode_of_operation = 0x0000;
            }
        }
        // If there were more than 2 arguments passed
        if(argc >= 3)
        {
            // Second order bit
            if (validargs_helper(*((argv+2)), "-e"))
            {
                //code to flip necessary bits
                mode_of_operation |= 0x0000;
            }
            else if (validargs_helper(*((argv+2)), "-d"))
            {
                mode_of_operation |= 0x2000;

            }
            else
            {
                return mode_of_operation = 0x0000;
            }
            //*((argv+1)) +1 moves to next element in argv array NOT string array
        }
        //If there were more than 3 arguments passed check for optional arguments
        if(argc >= 4)
        {
            for (optional_arg_pos = 3; optional_arg_pos < argc; optional_arg_pos++)
            {
                if (validargs_helper(*((argv+optional_arg_pos)), "-k"))
                {
                    // Code to validate key
                    // Signal that K was found
                    k_found++;
                    if (p_found)
                    {
                        cipher_mode = 'p';
                    }
                    else if (f_found)
                    {
                        cipher_mode = 'f';
                    }

                    // Pass cipher_mode to cipher_check if it returns true
                    cipher_check_int = cipher_check(*((argv + optional_arg_pos + 1)), cipher_mode);
                    if (cipher_check_int == 1)
                    {
                        // If it was in bounds, set the count for the number of elements
                        number_of_elements = array_length(*((argv + optional_arg_pos + 1)));
                        // Check for repeats
                        is_key_valid = key_validation(*((argv + optional_arg_pos + 1)), number_of_elements);
                    }
                    else
                    {
                        return mode_of_operation = 0x0000;
                    }

                    if (is_key_valid == 1 && cipher_mode == 'p')
                    {
                        // Store the key in the -p variable
                        printf("%s\n", "STORE IN -p VARIABLE");
                        // TODO: Find out if this is the correct way to store the variable
                        key = *((argv + optional_arg_pos + 1));
                    }
                    else if (is_key_valid == 1 && cipher_mode == 'f')
                    {
                        // Store the key in the -f variable
                        printf("%s\n", "STORE IN -f VARIABLE");
                        // TODO: Find out if this is the correct way to store the variable
                        key = *((argv + optional_arg_pos + 1));
                    }
                    else if (is_key_valid == 0)
                    {
                        printf("%s\n", "KEY IS INVALID");
                        return mode_of_operation = 0x0000;
                    }

                }
                else if (validargs_helper(*((argv+optional_arg_pos)), "-r"))
                {
                    // Signal that rows was found
                    r_found++;

                    if (r_found > 1)
                    {
                        mode_of_operation = 0x0000;
                        printf("Mode is :%d\n", mode_of_operation);
                        return mode_of_operation;
                    }
                    // Code to validate rows send it to helper function
                    // If there is a -r detected, pass the following argument
                    // and validate that it is correct
                    row_value = int_validation(*((argv + optional_arg_pos + 1)));
                    printf("The row value is: %d\n", row_value);
                     // If the value that was returned was invalid, return 0;
                    if(row_value == 0)
                    {
                        mode_of_operation = 0x0000;
                        printf("Mode is :%d\n", mode_of_operation);
                        return mode_of_operation;
                    }
                    else // Flip the necessaey bits
                    {
                        // Multiply by 16 to "shift" to high end
                        row_value *= 16;
                        mode_of_operation |= row_value;
                        printf("The value of mode is: 0x%x\n", mode_of_operation);
                    }

                }
                else if (validargs_helper(*((argv+optional_arg_pos)), "-c"))
                {
                    // Signal that rows was found
                    c_found++;

                    if (c_found > 1)
                    {
                        mode_of_operation = 0x0000;
                        printf("Mode is :%d\n", mode_of_operation);
                        return mode_of_operation;
                    }
                    // Code to validate columns
                    // Signal that columns was found
                    col_value = int_validation(*((argv+optional_arg_pos + 1)));
                    printf("The column value is: %d\n", col_value);
                     // If the value that was returned was invalid, return 0;
                    if(col_value == 0)
                    {
                        mode_of_operation = 0x0000;
                        printf("Mode is :%d\n", mode_of_operation);
                        return mode_of_operation;
                    }
                    else // Flip the necessaey bits
                    {
                        // OR with mode_of_operations
                        mode_of_operation |= col_value;
                        printf("The value of mode is: 0x%x\n", mode_of_operation);
                    }
                }
                else if (!k_found && !r_found && !c_found)
                    return mode_of_operation = 0x0000;
            }
        }

        if(argc >= 3)
        {
            // If I reach this point I should set default values for rows and columns
            mode_of_operation |= row_value;
            mode_of_operation |= col_value;
        }
    }

    printf("The mode of operation is: 0x%x\n", mode_of_operation);
    return mode_of_operation;
}

/**
 * The validargs_helper function is passed an arugment of a pointer
 * to an array of chars (aka a string) As well as a string to check whether
 * the array of chars and *str_to_check are equal
 *
 * The loop cycles through both arrays, checking each element with the element
 * that we are looking for to see there is a match.
 *
 * If there is a match the fucntion will return 1
 * Otherwise it will return 0 for false
 */
int validargs_helper(char *arguments , char *str_to_check)
{
    //this refereces the p
    //arguments+2  the +2 moves over to the next pointer ie the next element in the argv array
    //*(*(arguments+2)+1)) the +1 means to look at the next element in the string array
    //printf("%c\n", *(*(arguments+2)+1));
    while(*arguments == *str_to_check)
    {
        if(*arguments == '\0' || *str_to_check == '\0')
            break;

        arguments++;
        str_to_check++;
    }

    if (*arguments == '\0' && *str_to_check == '\0')
        return 1; // TRUE
    else
        return 0; // FALSE
}

// Int validation
int int_validation(char *int_str)
{
    int int_to_check;

    // Turn string to int
    int_to_check = atoi(int_str);
    printf("The number that was converted from a String: %d\n", int_to_check);
    // If the return value of atoi does not indicate a number return 0
    if (int_to_check == 0)
    {
        return 0;
    }
    else if(int_to_check < 9 || int_to_check > 15) // Check if its in the right bounds
    {
        return 0;
    }

    // Return the into to the function.
    return int_to_check;
}

// Key helper
int array_length(char *key_str)
{
    int num_elements = 0;
    while(*key_str != '\0')
    {
        num_elements++;
        key_str++;
    }
    return num_elements;
}

// cipher mode helper
int cipher_check(char *key_str, char cipher)
{
    while(*key_str != '\0')
    {
        if (cipher == 'p')
        {
            if (*key_str < 33 || *key_str > 126)
                return 0; // Out of bound for polybus cipher
        }
        if (cipher == 'f')
        {
            if (*key_str < 65 || *key_str > 90)
            {
                printf("%s\n", "INVALID KEY");
                return 0; // Out of bound for morse code cipher
            }
        }
        key_str++;
    }
    return 1; // If this point is reached the keys are in bounds
}

// Key validation
int key_validation(char *key_str, int count)
{
    for(int i = 0; i < count; i++) {
        for(int j = i + 1; j < count; j++)
        {
            if (*(key_str + i) == *(key_str + j))
            {
                return 0; // FAILED VALIDATION
            }
        }
    }
    return 1; // PASSED VALIDATION
}

