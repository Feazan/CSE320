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
int key_validation(char *key_str);

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
    int row_value = 160;
    int col_value = 10;

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
                //OR the second highest order bit with 0
                mode_of_operation |= 0x0000;
            }
            // This will check if there are more arguments to check
            else if (validargs_helper(*((argv+1)), "-f"))
            {
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
                    printf("%s\n", "KEY");
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
int key_validation(char *key_str)
{
    return 0;

}

