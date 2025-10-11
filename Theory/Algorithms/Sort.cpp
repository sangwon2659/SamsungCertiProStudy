/*
Contains the following Sorting Algorithms:
- Insertion Sort
- Quick Sort
- Counting Sort
- Bubble Sort
- Merge Sort
- Selection Sort
- Topological Sort
*/

#include <iostream>
using namespace std;

#define MAX_NUM 100

int input[MAX_NUM];
int input_size;

////////// InsertionSort Start //////////
// Insertion Sort는 모든 인덱스를 단순하게 쭉 돌면서
// 그보다 작은 인덱스의 모든 수와 비교를 하며 Swap을 하는 방식 정렬 알고리즘이다
// 시간복잡도 Best O(n) Worst O(n^2)

void insertionSort()
{
    int i, j;
    int temp;

    for (i = 1; i < input_size; i++)
    {
        temp = input[i];
        j = i - 1;

        while (j >= 0 && temp < input[j])
        {
            input[j + 1] = input[j];
            input[j] = temp;
            j--;
        }
    }
}

////////// InsertionSort End //////////

////////// QuickSort Start //////////
// Quick Sort는 Pivot을 정해서 우측 좌측 숫자들을 정리하고
// 다시 Partition으로 나눠서
// 새로운 Pivot으로 또 우측 좌측을 정리하면서
// 최종으로 정렬된 Array를 반환하는 알고리즘이다
// O(nlogn) 시간복잡도

void quickSort(int first, int last)
{
    int pivot;
    int i;
    int j;
    int temp;
    
    // 이건 사실상 당연한 조건
    if (first < last)
    {
        // Pivot은 가장 첫번째 인덱스로 시작
        pivot = first;
        i = first;
        j = last;
 
        while (i < j)
        {
            // input[pivot]보다 작으면서 인덱스를 벗어나지 않는 지점까지 i++;
            while (input[i] <= input[pivot] && i < last)
            {
                i++;
            }
            // 반대 쪽도 input[pivot]보다 크면 계속 j--; 
            while (input[j] > input[pivot])
            {
                j--;
            }
            // i가 j보다 작다면, 즉 아직 Pivot 기준으로 좌측은 작은 값들만 우측은 큰 값들로만 정렬이 안되었다면 해당 값 Swap
            if (i < j)
            {
                temp = input[i];
                input[i] = input[j];
                input[j] = temp;
            }
        }
    
        // j하고 pivot하고 Swap
        temp = input[pivot];
        input[pivot] = input[j];
        input[j] = temp;
 
        // 나머지 Partition들도 재귀적으로 quickSort를 해줘야하니
        // 새로운 Pivot인 j를 기준으로 
        // first <-> j - 1 & j + 1 <-> last로 quickSort 진행
        quickSort(first, j - 1);
        quickSort(j + 1, last);
    }
}

////////// QuickSort End //////////

////////// CountingSort Start //////////
// 정말 단순하게 각 숫자가 몇번 등장하는지 Count를 하고
// 누적합으로 바꿔준다 (큰 수가 포함되어 있는 경우의 비효율성을 최소화하기 위해)
// 이렇게 정렬된 배열은 뒤에서 앞으로 순회하면서 Sorted Array로 새로 넣어준다
// 시간복잡도 O(n)

#define MAX_DIGIT 10
#define MAX_N 100

int arr[MAX_N];
int cnt[MAX_DIGIT];
int sortedArr[MAX_N];

void calculateDigitNumber(int input_size)
{
    for (int i = 0; i < input_size; i++)
    {
        cnt[arr[i]]++;
    }
 
    for (int i = 1; i < MAX_DIGIT; i++)
    {
        cnt[i] = cnt[i-1] + cnt[i];
    }
}
 
void executeCountingSort(int input_size)
{
    for (int i = input_size - 1; i >= 0; i--)
    {
        sortedArr[--cnt[arr[i]]] = arr[i];
    }
}

////////// CountingSort End //////////

////////// BubbleSort Start //////////

