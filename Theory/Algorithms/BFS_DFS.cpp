#include <iostream>
#include <vector>
#include <stack>
#include <queue>

const int N = 6;
bool gVisited[N] = {};
void dfs_recursion(const vector<vector<int>>& adj_list, int s)
{
	if (gVisited[s])
		return;

	gVisited[s] = true;
	cout << s << ", ";

	for (int v : adj_list[s])
		dfs_recursion(adj_list, v);
}

vector<int> dfs(const vector<vector<int>>& adj_list, int s)
{
	vector<bool> visited(adj_list.size(), false);
	vector<int> visit_order;
	stack<int> stk;
	stk.push(s);

	while (!stk.empty()) {
		int v = stk.top();
		stk.pop();

		if (visited[v])
			continue; // 이미 방문했으면 스킵!

		visited[v] = true; // 정점 v를 방문
		visit_order.push_back(v);

		for (int a : adj_list[v]) {
			if (!visited[a])
				stk.push(a);
		}
	}

	return visit_order;
}

vector<int> bfs(const vector<vector<int>>& adj_list, int s)
{
	vector<bool> visited(adj_list.size(), false);
	vector<int> visit_order;
	queue<int> q;
	q.push(s);

	while (!q.empty()) {
		int v = q.front();
		q.pop();

		if (visited[v])
			continue; // 이미 방문했으면 스킵!

		visited[v] = true; // 정점 v를 방문
		visit_order.push_back(v);

		for (int a : adj_list[v]) {
			if (!visited[a])
				q.push(a);
		}
	}

	return visit_order;
}

int main()
{
  vector<vector<int>> adj_list = {
  	{1, 3, 4},
  	{0, 2, 4},
  	{1, 5},
  	{0, 4},
  	{0, 1, 3},
  	{2}
  };

  cout << "dfs_recursion: ";
  dfs_recursion(adj_list, 0);
  cout << endl;
  
  auto dfs_order = dfs(adj_list, 0);
  cout << "dfs_stack: ";
  for (auto n : dfs_order)
  cout << n << ", ";
  cout << endl;
  
  auto bfs_order = bfs(adj_list, 0);
  cout << "bfs_queue: ";
  for (auto n : bfs_order)
  cout << n << ", ";
  cout << endl;
}
