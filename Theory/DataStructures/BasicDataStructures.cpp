/*
Contains the following Data Structures:
- Stack
- Queue
- Priority Queue
- Hash
- Tree
- Graph
- Linked List
- Deque
- Map
- Set
*/

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

////////// Graph Start //////////
// Reference 코드에서의 Graph는 다수의 Vertex를 가지고 있고
// 각 Vertex는 단순 연결 리스트 구조의 GraphAdjList를 가지고 있어서
// 해당 List에 Node가 쭉쭉 들어가게 된다
// 여기에서의 head는 이중 연결 리스트에 구현된 구조와는 다르게
// head가 비어있지 않고 실제 첫번째 노드인 구조이다

// Graph의 Vertex에 연결된 각 Node를 정의하는 Struct
typedef struct GraphAdjlistNode
{
    int vertex;
    GraphAdjlistNode *next;
} graphAdjListNode;

// Graph의 Vertex에 연결된 각 Node를 담는 단방향 LinkedList
typedef struct GraphAdjList
{
    int num_members;
    GraphAdjlistNode *head;
    GraphAdjlistNode *tail;
} graphAdjList;

// Graph의 Struct 그 자체
typedef struct Graph
{
    int num_vertices;
    GraphAdjList *graphAdjListArr;
} graph;

// 새로운 Node를 생성하는 함수
graphAdjListNode *createGraphNode(int v)
{
    graphAdjListNode *newNode = (graphAdjListNode *)malloc(sizeof(graphAdjListNode));

    newNode->vertex = v;
    newNode->next = NULL;

    return newNode;
}

Graph *createGraph(int n)
{
    Graph *graph = (Graph *)malloc(sizeof(Graph));

    graph->num_vertices = n;
    graph->graphAdjListArr = (graphAdjList *)malloc(n * sizeof(graphAdjList));

    // 각각의 Vertex는 별도의 graphAdjList를 가지고 있기 때문에 각각의 헤드와 테일에 대해 초기화
    for (int i = 0; i < n; ++i)
    {
        graph->graphAdjListArr[i].head = graph->graphAdjListArr[i].tail = NULL;
        graph->graphAdjListArr[i].num_members = 0;
    }
    
    return graph;
}

void destroyGraph(Graph *graph)
{
    if (graph != NULL)
    {
        if (graph->graphAdjListArr)
        {
            for (int v = 0; v < graph->num_vertices; v++)
            {
                GraphAdjlistNode *graphAdjListPtr = graph->graphAdjListArr[v].head;
                while (graphAdjListPtr != NULL)
                {
                    graphAdjListNode *tmp = graphAdjListPtr;
                    graphAdjListPtr = graphAdjListPtr->next;
                    free(tmp);
                }
            }

            free(graph->graphAdjListArr);
        }
        free(graph);
    }
}

// 여기서 src는 잇고자 하는 시작점의 Vertex이고
// dest는 목적지 Vertex이다
void addGraphEdge(Graph *graph, int src, int dest)
{
    // 우선 목적지 Vertex Node를 생성한다
    graphAdjListNode *newNode = createGraphNode(dest);

    // 비어있지 않은 graphAdjLustArr에 Node를 넣게되면
    // 
    if (graph->graphAdjListArr[src].tail != NULL)
    {
        // 여기서 왜 tail->next까지 newNode로 지정하는 건지는 잘 모르곘다....
        graph->graphAdjListArr[src].tail->next = newNode;
        graph->graphAdjListArr[src].tail = newNode;
    }
    // 비어있는 graphAdjListArr에 Node를 넣게되면
    // Head와 Tail이 둘 다 해당 새로운 Node를 가르키게끔 한다
    else
    {
        graph->graphAdjListArr[src].head = graph->graphAdjListArr[src].tail = newNode;
    }
    graph->graphAdjListArr[src].num_members++;

    newNode = createGraphNode(src);
    if (graph->graphAdjListArr[dest].tail != NULL)
    {
        graph->graphAdjListArr[dest].tail->next = newNode;
        graph->graphAdjListArr[dest].tail = newNode;
    }
    else
    {
        graph->graphAdjListArr[dest].head = graph->graphAdjListArr[dest].tail = NULL;
    }
    graph->graphAdjListArr[dest].num_members++;
}

