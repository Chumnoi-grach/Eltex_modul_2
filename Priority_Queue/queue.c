#include "queue.h"
#include <stdlib.h>
#include <stdio.h>

void initQueue(Priority_Queue* pq) {
    pq->head = NULL;
    pq->size = 0;
}

int paste_to_queue(Priority_Queue *pq, int data, int priority) {
    if (!pq || priority < MIN_PRIORITY || priority > MAX_PRIORITY) return -1;
    
    Node* pasted = malloc(sizeof(Node));
    if (!pasted) {
        return -2;
    }
    pasted->data = data;
    pasted->priority = priority;
    pasted->next = NULL;
    
    // Если очередь пуста или новый элемент имеет более высокий приоритет (меньшее число)
    if (pq->head == NULL || pq->head->priority > priority) {
        pasted->next = pq->head;
        pq->head = pasted;
        pq->size++;
        return 0;
    }
    
    // Поиск места для вставки (сортировка по возрастанию приоритета)
    Node *current = pq->head;
    // Идем пока есть следующий элемент и его приоритет <= нового
    while (current->next != NULL && current->next->priority <= priority) {
        current = current->next;
    }
    
    // Вставляем после current
    pasted->next = current->next;
    current->next = pasted;
    pq->size++;
    return 0;
}

int get_first_queue(Priority_Queue *pq) {
    if (pq == NULL || pq->head == NULL) {
        return -1;
    }
    
    Node* temp = pq->head;
    int data = temp->data;
    pq->head = pq->head->next;
    free(temp);
    pq->size--;
    return data;
}
int get_first_of_priority_of_queue(Priority_Queue *pq, int priority) {
    if (pq == NULL || pq->head == NULL) {
        return -1;
    }
    
    if (priority < MIN_PRIORITY || priority > MAX_PRIORITY) {
        return -2;
    }
    
    if (pq->head->priority == priority) {
        return get_first_queue(pq);
    }
    
    Node* current = pq->head;
    while (current->next != NULL && current->next->priority != priority) {
        current = current->next;
    }
    
    if (current->next == NULL) {
        return -1;
    }
    
    Node* temp = current->next;
    int data = temp->data;
    current->next = temp->next;
    free(temp);
    pq->size--;
    return data;
}
int get_min_priority_of_queue(Priority_Queue *pq, int priority) {
     if (pq == NULL || pq->head == NULL) {
        return -1;
    }
    if (priority < MIN_PRIORITY || priority > MAX_PRIORITY) {
        return -2;
    }
    
    if (pq->head->priority >= priority) {
        return get_first_queue(pq);
    }
    
    Node* current = pq->head;
    while (current->next != NULL && current->next->priority < priority) {
        current = current->next;
    }
    
    if (current->next == NULL) {
        return -1;
    }
    
    Node* temp = current->next;
    int data = temp->data;
    current->next = temp->next;
    free(temp);
    pq->size--;
    return data;
}
int is_empty(Priority_Queue *pq) {
    if (pq == NULL) return 1;
    return (pq->head == NULL);
}
void clear_queue(Priority_Queue* pq) {
    if (pq == NULL) return;
    
    while (pq->head != NULL) {
        Node* temp = pq->head;
        pq->head = pq->head->next;
        free(temp);
    }
    pq->size = 0;
}

void print_queue(Priority_Queue *pq) {
    if (pq == NULL || pq->head == NULL) {
        printf("Очередь пуста\n");
        return;
    }
    
    Node* current = pq->head;
    printf("Очередь (приоритет: данные): ");
    while (current != NULL) {
        printf("[%d:%d] ", current->priority, current->data);
        current = current->next;
    }
    printf("\n");
}