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

////////// Hash Start //////////

// 기본적인 절차
// 1. Hash값을 왼쪽으로 5번 비트연산 시킨다
// 2. 원본 Hash값을 더한다
// 3. 한 문자의 ASCII 값을 더한다
// 4. 위의 결과를 모든 문자에 대해 반복한다
// 5. 최종 값이 해시테이블의 범위를 벗어나면, 나머지 연산을 취해준다

#define MAX_KEY 64
#define MAX_DATA 128
#define MAX_TABLE 4096

typedef struct
{
    // char는 끝에 '\0'가 붙기 때문에 +1 해줘야 함
    char key[MAX_KEY + 1];
    char data[MAX_DATA + 1];
} Hash;
Hash tb[MAX_TABLE];

unsigned long hashKey(const char *str)
{
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
    {
        hash = ((hash << 5) + hash + c) % MAX_TABLE;
    }

    return hash;
}

bool find(const char *key, char *data)
{
    unsigned long h = hashKey(key);
    int cnt = MAX_TABLE;

    // tb[h].key[0] == 0면 아무것도 없다는 뜻
    // cnt를 줄여가면서 cnt = 0, 즉 MAX_TABLE만큼 돌리면서 찾아보겠다는 것
    while (tb[h].key[0] != 0 && cnt--)
    {
        // Hash한 Key와 Table의 Key가 맞다면 0이 출력되고 if문 진입
        if (strcmp(tb[h].key, key) == 0)
        {
            strcpy(data, tb[h].data);
            return true;
        }

        // h를 하나씩 올려가면서 탐색 (% MAX_TABLE을 함)
        h = (h + 1) % MAX_TABLE;
    }

    return false;
}

bool addData(const char *key, char *data)
{
    unsigned long h = hashKey(key);

    while (tb[h].key[0] != 0)
    {
        if (strcmp(tb[h].key, key) == 0)
        {
            return false;
        }

        h = (h + 1) % MAX_TABLE;
    }

    strcpy(tb[h].key, key);
    strcpy(tb[h].data, data);

    return true;
}

////////// Hash End //////////

////////// Tree Start //////////

// Preorder(전위)는 Root -> Left -> Right
// Inorder(중위)는 Left -> Root -> Right
// PostOrder(후위)는 Left -> Right -> Root

#define MAX_NODE_NUM 10000
#define MAX_CHILD_NUM 2

typedef struct
{
    int parent;
    int child[MAX_CHILD_NUM];
} TreeNode;
TreeNode tree[MAX_NODE_NUM];

int node_num;
int edge_num;
int root;

// node_num을 받아서 그 개수만큼 트리를 쭉 돌면서
// 각 노드의 부모와 자식을 -1로 정의
void initTree()
{
    for (int i = 0; i <= node_num; ++i)
    {
        tree[i].parent = -1;
        for (int j = 0; j < MAX_CHILD_NUM; ++j)
        {
            tree[i].child[j] = -1;
        }
    }
}

void addChild(int parent, int child)
{
    int i;
    // 비어있는 Child 노드 찾기
    for (i = 0; i < MAX_CHILD_NUM; ++i)
    {
        if (tree[parent].child[i] == -1)
        {
            break;
        }
    }

    tree[parent].child[i] = child;
    tree[child].parent = parent;
}

int getRoot()
{
    for (int i = 1; i <= node_num; ++i)
    {
        if (tree[i].parent == -1)
        {
            return i;
        }
    }

    return  -1;
}

void preOrder(int root)
{
    int i;
    int child;
    printf("%d", root);

    for (i = 0; i < MAX_CHILD_NUM; ++i)
    {
        child = tree[root].child[i];
        if (child != -1)
        {
            preOrder(child);
        }
    }
}

void inOrder(int root)
{
    if (root == -1) return;

    inOrder(tree[root].child[0]);
    printf("%d", root);
    inOrder(tree[root].child[1]);
}

void postOrder(int root)
{
    if (root == -1) return;

    postOrder(tree[root].child[0]);
    postOrder(tree[root].child[1]);
    printf("%d", root);
}

////////// Tree End //////////

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

    ////////// Hash End //////////

    addData("Samsung", "Electronics");
    addData("Hyundai", "Motors");
    addData("SK", "Innovation");

    char hash_return[MAX_DATA + 1];
    find("Samsung", hash_return);
    printf("%s\n", hash_return);
    find("Hyundai", hash_return);
    printf("%s\n", hash_return);
    find("SK", hash_return);
    printf("%s\n", hash_return);

    ////////// Hash End //////////

    ////////// Tree Start //////////

    node_num = 5;
    edge_num = 4;
    initTree();

    addChild(1, 2);
    addChild(1, 3);
    addChild(2, 4);
    addChild(3, 5);

    preOrder(getRoot());
    printf("\n");
    inOrder(getRoot());
    printf("\n");
    postOrder(getRoot());
    
    ////////// Tree End //////////
}