void displayGraph(Graph *graph, int i)
{
    graphAdjListNode *adjListPtr = graph->graphAdjListArr[i].head;

    while (adjListPtr != NULL)
    {
        printf("%d ", adjListPtr->vertex);
        adjListPtr = adjListPtr->next;
    }

    printf("\n");
}

////////// Graph End //////////

////////// Linked List Start //////////
// 여기서는 이중 Linked List로 구현함
// Linked List는 Head가 있고 그 중간에 Node를 추가할 수도 삭제할 수도 있는 구조임
// Head는 비어있는 Node이고 첫번째의 실제 Node를 가르킴
// 새로 들어가는 Node는 Head가 가르키는 자리로 바로 들어감
// (인터넷 보면 구현 방식은 조금씩 다른 것 같음)

typedef struct ListNode
{
    int data;
    ListNode *prev;
    ListNode *next;
};

ListNode *createList (int data)
{
    ListNode *node = (ListNode *)malloc(sizeof(ListNode));

    node->prev = NULL;
    node->next = NULL;
    node->data = data;

    return node;
}

// Reference에 구현된 코드는 new_node를 사전에 지정한 head 바로 다음에 넣어준다
ListNode *insertList(ListNode *head, ListNode *new_node)
{
    // head와 head의 기존 next_node 사이에 new_node를 넣는 것
    // 그렇게 하기 위해서는 ListNode *next에 우선 head->next를 저장해놓고
    // head의 next를 new_node로 한 후에 new_node에 prev & next를 묶어주면 됨
    ListNode *next  = head->next;
    head->next = new_node;
    new_node->next = next;
    new_node->prev = head;

    // next의 prev도 new_node로 새로 설정해줘야 함
    if (next != NULL)
    {
        next->prev = new_node;
    }

    return new_node;
}

int eraseList (ListNode *head, int data)
{
    // head는 비어있는 Node이기 때문에 it을 그 바로 다음 Node로 설정한다
    ListNode *it = head->next;
    int ret = 0;

    while (it != NULL)
    {
        // it-data가 우리가 찾는 것이라면
        // it의 저장해놓은 후
        // it을 it->next로 바꿔치기 한 다음에
        if (it->data == data)
        {
            ListNode* prev = it->prev;
            ListNode* next = it->next;
            ListNode* tmp = it;
            it = it->next;
 
            // prev->next를 next로 지정
            prev->next = next;
            // next가 NULL이 아니라면 next->prev는 prev로
            if (next != NULL)
            {
                next->prev = prev;
            }
             
            // tmp의 메모리는 놓아주기
            free(tmp);

            // Reference에는 삭제한 Node의 개수만큼 출력하게 되어있다
            // 연결리스트는 중복 무관
            ret++;
        }
        // it->data가 우리가 찾는 게 아니라면 다음 Node로 넘어가기
        else
        {
            it = it->next;
        }
    }

    return ret;
}

////////// Linked List End //////////

////////// Deque Start //////////

#define DEQUE_MAX 100
int deque_arr[DEQUE_MAX];
int deque_front;
int deque_rear;
int deque_size;

void dequeInit(int n)
{
    deque_front = -1;
    deque_rear = 0;
    deque_size = n;
}

bool isDequeFull()
{
    // Rotary 느낌으로 쭉쭉 도는 것이기 때문에 deque_front == 0 && deque_rear == deque_size - 1일 때 뿐만 아니라
    // deque_front == deque_rear + 1일 때도 Full인 것
    return ((deque_front == 0 && deque_rear == deque_size - 1) || deque_front == deque_rear + 1);
}

bool isDequeEmpty()
{
    return (deque_front == -1);
}

// 무한루프를 도는 것처럼 돌면서 채워짐 (%를 쓰는 대신 if문으로 구현한 것)
void insertDequeFront(int value)
{
    if (isDequeFull())
    {
        printf("Deque Overflow!\n");
    }

    if (deque_front == -1)
    {
        deque_front = deque_rear = 0;
    }
    else if (deque_front == 0)
    {
        deque_front = deque_size - 1;
    }
    else
    {
        deque_front = deque_front - 1;
    }

    deque_arr[deque_front] = value;
}

