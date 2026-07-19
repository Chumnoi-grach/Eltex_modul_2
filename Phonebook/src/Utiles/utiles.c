#include <string.h>
#include <stdlib.h>
#include "utiles.h"

int has_duplicates(const char *str) {
    if (!str) return 0;
    
    int res = 0;
    char *symv_str = (char *)calloc((strlen(str) + 1), sizeof(char));
    if (symv_str == NULL) return 0;

    for (int i = 0; str[i] != '\0'; i++) {
        if (strchr(symv_str, str[i]) != NULL) {
            res = 1;
            break;
        }
        symv_str[i] = str[i];
    }

    free(symv_str);
    return res;
}