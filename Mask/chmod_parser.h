#ifndef CHMOD_PARSER_H
#define CHMOD_PARSER_H

#include <sys/stat.h>
#include <sys/types.h>

mode_t apply_chmod_modification(mode_t current, const char* mod);

#endif