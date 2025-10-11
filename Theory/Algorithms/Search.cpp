/*
Contains the following Search Algorithms:
- Binary Search
- Parametric Search
*/

#include <iostream>
using namespace std;

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

// Function to check if a candidate "mid" value satisfies the problem's condition
bool check(int mid) {
    // Example condition: Can we divide array elements into <= mid segments?
    // This depends on the specific problem
    // Return true if "mid" is feasible, false otherwise
    return true;  // placeholder
}

// Parametric Search using binary search on answer space
int parametricSearch(int low, int high) {
    int ans = -1;
    while (low <= high) {
        int mid = (low + high) / 2;

        if (check(mid)) {   // If mid satisfies condition
            ans = mid;      // record it as a possible answer
            high = mid - 1; // try to find a better (smaller) answer
        } else {
            low = mid + 1;  // mid not feasible, go higher
        }
    }
    return ans;
}

int main()
{
    // Read input array
    cin >> M;
    for (int i = 0; i < M; i++) cin >> arr[i];

    // Perform parametric search in some answer range
    int result = parametricSearch(0, 1000);  // 0~1000 is just an example range
    cout << "Optimal value: " << result << "\n";
}
