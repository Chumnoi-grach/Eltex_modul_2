#include "input.h"
#include <stdio.h>
#include <string.h>

void clear_input(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

int get_int(const char *request) {
    int value;
    printf("%s", request);
    scanf("%d", &value);
    clear_input();
    return value;
}

void get_string(const char *request, char *buffer, int size) {
    printf("%s", request);
    if (fgets(buffer, size, stdin)) {
        buffer[strcspn(buffer, "\n")] = '\0';
    }
}

// bool get_string_optional(char *buffer, int size) {
//     if (fgets(buffer, size, stdin)) {
//         buffer[strcspn(buffer, "\n")] = '\0';
//         return strlen(buffer) > 0;
//     }
//     return false;
// }

void get_parameters(const char *request, char *buffer) {
    printf("%s", request);
    if (fgets(buffer, 9, stdin)) {
        buffer[strcspn(buffer, "\n")] = '\0';
    }
}

char get_char(void) {
    char c = getchar();
    clear_input();
    return c;
}