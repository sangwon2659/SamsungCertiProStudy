#include <iostream>
using namespace std;

#define MAX_NUM 100

int input[MAX_NUM];
int input_size;

// Ins

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
    input_size = 6;

    input[0] = 2;
    input[1] = 4;
    input[2] = 6;
    input[3] = 5;
    input[4] = 10;
    input[5] = 1;

    printInput();
    insertionSort();
    printInput();
}
