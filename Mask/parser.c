#include "parser.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>

int is_string_permissions(const char* input) {
    if (strlen(input) != 9) {
        return 0;
    }
    
    for (int i = 0; i < 9; i++) {
        if (i % 3 == 0) {
            if (input[i] != 'r' && input[i] != '-') return 0;
        } else if (i % 3 == 1) {
            if (input[i] != 'w' && input[i] != '-') return 0;
        } else {
            if (input[i] != 'x' && input[i] != '-') return 0;
        }
    }
    
    return 1;
}

int is_octal_permissions(const char* input) {
    if (strlen(input) != 3) {
        return 0;
    }
    for (int i = 0; i < 3; i++) {
        if ('0' > input[i] || '7' < input[i]) {
            return 0;
        }
    }
    
    return 1;
}

int is_bit_permissions(const char* input) {
    if (strlen(input) != 9) {
        return 0;
    }
    for (int i = 0; i < 9; i++) {
        if (input[i] != '1' && input[i] != '0') {
            return 0;
        }
    }
    
    return 1;
}

mode_t parse_permissions(const char* input) {
    if (!input) return 0;
    mode_t mode = 0;

    if (is_octal_permissions(input)) {
        int octal = strtol(input, NULL, 8);
        mode = (mode_t)octal;
        return mode;
    }
    
    if (is_string_permissions(input)) {
        if (input[0] == 'r') mode |= S_IRUSR;
        if (input[1] == 'w') mode |= S_IWUSR;
        if (input[2] == 'x') mode |= S_IXUSR;
        
        if (input[3] == 'r') mode |= S_IRGRP;
        if (input[4] == 'w') mode |= S_IWGRP;
        if (input[5] == 'x') mode |= S_IXGRP;
        
        if (input[6] == 'r') mode |= S_IROTH;
        if (input[7] == 'w') mode |= S_IWOTH;
        if (input[8] == 'x') mode |= S_IXOTH;
        
        return mode;
    }

    if (is_bit_permissions(input)) {
        // if (input[0] == '1') mode |= S_IRUSR;
        // if (input[1] == '1') mode |= S_IWUSR;
        // if (input[2] == '1') mode |= S_IXUSR;
        
        // if (input[3] == '1') mode |= S_IRGRP;
        // if (input[4] == '1') mode |= S_IWGRP;
        // if (input[5] == '1') mode |= S_IXGRP;
        
        // if (input[6] == '1') mode |= S_IROTH;
        // if (input[7] == '1') mode |= S_IWOTH;
        // if (input[8] == '1') mode |= S_IXOTH;
        

        int bit = strtol(input, NULL, 2);
        mode = (mode_t)bit;
        return mode;
    }
    
    return 0;
}