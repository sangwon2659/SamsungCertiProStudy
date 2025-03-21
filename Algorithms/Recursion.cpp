#include <iostream>
using namespace std;

long long factorial(int num)
{
    if (num == 0) return 1;

    return num * factorial (num - 1);
}

int main()
{
    long long value = factorial(20);
    printf("%lld", value);
}
