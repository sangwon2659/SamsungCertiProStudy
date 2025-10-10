#include <iostream>
using namespace std;

////////// Binary Search Start //////////

#define MAX_M 100
 
int T;    // # of test case
int M;    // # of element in array
int N;    // # of numbers to search
int arr[MAX_M];
 
void binarySearch(int* arr, int low, int high, int target)
{
    int mid;
    if (low > high) 
    {
        printf("-1 ");
        return;
    }
 
    mid = (low + high) / 2;
 
    if (target < arr[mid])
    {
        binarySearch(arr, low, mid - 1, target);
    }
    else if (arr[mid] < target)
    {
        binarySearch(arr, mid + 1, high, target);
    }
    else
    {
        printf("%d ", mid);
        return;
    }
}

////////// Binary Search End //////////

int main()
{
}
