#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>

uint32_t ip_to_uint32(const char *ip_str) {
    unsigned int a, b, c, d;
    sscanf(ip_str, "%u.%u.%u.%u", &a, &b, &c, &d);
    return (a << 24) | (b << 16) | (c << 8) | d;
}

void uint32_to_ip(uint32_t ip, char *buffer) {
    sprintf(buffer, "%u.%u.%u.%u", 
            (ip >> 24) & 0xFF,
            (ip >> 16) & 0xFF,
            (ip >> 8) & 0xFF,
            ip & 0xFF);
}

uint32_t generate_random_ip(void) {
    uint32_t ip = 0;
    ip |= ((uint32_t)(rand() % 256) << 24);
    ip |= ((uint32_t)(rand() % 256) << 16);
    ip |= ((uint32_t)(rand() % 256) << 8);
    ip |= (uint32_t)(rand() % 256);
    return ip;
}

int main() {

    char gateway_str[16];
    char mask_str[16];
    int N;

    printf("Введите IP_шлюза: ");
    fgets(gateway_str, 16, stdin);
    gateway_str[strcspn(gateway_str, "\n")] = '\0';
    getchar();
    printf("Введите Маску подсети: ");
    fgets(mask_str, 16, stdin);
    //getchar();
    mask_str[strcspn(mask_str, "\n")] = '\0';
    printf("Введите количество пакетов: ");
    scanf("%d", &N);

    if (N <= 0) {
        printf("Ошибка: количество пакетов должно быть положительным числом\n");
        return 1;
    }

    uint32_t gateway_ip = ip_to_uint32(gateway_str);
    uint32_t subnet_mask = ip_to_uint32(mask_str);

    uint32_t gateway_network = gateway_ip & subnet_mask;

    srand(time(NULL));
    int local_count = 0;
    int foreign_count = 0;

    uint32_t *random_ips = (uint32_t*)malloc(N * sizeof(uint32_t));
    if (random_ips == NULL) {
        printf("Ошибка выделения памяти\n");
        return 1;
    }

    printf("\nОбработка пакетов\n");
    printf("Шлюз: %s\n", gateway_str);
    printf("Маска: %s\n", mask_str);
    printf("Адрес сети шлюза: ");
    char network_str[16];


    
    uint32_to_ip(gateway_network, network_str);
    printf("%s\n", network_str);
    printf("Всего пакетов: %d\n\n", N);

    printf("Генерация и обработка пакетов:\n");
    printf("----------------------------------------\n");

    for (int i = 0; i < N; i++) {
        uint32_t dest_ip = generate_random_ip();
        random_ips[i] = dest_ip;
        char dest_ip_str[16];
        uint32_to_ip(dest_ip, dest_ip_str);

        uint32_t dest_network = dest_ip & subnet_mask;

        char *decision;
        if (dest_network == gateway_network) {
            decision = "СВОЯ подсеть";
            local_count++;
        } else {
            decision = "ДРУГАЯ сеть";
            foreign_count++;
        }

        char dest_network_str[16];
        uint32_to_ip(dest_network, dest_network_str);
        printf("Пакет %2d: %s -> сеть: %s [%s]\n", 
               i + 1, dest_ip_str, dest_network_str, decision);
    }

    printf("----------------------------------------\n\n");

    printf("\nСтатистика\n");
    printf("Всего обработано пакетов: %d\n", N);
    printf("\n");
    printf("Своя подсеть:  %4d пакетов (%5.2f%%)\n", 
           local_count, (float)local_count / N * 100);
    printf("Другие сети:   %4d пакетов (%5.2f%%)\n", 
           foreign_count, (float)foreign_count / N * 100);

    free(random_ips);

    return 0;
}