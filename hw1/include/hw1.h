#ifndef HW_H
#define HW_H

#include "const.h"


void encrypt_polybius(unsigned short mode);
void decrypt_polybius(unsigned short mode);
int array_length(char *key_str);
void encrypt_morse(unsigned short mode);
void decrypt_morse(unsigned short mode);
void insert(char *target, char *source, int length);
void key_insert_to_table(char *target, const char *source);
void alpha_after_key_insert(const char *key_for_contains, char *the_table ,char *the_alphabet, int key_length, int alpha_length);
int key_length(const char *key_arg);
int contains(const char *str, char char_to_check);

#endif