void bubbleSort()
{
    int i, j, temp;
    for (i = 0; i < input_size - 1; i++)
        for (j = 0; j < input_size - i - 1; j++)
            if (input[j] > input[j + 1])
            {
                temp = input[j]; input[j] = input[j + 1]; input[j + 1] = temp;
            }
}

////////// BubbleSort End //////////

////////// MergeSort Start //////////

void merge(int l, int m, int r)
{
    int n1 = m - l + 1, n2 = r - m;
    int L[n1], R[n2];
    for (int i = 0; i < n1; i++) L[i] = input[l + i];
    for (int i = 0; i < n2; i++) R[i] = input[m + 1 + i];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) input[k++] = (L[i] <= R[j]) ? L[i++] : R[j++];
    while (i < n1) input[k++] = L[i++];
    while (j < n2) input[k++] = R[j++];
}

void mergeSort(int l, int r)
{
    if (l < r)
    {
        int m = l + (r - l) / 2;
        mergeSort(l, m);
        mergeSort(m + 1, r);
        merge(l, m, r);
    }
}

////////// MergeSort End //////////

////////// TopologicalSort Start //////////

vector<int> adj[MAX_NUM];
int indegree[MAX_NUM];

void topologicalSort(int n)
{
    queue<int> q;
    for (int i = 0; i < n; i++)
        if (indegree[i] == 0) q.push(i);

    cout << "Topological Sort: ";
    while (!q.empty())
    {
        int u = q.front(); q.pop();
        cout << u << " ";
        for (int v : adj[u])
        {
            indegree[v]--;
            if (indegree[v] == 0) q.push(v);
        }
    }
    cout << endl;
}

void prepareDAGFromInput()
{
    fill(indegree, indegree + MAX_NUM, 0);
    for (int i = 0; i < MAX_NUM; i++) adj[i].clear();

    for (int i = 0; i < input_size - 1; i++)
    {
        int u = input[i];
        int v = input[i + 1];
        adj[u].push_back(v);
        indegree[v]++;
    }
}

////////// SelectionSort Start //////////
// Selection Sort는 매번 남은 배열에서 최소값을 찾아
// 현재 위치와 Swap하는 방식으로 정렬하는 알고리즘이다
// 시간복잡도 Best O(n^2) Worst O(n^2)
// Swap 횟수가 적다는 특징이 있음

void selectionSort()
{
    int i, j, min_idx, temp;

    for (i = 0; i < input_size - 1; i++)
    {
        // i번째 위치에 올 최소값을 찾음
        min_idx = i;
        for (j = i + 1; j < input_size; j++)
        {
            if (input[j] < input[min_idx])
                min_idx = j;
        }

        // 최소값과 현재 위치를 Swap
        if (min_idx != i)
        {
            temp = input[i];
            input[i] = input[min_idx];
            input[min_idx] = temp;
        }
    }
}

////////// SelectionSort End //////////

////////// TopologicalSort End //////////

void printInput()
{
    int i;

    for (i = 0; i < input_size; ++i)
    {
        printf("%d", input[i]);
    }
    printf("\n");
}

int main()
{
    input_size = 10;

    input[0] = 2;
    input[1] = 4;
    input[2] = 6;
    input[3] = 5;
    input[4] = 10;
    input[5] = 1;
    input[6] = 7;
    input[7] = 8;
    input[8] = 3;
    input[9] = 0;

    /*
    // Insertion Sort
    printInput();
    insertionSort();
    printInput();

    // Quick Sort
    printInput();
    quickSort(0, input_size - 1);
    printInput();

    // Counting Sort
    printInput();
    calculateDigitNumber(input_size);
    executeCountingSort(input_size);
    printInput();

    // Bubble Sort
    printInput();
    bubbleSort();
    printInput();

    // Merge Sort
    printInput();
    mergeSort(0, input_size - 1);
    printInput();

    // Selection Sort
    printInput();
    selectionSort();
    printInput();
    */

    // Topological Sort
    printInput();
    prepareDAGFromInput();
    topologicalSort(MAX_NUM); // topological sort of DAG nodes from input
    printInput();
}
