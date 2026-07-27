#ifndef QUEUE
#define QUEUE
#define MAX_PRIORITY 255
#define MIN_PRIORITY 0

typedef struct Node Node;
typedef struct Priority_Queue Priority_Queue;

struct Node {
    int data;
    int priority;
    Node *next;
};

struct Priority_Queue {
    Node *head;
    int size; 
};

void initQueue(Priority_Queue* pq);

//Помнить про принцип fifo
int paste_to_queue(Priority_Queue *pq, int data, int priority);
int get_first_queue(Priority_Queue *pq);
int get_first_of_priority_of_queue(Priority_Queue *pq, int priority);
int get_min_priority_of_queue(Priority_Queue *pq, int priority);
int is_empty(Priority_Queue *pq);
void clear_queue(Priority_Queue* pq);
void print_queue(Priority_Queue *pq);


#endif