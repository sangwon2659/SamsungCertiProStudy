#include <iostream>
using namespace std;

#define MAX_N 100

////////// Stack Start //////////
int stack_top;
int stack[MAX_N];

void stackInit(void)
{
    stack_top = 0;
}

bool stackIsEmpty()
{
    return (stack_top == 0);
}

bool stackIsFull()
{
    return (stack_top == MAX_N);
}

bool stackPush(int value)
{
    if (stackIsFull())
    {
        printf("Stack Overflow!");
        return false;
    }

    stack[stack_top] = value;
    stack_top++;

    return true;
}

bool stackPop(int *value)
{
    if (stackIsEmpty())
    {
        printf("Stack is Empty!");
        return false;
    }

    stack_top--;
    // Pop하기 전의 Value를 전달인자로 받은 주소에 값으로 넣어주기
    *value = stack[stack_top];

    return true;
}

////////// Stack End //////////

////////// Queue Start //////////

int queue_front;
int queue_rear;
int queue[MAX_N];

void queueInit()
{
    queue_front = 0;
    queue_rear = 0;
}

bool queueIsEmpty()
{
    return (queue_front == queue_rear);
}

bool queueIsFull()
{
    if ((queue_rear + 1) % MAX_N == queue_front)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool queueEnqueue(int value)
{
    if (queueIsFull())
    {
        printf("Queue is Full!");
        return false;
    }

    queue[queue_rear] = value;
    queue_rear++;

    if (queue_rear == MAX_N)
    {
        queue_rear = 0;
    }

    return true;
}

int queueDequeue(int *value)
{
    if (queueIsEmpty())
    {
        printf("Queue is Empty");
        return false;
    }

    *value = queue[queue_front];
    queue_front++;

    if (queue_front == MAX_N)
    {
        queue_front = 0;
    }

    return true;
}

////////// Queue End //////////

////////// Priority Queue Start //////////

// 우선순위 큐는 보통 힙으로 구현
// 힙은 Complete Binary Tree의 일종으로 우선순위 큐를 위해 만들어진 구조
// 힙을 저장하는 표준적인 구조는 배열
// 힙이 0부터 시작하면 부모와 자식 간의 관계는 Left: 부모인덱스 * 2 + 1 & Right: 부모인덱스 * 2 + 2
// (부모를 찾을 땐 자식인덱스 - 1 / 2)
// *힙은 잔정렬 상태 (느슨한 정렬 상태)
// 부모 노드의 키 값이 자식 노드의 키 값보다 >/<인 것일 뿐
// 해당 Sample Code는 값이 작은 경우가 우선순위가 되는 Priority Queue

int heap_size;
int heap[MAX_N];

void priorityQueueInit()
{
    heap_size = 0;
}

bool priorityQueueIsEmpty()
{
    return (heap_size == 0);
}

bool priorityQueuePush(int value)
{
    if (heap_size + 1 > MAX_N)
    {
        printf("Priority Queue is Full!");
        return false;
    }

    heap[heap_size] = value;
    int current = heap_size;

    // 작은 숫자가 높은 우선순위를 갖도록 구현한다면
    while (current > 0 && heap[current] < heap[(current - 1) / 2])
    {
        int temp = heap[(current - 1) / 2];
        heap[(current - 1) / 2] = heap[current];
        heap[current] = temp;
        current = (current - 1) / 2;
    }

    heap_size++;
    return true;
}

bool priorityQueuePop(int *value)
{
    if (priorityQueueIsEmpty())
    {
        printf("Priority Queue is Empty!");
        return false;
    }

    *value = heap[0];
    heap_size--;

    heap[0] = heap[heap_size];
    int current = 0;

    // Current의 다음 레벨의 Left Child가 heap_size보다 작으면 진입
    while (current * 2 + 1 < heap_size)
    {
        int child;
        // Current의 다음 레벨의 Left Child가 마지막이라면
        // Child는 Left로 결정
        // (heap_size는 마지막 node의 인덱스에 +1이기 때문)
        if (current * 2 + 2 == heap_size)
        {
            child = current * 2 + 1;
        }
        // 만약 위의 경우가 아니라면 child 중에서 더 값이 큰 Node가 Child Node
        else
        {
            child = heap[current * 2 + 1] < heap[current * 2 + 2] ? current * 2 + 1 : current * 2 + 2;
        }

        // Child Node 중에서 더 큰 Node보다도 현재 값이 더 작으면 이미 정렬된 상태이기에 Break
        if (heap[current] < heap[child])
        {
            break;
        }

        // 바꿔치기
        int temp = heap[current];
        heap[current] = heap[child];
        heap[child] = temp;

        current = child;
    }

    return true;
}

////////// Priority Queue End //////////

int main()
{
    ////////// Stack Start //////////

    stackInit();
    for (int i = 0; i < 10; ++i)
    {
        stackPush(i);
    }
    while (!stackIsEmpty())
    {
        int pop_value;
        stackPop(&pop_value);
        printf("%d", pop_value);
    }
    printf("\n");

    ////////// Stack End //////////

    ////////// Queue Start //////////

    queueInit();
    for (int i = 0; i < 10; ++i)
    {
        queueEnqueue(i);
    }
    while (!queueIsEmpty())
    {
        int dequeue_value;
        queueDequeue(&dequeue_value);
        printf("%d", dequeue_value);
    }
    printf("\n");

    ////////// Queue End //////////

    ////////// Priority Queue Start //////////

    priorityQueueInit();
    priorityQueuePush(5);
    priorityQueuePush(8);
    priorityQueuePush(20);
    priorityQueuePush(2);
    priorityQueuePush(10);

    while (!priorityQueueIsEmpty())
    {
        int priorityqueue_value;
        priorityQueuePop(&priorityqueue_value);
        printf("%d", priorityqueue_value);
    }
    printf("\n");

    ////////// Priority Queue End //////////
}
