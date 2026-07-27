#ifndef PERMISSIONS_H
#define PERMISSIONS_H

#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <stdio.h>

char* mode_to_string(mode_t mode);

int mode_to_octal(mode_t mode);

char* mode_to_binary(mode_t mode);


int get_file_permissions(const char* filename, mode_t* mode);

void print_permissions(mode_t mode);

#endif