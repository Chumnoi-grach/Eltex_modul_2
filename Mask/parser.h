#ifndef PARSER_H
#define PARSER_H

#include <sys/stat.h>
#include <sys/types.h>

mode_t parse_permissions(const char* input);

int is_string_permissions(const char* input);

int is_octal_permissions(const char* input);

#endif