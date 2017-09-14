#ifndef HW_H
#define HW_H

#include "const.h"


void encrypt_polybius(unsigned short mode);
void decrypt_polybius(unsigned short mode);
int array_length(char *key_str);
void encrypt_morse(unsigned short mode);
void decrypt_morse(unsigned short mode);

#endif
