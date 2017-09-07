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

int validargs_helper(char *arguments , char char_to_check);

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

    // Not sure what the purpose of this is
    int i;
    // The local variable that will return final mode of operation
    unsigned short mode_of_operation = 0x0000;
    printf("The unsigned short to return: 0x%04x\n", mode_of_operation);

    // If there are not enough arguments
    if (argc <= 1 || argc > MAXVALUE)
    {
        printf("%s\n", "Incorrect Number of Arguments");
        // Could possibly return EXIT_FAILURE message
        return mode_of_operation;
    }
    else
    {
        // If  there were more than one argument passed
        if(argc >= 2)
        {
            // Highest order bit
            // This is the first argument after the name of the program
            if(validargs_helper(*((argv+1)), 'h'))
                return 0x8000;
            else if (validargs_helper(*((argv+1)), 'p'))
            {
                //OR the second highest order bit with 0
                mode_of_operation |= 0x0000;
            }
            // This will check if there are more arguments to check
            else if (validargs_helper(*((argv+1)), 'f'))
            {
                //OR the second highest order bit with 1
                mode_of_operation |= 0x4000;
            }
        }
        // If there were more than 2 arguments passed
        if(argc >= 3)
        {
            // Second order bit
            if (validargs_helper(*((argv+2)), 'e'))
            {
                //code to flip necessary bits
                mode_of_operation |= 0x0000;
            }
            else if (validargs_helper(*((argv+2)), 'd'))
            {
                mode_of_operation |= 0x2000;

            }
            //*((argv+1)) +1 moves to next element in argv array NOT string array
        }
        //If there were more than 3 arguments passed
        if(argc >= 4)
        {
            if (validargs_helper(*((argv+3)), 'k'))
            {
                // The argument after this should be the key.
                // Which can be longer than 2
                // Need to send it to another function to validate the key

            }
            else if (validargs_helper(*((argv+3)), 'r'))
            {
                // This means that the arguments that follows is the
                // Numer of rows

            }
            else if (validargs_helper(*((argv+3)), 'c'))
            {
                // This means that the arguments that follows should
                // be the number of columns
            }
        }
    }

    printf("The mode of operation is: 0x%x\n", mode_of_operation);
    return mode_of_operation;
}

/**
 * The validargs_helper function is passed an arugment of a pointer
 * to an array of chars (aka a string) As well as a single char to check whether
 * the array of chars contains the char_to_check
 *
 * The loop cycles through the array, checking each element with the element
 * that we are looking for to see there is a match.
 *
 * If there is a match the fucntion will return 1
 * Otherwise it will return 0 for false
 */
int validargs_helper(char *arguments , char char_to_check)
{
    //this refereces the p
    //arguments+2  the +2 moves over to the next pointer ie the next element in the argv array
    //*(*(arguments+2)+1)) the +1 means to look at the next element in the string array
    //printf("%c\n", *(*(arguments+2)+1));


    while(*arguments != '\0')
    {
        if(char_to_check == *arguments)
            return 1;
        else
            arguments++;
    }

    return 0;
}

/**
 * The validate_key function is used as a helper function to  validargs
 * This function is passed the argument  following the -k command and will
 * check whether the key meets  the requirements and will return 1 to indicate
 * that it does or return 0 to indicate that the key is not valid
 */
int validate_key(char *key)
{

}