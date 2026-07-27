#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "queue.h"

void generate_message(int* data, int* priority) {
    *data = rand() % 1000;
    *priority = rand() % 10;
}

void demonstrateQueue() {
    Priority_Queue pq;
    initQueue(&pq);
    
    printf("Тестовая программа\n\n");
    
    printf("1. Добавление элементов:\n");
    srand(time(NULL));
    
    for (int i = 0; i < 10; i++) {
        int data, priority;
        generate_message(&data, &priority);
        data = i;
        printf("Добавление: данные=%d, приоритет=%d\n", data, priority);
        paste_to_queue(&pq, data, priority);
        print_queue(&pq);
    }
    
    printf("\nРазмер очереди: %d\n\n", pq.size);
    
    printf("2. Извлечение первого элемента:\n");
    int first = get_first_queue(&pq);
    if (first != -1) {
        printf("Извлечен первый элемент: %d\n", first);
        print_queue(&pq);
    }
    printf("Размер очереди: %d\n\n", pq.size);
    
    printf("3. Доп добавление элементов:\n");
    for (int i = 0; i < 5; i++) {
        int data, priority;
        generate_message(&data, &priority);
        printf("Добавление: данные=%d, приоритет=%d\n", data, priority);
        paste_to_queue(&pq, data, priority);
    }
    print_queue(&pq);
    printf("Размер очереди: %d\n\n", pq.size);
    
    int old_size = pq.size;
    while (pq.size == old_size) {
    printf("4. Извлечение с указанным приоритетом:\n");
    int targetPriority = rand() % 50;
    printf("Ищем элемент с приоритетом: %d\n", targetPriority);
    int found = get_first_of_priority_of_queue(&pq, targetPriority);
    if (found != -1) {
        printf("Извлечен элемент: %d (приоритет %d)\n", found, targetPriority);
        print_queue(&pq);
    }
    printf("Размер очереди: %d\n\n", pq.size);
    }
    

    printf("5. Извлечение с приоритетом не выше заданного:\n");
    int minPriority = rand() % 10;
    printf("Минимальный приоритет: %d (ищем priority >= %d)\n", minPriority, minPriority);
    int minFound = get_min_priority_of_queue(&pq, minPriority);
    if (minFound != -1) {
        printf("Извлечен элемент: %d\n", minFound);
        print_queue(&pq);
    }
    printf("Размер очереди: %d\n\n", pq.size);
    
    printf("6. Извлечение всех оставшихся элементов:\n");
    while (!is_empty(&pq)) {
        int data = get_first_queue(&pq);
        printf("Извлечен: %d\n", data);
    }
    printf("Размер очереди: %d\n\n", pq.size);
    
    printf("7. Очистка очереди:\n");
    clear_queue(&pq);
    printf("Очередь очищена. Размер: %d\n", pq.size);
}


int main() {
    demonstrateQueue();
    
    printf("\nКонец\n");
    return 0;
}