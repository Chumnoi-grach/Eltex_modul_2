#ifndef INPUT_H
#define INPUT_H

#include "input.h"



void clear_input(void);

int get_int(const char *request);

void get_string(const char *request, char *buffer, int size);

void get_parameters(const char *request, char *buffer);

char get_char(void);


#endif