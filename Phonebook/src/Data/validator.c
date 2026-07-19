#include "validator.h"
#include <string.h>
#include <ctype.h>

int validate_name(const char *name) {
    if (!name || strlen(name) == 0) return 0;
    
    for (int i = 0; name[i]; i++) {
        unsigned char ch = (unsigned char)name[i];
        
        // Пропускаем UTF-8 многобайтовые последовательности
        if (ch >= 0x80) {
            // Проверяем корректность UTF-8
            if ((ch & 0xC0) == 0x80) continue; // continuation byte
            
            int bytes = 0;
            if ((ch & 0xE0) == 0xC0) bytes = 2;
            else if ((ch & 0xF0) == 0xE0) bytes = 3;
            else if ((ch & 0xF8) == 0xF0) bytes = 4;
            else return 0; // невалидный UTF-8
            
            if (i + bytes > (int)strlen(name)) return 0;
            
            for (int j = 1; j < bytes; j++) {
                if ((name[i + j] & 0xC0) != 0x80) return 0;
            }
            
            i += bytes - 1;
            continue;
        }
        
        // ASCII: буквы, пробел, дефис, подчеркивание
        if (!isalpha(ch) && ch != ' ' && ch != '-' && ch != '_') {
            return 0;
        }
    }
    
    return 1;
}

int validate_phone(const char *phone) {
    if (!phone || strlen(phone) == 0) return 1;
    
    for (int i = 0; phone[i]; i++) {
        if (!isdigit(phone[i]) && phone[i] != '+' && 
            phone[i] != '-' && phone[i] != ' ' && phone[i] != '(' && phone[i] != ')') {
            return 0;
        }
    }
    return 1;
}

int validate_email(const char *email) {
    if (!email || strlen(email) == 0) return 1;
    
    const char *at = strchr(email, '@');
    const char *dot = strrchr(email, '.');
    
    if (!at || !dot || at > dot) return 0;
    if (at == email || dot == email + strlen(email) - 1) return 0;
    

    if (dot + 1 == email + strlen(email)) return 0;
    
    if (dot - at <= 1) return 0;

    return 1;
}
// #include "validator.h"
// #include <locale.h>
// #include <string.h>
// #include <ctype.h>
// //#include <regex.h>

// int validate_name(const char *name) {
//     if (!name || strlen(name) == 0) return 0;
//     for (int i = 0; name[i]; i++) {
//         if (!isalpha(name[i]) && name[i] != ' ' && name[i] != '-' && name[i] != '_') {
//             return 0;
//         }
//     }
//     return 1;
// }

// int validate_phone(const char *phone) {
//     if (!phone || strlen(phone) == 0) return 1;
    
//     for (int i = 0; phone[i]; i++) {
//         if (!isdigit(phone[i]) && phone[i] != '+' && 
//             phone[i] != '-' && phone[i] != ' ') {
//             return 0;
//         }
//     }
//     return 1;
// }

// int validate_email(const char *email) {
//     if (!email || strlen(email) == 0) return 1;
    
//     const char *at = strchr(email, '@');
//     const char *dot = strrchr(email, '.');
    
//     if (!at || !dot || at > dot) return 0;
//     if (at == email || dot == email + strlen(email) - 1 || dot == email) return 0;
    
//     return 1;
// }


