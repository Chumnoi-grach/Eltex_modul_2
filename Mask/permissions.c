#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include "permissions.h"
#include <sys/types.h>

void print_permissions(mode_t mode) {
    printf("Буквенный формат: %s\n", mode_to_string(mode));
    printf("Цифровой формат: %d\n", mode_to_octal(mode));
    printf("Двоичный формат: %s\n", mode_to_binary(mode));
}

char* mode_to_string(mode_t mode) {
    static char str[10];
    char* p = str;
    
    //Хозяин
    *p++ = (mode & S_IRUSR) ? 'r' : '-'; //100 000 000
    *p++ = (mode & S_IWUSR) ? 'w' : '-'; //010 000 000
    *p++ = (mode & S_IXUSR) ? 'x' : '-'; //001 000 000
    //Братаны
    *p++ = (mode & S_IRGRP) ? 'r' : '-'; //000 100 000
    *p++ = (mode & S_IWGRP) ? 'w' : '-'; //000 010 000
    *p++ = (mode & S_IXGRP) ? 'x' : '-'; //000 001 000
    //Те
    *p++ = (mode & S_IROTH) ? 'r' : '-'; //000 000 100
    *p++ = (mode & S_IWOTH) ? 'w' : '-'; //000 000 010
    *p++ = (mode & S_IXOTH) ? 'x' : '-'; //000 000 001
    
    *p = '\0';
    return str;
}

int mode_to_octal(mode_t mode) {
    mode_t perms = mode & 0777;
    int octal = 0;
    
    octal += ((perms & 0700) >> 6) * 100;
    octal += ((perms & 0070) >> 3) * 10;
    octal += (perms & 0007);
    
    return octal;
}

char* mode_to_binary(mode_t mode) {
    static char bits[10];
    mode_t perms = mode & 0777;
    
    for (int i = 8; i >= 0; i--) {
        bits[8 - i] = (perms & (1 << i)) ? '1' : '0';
    }
    bits[9] = '\0';
    
    return bits;
}

int get_file_permissions(const char* filename, mode_t* mode) {
    struct stat st;
    
    if (stat(filename, &st) != 0) {
        return -1;
    }
    
    *mode = st.st_mode & 0777;
    return 0;
}
