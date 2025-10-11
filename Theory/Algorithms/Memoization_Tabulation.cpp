/*
복잡한 문제를 간단한 여러 개의 부분 문제(subproblem)로 나누고, 
부분 문제에 대한 해답을 활용하여 전체 문제를 해결하는 문제 해결 방식
분할 정복법(divide and conquer)에서는 분할된 부분 문제들이 서로 겹치지 않지만, 
동적 계획법에서는 중복되는 부분 문제가 발생한다는 점이 다름

//동적계획법 필요 조건
1.중복되는 부분 문제(overlapping subproblems)
주어진 문제를 여러 개의 부분 문제로 분할할 경우, 
몇몇 부분 문제가 반복되어 나타나는 현상
부분 문제의 솔루션을 저장하여 같은 부분 문제를 중복해서 계산하지 않도록 설정
2.최적 부분 구조(optimal substructure)
부문 문제의 최적해(optimal solution)를 조합하여 전체 문제의 최적해를 구할 수 있는 경우를 지칭

//동적 계획법 문제의 해결 방법
기저 조건(base case) 정의
상태 전환(state transition)을 나타내는 순환식 정의
순환식을 메모이제이션(memoization) 또는 타뷸레이션(tabulation) 방식으로 풀이

//메모이제이션
재귀 호출이 많이 발생하므로, 함수 호출에 따른 오버헤드(overhead)가 있음
*오버헤드란 프로그램의 실행흐름 도중에 동떨어진 위치의 코드를 실행해야할 때, 추가적으로 시간,메모리,자원이 사용되는 현상
경우에 따라 모든 부분 문제를 풀지 않아도 문제에 대한 해답을 구할 수 있음
캐시로 사용하는 표의 일부만 필요에 따라 계산하여 채움

//타뷸레이션
표(배열)에 저장된 값을 참조하는 방식이므로 빠르게 동작
상향식(bottom-up) 방식이기 때문에 모든 부분 문제에 대한 해답을 구해서 표에 저장
표의 맨 처음부터 끝까지 차례대로 계산하여 모두 채움
*/

#include <iostream>
#include <vector>

vector<int> memo(50, -1);

int fibo_memoization(int n)
{
	if (n < 2)
		return n;

	if (memo[n] != -1)
		return memo[n];
	
	memo[n] = fibo_memoization(n - 2) + fibo_memoization(n - 1);
	return memo[n];
}

int fibo_tabulation(int n)
{
	vector<int> dp(n + 1, 0);
	dp[1] = 1;

	for (int i = 2; i <= n; i++)
		dp[i] = dp[i - 2] + dp[i - 1];

	return dp[n];
}

int main()
{
    int n = 5;
    cout << fibo_memoization(n) << endl;
    cout << fibo_tabulation(n) << endl;
    cout << endl;
}
