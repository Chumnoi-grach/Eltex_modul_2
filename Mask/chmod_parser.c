#include "chmod_parser.h"
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>


static int parse_single_mod(const char* part, mode_t* to_add, mode_t* to_remove, mode_t* to_set) {
    if (strlen(part) < 3) {
        return 0;
    }
    
    mode_t who = 0;
    int pos = 0;
    
    while (part[pos] && part[pos] != '+' && part[pos] != '-' && part[pos] != '=') {
        switch (part[pos]) {
            case 'u': who |= S_IRUSR | S_IWUSR | S_IXUSR; break;
            case 'g': who |= S_IRGRP | S_IWGRP | S_IXGRP; break;
            case 'o': who |= S_IROTH | S_IWOTH | S_IXOTH; break;
            case 'a': who |= S_IRUSR | S_IWUSR | S_IXUSR | 
                             S_IRGRP | S_IWGRP | S_IXGRP |
                             S_IROTH | S_IWOTH | S_IXOTH; break;
            default: return 0;
        }
        pos++;
    }
    
    if (who == 0 || part[pos] == '\0') {
        return 0;
    }
    
    char op = part[pos];
    pos++;
    
    mode_t perms = 0;
    while (part[pos]) {
        switch (part[pos]) {
            case 'r': perms |= S_IRUSR | S_IRGRP | S_IROTH; break;
            case 'w': perms |= S_IWUSR | S_IWGRP | S_IWOTH; break;
            case 'x': perms |= S_IXUSR | S_IXGRP | S_IXOTH; break;
            default: return 0;
        }
        pos++;
    }
    
    if (op == '+') {
        *to_add |= who & perms;
    } else if (op == '-') {
        *to_remove |= who & perms;
    } else if (op == '=') {
        *to_set |= who & perms;
        *to_remove |= who & ~perms;
    } else {
        return 0;
    }
    
    return 1;
}

mode_t apply_chmod_modification(mode_t current, const char* mod) {
    mode_t to_add = 0;
    mode_t to_remove = 0;
    mode_t to_set = 0;
    
    char mod_copy[256];
    strncpy(mod_copy, mod, sizeof(mod_copy) - 1);
    mod_copy[sizeof(mod_copy) - 1] = '\0';
    
    char* token = strtok(mod_copy, ",");
    while (token != NULL) {
        while (*token == ' ' || *token == '\t') token++;
        char* end = token + strlen(token) - 1;
        while (end > token && (*end == ' ' || *end == '\t')) {
            *end = '\0';
            end--;
        }
        
        if (!parse_single_mod(token, &to_add, &to_remove, &to_set)) {
            return 0;
        }
        
        token = strtok(NULL, ",");
    }
    
    mode_t result = current;
    
    result &= ~to_remove;
    result |= to_add;
    result |= to_set;
    
    return result;
}