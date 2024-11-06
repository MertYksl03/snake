

struct Queue; // Forward declaration of Queue

void initialize_queue(Queue* q);
bool isEmpty(Queue* q);
bool isFull(Queue* q);
void enqueue(Queue* q, int value);
void dequeue(Queue* q);
int peek(Queue* q);

