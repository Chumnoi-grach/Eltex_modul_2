#include <string.h>
#include <ctype.h>
#include <regex.h>

#ifndef VALIDATOR_H
#define VALIDATOR_H

int validate_name(const char *name);

int validate_phone(const char *phone);

int validate_email(const char *email);

#endif