// 무한루프를 도는 것처럼 돌면서 채워짐 (%를 쓰는 대신 if문으로 구현한 것)
void insertDequeRear(int value)
{
    if (isDequeFull())
    {
        printf("Deque Overflow!\n");
    }

    if (deque_front == -1)
    {
        deque_front = deque_rear = 0;
    }
    else if (deque_rear == deque_size - 1)
    {
        deque_rear = 0;
    }
    else
    {
        deque_rear = deque_rear + 1;
    }

    deque_arr[deque_rear] = value;
}

int getDequeFront()
{
    if (isDequeEmpty())
    {
        printf("Deque Overflow\n");
        return -1;
    }

    return deque_arr[deque_front];
}

int getDequeRear()
{
    if (isDequeEmpty() || deque_rear < 0)
    {
        printf("Deque Underflow\n");
        return -1;
    }

    return deque_arr[deque_rear];
}

// 무한루프를 도는 것처럼 돌면서 채워짐 (%를 쓰는 대신 if문으로 구현한 것)
void deleteDequeFront()
{
    if (isDequeEmpty())
    {
        printf("Deque Underflow!\n");
        return;
    }

    if (deque_front == deque_rear)
    {
        deque_front = -1;
        deque_rear = -1;
    }
    else if (deque_front == deque_size - 1)
    {
        deque_front = 0;
    }
    else
    {
        deque_front = deque_front + 1;
    }
}

// 무한루프를 도는 것처럼 돌면서 채워짐 (%를 쓰는 대신 if문으로 구현한 것)
void deleteDequeRear()
{
    if (isDequeEmpty())
    {
        printf("Deque Underflow!\n");
        return;
    }

    if (deque_front == deque_rear)
    {
        deque_front = -1;
        deque_rear = -1;
    }
    else if (deque_rear == 0)
    {
        deque_rear = deque_size - 1;
    }
    else
    {
        deque_rear = deque_rear - 1;
    }
}

////////// Deque End //////////

////////// Map Start //////////
// 키를 값에 매핑할 수 있는 자료구조이고 중복을 허용하지 않는다

typedef struct MapNode
{
    int key;
    int value;
    MapNode *left, *right;
};

// 새로운 Memory를 할당해서 전달받은 Key값과 Value를 저장하고
// Left, Right는 NULL로 지정
MapNode *newMapNode(int k, int v)
{
    MapNode *temp = (MapNode *)malloc(sizeof(MapNode));
    temp->key = k;
    temp->value = v;
    temp->left = temp->right = NULL;
    return temp;
}

MapNode *currentMapNode;

// Node가 NULL이면 Node를 새로 생성
// Node가 비어있지 않으면 Key값을 비교해서 왼쪽 또는 오른쪽의 비어있는 Node를
// 찾아서 거기에 새로 생성
// 이렇게 하면 오름차순으로 자동 정렬된 상태로 Map을 만들게 된다
// 이미 Key가 존재하는 경우 (NULL이 아니면서) 새로운 Value값을 넣어준다
MapNode *putMapRec(MapNode *node, int key, int value)
{
    if (node == NULL)
    {
        return newMapNode(key, value);
    }

    if (key < node->key)
    {
        node->left = putMapRec(node->left, key, value);
    }
    else if (key > node->key)
    {
        node->right = putMapRec(node->right, key, value);
    }
    else
    {
        node->value = value;
    }

    return node;
}

void putMap(int key, int value)
{
    currentMapNode  = putMapRec(currentMapNode, key, value);
}

// DFS방식으로 왼쪽으로 쭉쭉 찾아보고
// 오른쪽으로 쭉쭉 찾아본다
// 반환 값은 찾은 Key를 가지고 있는 Node의 Value값
// 만약 찾지 못하면 -1를 반환한다
int findMapRec(MapNode *node, int key)
{
    if (node != NULL)
    {
        if (key == node->key)
        {
            return node->value;
        }

        int ret = -1;
        ret = findMapRec(node->left, key);
        if (ret != -1)
        {
            return ret;
        }

        ret = findMapRec(node->right, key);
        if (ret != -1)
        {
            return ret;
        }
    }

    return -1;
}

bool containsMap(int key)
{
    int ret = findMapRec(currentMapNode, key);
    if (ret != -1)
    {
        return true;
    }

    return false;
}

int get (int key)
{
    return findMapRec(currentMapNode, key);
}

MapNode *minValueNodeOfMap (MapNode *node)
{
    MapNode *current = node;
    
    while (current->left != NULL)
    {
        current = current->left;
    }

    return current;
}

MapNode *removeMapRec(MapNode *node, int key)
{
    if (node == NULL)
    {
        return node;
    }

    if (key < node->key)
    {
        node->left = removeMapRec(node->left, key);
    }
    else if (key > node->key)
    {
        node->right = removeMapRec(node->right, key);
    }
    else
    {
        // node->left가 NULL이라는 건 remove하고자 하는 Node가
        // currentSet기준으로 오른쪽에 있는 Node라는 것
        // 그렇다면 해당 Node의 ->right를 currentMapNode로 지정하고
        // 해당 Node는 Memory를 Release해주면 된다
        // Vice Versa
        if (node->left == NULL)
        {
            MapNode *temp = node->right;
            free(node);
            return temp;
        }
        else if (node->right == NULL)
        {
            MapNode *temp = node->left;
            free(node);
            return temp;
        }

        // Node의 ->Left와 ->Right가 둘다 NULL이 아닌 경우는
        // 맨 처음에 Node로 설정한 Node인 경우
        // 이때 findMinValueNodeOfNode(node->right)를
        // 하면 바로 오른쪽에 있는 Node가 반환될 것
        MapNode *temp = minValueNodeOfMap(node->right);

        // 지우고자 하는 Node의 Key와 Value를 바로 오른쪽 Node의 Key와 Value로 설정을 하고
        node->key = temp->key;
        node->value = temp->value;

        // 오른쪽 Node를 지우면서 반환되는 Right Node를 현재의
        // Node의 오른쪽 Node로 설정해주면 다 해결된다
        node->right = removeMapRec(node->right, temp->key);
    }

    return node;
}

void removeMap(int key)
{
    currentMapNode = removeMapRec(currentMapNode, key);
}

////////// Map End //////////

////////// Set Start //////////
// 중복을 허용하지 않는 자료구조이며 삽입과 동시에 정렬이 된다

typedef struct SetNode
{
    int key;
    SetNode *left, *right;
};

// 새로운 Memory를 할당해서 전달받은 Key값을 저장하고
// Left, Right는 NULL로 지정
SetNode *newSetNode(int item)
{
    SetNode *temp = (SetNode *)malloc(sizeof(SetNode));
    temp->key = item;
    temp->left = temp->right = NULL;
    return temp;
}

SetNode *currentSetNode;

// Node가 NULL이면 Node를 새로 생성
// Node가 비어있지 않으면 Key값을 비교해서 왼쪽 또는 오른쪽의 비어있는 Node를
// 찾아서 거기에 새로 생성
// 이렇게 하면 오름차순으로 자동 정렬된 상태로 Set를 만들게 된다
SetNode *addSetRec(SetNode *node, int key)
{
    if (node == NULL)
    {
        return newSetNode(key);
    }

    if (key < node-> key)
    {
        node->left = addSetRec(node->left, key);
    }
    else if (key > node->key)
    {
        node->right = addSetRec(node->right, key);
    }
    
    return node;
}

void addSet (int key)
{
    currentSetNode = addSetRec(currentSetNode, key);
}

// DFS로 Key가 동일한 Node찾기
bool findSetRec(SetNode *node, int key)
{
    if (node != NULL)
    {
        if (key == node->key)
        {
            return true;
        }
        if (findSetRec(node->left, key))
        {
            return true;
        }
        if (findSetRec(node->right, key))
        {
            return true;
        }
    }

    return false;
}

bool setContains(int key)
{
    return findSetRec(currentSetNode, key);
}

// Left부터 쭉쭉 들어가면서 하나씩 출력해서 나온다
// Current 기준으로 왼쪽에 있는 Node들은 Left 밖에 없고
// 오른쪽에 있는 Node들은 Right 밖에 없기 때문에 한번씩만 출력을 하게 된다
void printAllSet(SetNode *node)
{
    if (node != NULL)
    {
        printAllSet(node->left);
        printf("%d", node->key);
        printAllSet(node->right);
    }
}

void printAllSet()
{
    printAllSet(currentSetNode);
}

SetNode *findMinValueNodeOfSet(SetNode *node)
{
    SetNode *current = node;
    
    while (current->left != NULL)
    {
        current = current->left;
    }

    return current;
}

SetNode *removeSetRec(SetNode *node, int key)
{
    if (node == NULL)
    {
        return node;
    }

    if (key < node->key)
    {
        node->left = removeSetRec(node->left, key);
    }
    else if (key > node->key)
    {
        node->right = removeSetRec(node->right, key);
    }
    else
    {
        // node->left가 NULL이라는 건 remove하고자 하는 Node가
        // currentSet기준으로 오른쪽에 있는 Node라는 것
        // 그렇다면 해당 Node의 ->right를 currentSet로 지정하고
        // 해당 Node는 Memory를 Release해주면 된다
        // Vice Versa
        if (node->left == NULL)
        {
            SetNode *temp = node->right;
            free(node);
            return temp;
        }
        else if (node->right == NULL)
        {
            SetNode *temp = node->left;
            free(node);
            return temp;
        }

        // Node의 ->Left와 ->Right가 둘다 NULL이 아닌 경우는
        // 맨 처음에 Node로 설정한 Node인 경우
        // 이때 findMinValueNodeOfSet(node->right)를
        // 하면 바로 오른쪽에 있는 Node가 반환될 것
        SetNode *temp = findMinValueNodeOfSet(node->right);

        // 지우고자 하는 Node의 Key를 바로 오른쪽 Node의 Key로 설정을 하고
        node->key = temp->key;

        // 오른쪽 Node를 지우면서 반환되는 Right Node를 현재의
        // Node의 오른쪽 Node로 설정해주면 다 해결된다
        node->right = removeSetRec(node->right, temp->key);

        // ... 근데 이렇게까지 복잡하게 해야되나...?
        // 이해하기 참 힘든 코드임
    }

    return node;
}

void removeSet (int key)
{
    currentSetNode = removeSetRec(currentSetNode, key);
}

////////// Set End //////////

int main()
{
    ////////// Stack Start //////////

    printf("===========Stack===========\n");
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

    printf("===========Queue===========\n");
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

    printf("===========PriorityQueue===========\n");
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

    printf("===========Hash===========\n");
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

    printf("===========Tree===========\n");
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
    printf("\n");
    
    ////////// Tree End //////////

    ////////// Graph Start //////////

    printf("===========Graph===========\n");
    Graph *graph = createGraph(4);
    addGraphEdge(graph, 0, 1);
    addGraphEdge(graph, 0, 2);
    addGraphEdge(graph, 0, 3);
    addGraphEdge(graph, 1, 2);
    addGraphEdge(graph, 2, 1);
    addGraphEdge(graph, 3, 1);

    displayGraph(graph, 0);
    displayGraph(graph, 1);
    displayGraph(graph, 2);
    displayGraph(graph, 3);

    ////////// Graph End //////////

    ////////// Linked List Start //////////

    printf("===========LinkedList===========\n");
    // head에는 data를 NULL로
    ListNode *head = createList(NULL);
    ListNode *node = createList(1);
    insertList(head, node);
    node = createList(2);
    insertList(head, node);
    node = createList(5);
    insertList(head, node);
    node = createList(8);
    insertList(head, node);

    ListNode *it = head->next;
    while (it != NULL)
    {
        printf("%d //", it->data);
        it = it->next;
    }
    printf("\n");

    eraseList(head, 5);

    it = head->next;
    while (it != NULL)
    {
        printf("%d //", it->data);
        it = it->next;
    }
    printf("\n");

    ////////// Linked List End //////////

    ////////// Deque End //////////

    printf("===========Deque===========\n");
    dequeInit(5);
    insertDequeFront(1);
    insertDequeFront(2);
    insertDequeRear(3);
    insertDequeFront(4);
    printf("Deque Front: %d // Deque Rear: %d\n", getDequeFront(), getDequeRear());
    printf("Is Dequeu Full?: %d\n", isDequeFull());

    ////////// Deque End //////////

    ////////// Map Start //////////

    printf("===========Map===========\n");
    currentMapNode = NULL;
    putMap(2, 5);
    putMap(5, 10);
    putMap(20, -1);
    putMap(40, 6);
    printf("%d", get(2));
    printf("\n");
    removeMap(2);
    printf("%d", get(2));
    printf("\n");
    printf("%d", get(-1));
    printf("\n");

    ////////// Map End //////////

    ////////// Set Start //////////

    printf("===========Set===========\n");
    currentSetNode = NULL;
    addSet(5);
    addSet(4);
    addSet(8);
    addSet(10);
    addSet(2);
    removeSet(5);

    printAllSet();
    printf("\n");

    ////////// Set End //////////
}
