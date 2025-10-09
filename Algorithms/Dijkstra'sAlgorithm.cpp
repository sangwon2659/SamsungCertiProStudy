// Dijkstra's Algorithm
// 시작점에서부터 갈 수 있는 모든 노드의 가장 짧은 경로를 비교하면서 PQ에 넣어서 Greedy하게 Global Optimal을 탐색함
// Start Node가 있어야 하고 edgeWeight가 음수인 경우가 있으면 안된다

#include <iostream>
#include <vector>
#include <queue>

using namespace std;
using ll = long long;

const int INF = 1 << 30;

// N: Number of Nodes
// AdjList: Adjacent List (0 -> N-1까지 노드별로 {adjNode, edgeWeight} 구조의 Pair들이 들어있음)
// S: Start Node
vector<ll> dijkstra(int N, int S, vector<vector<pair<int, int>>> vAdjList)
{
    // Priority Queue는 다음 구조
    //    class T,                        // the type of stored elements
    //    class Container = vector<T>,    // the underlying container
    //    class Compare = less<typename Container::value_type>
    // 그렇기 때문에 vector<pair<ll, int>>도 넣어주어야 함
    priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<pair<ll, int>>> pq;
    // ll으로 이루어진 dist 선언
    // 선언할 때는 모두 INF로 지정
    vector<ll> vDist(N, INF);

    // StartNode부터 StartNode는 Distance가 0
    vDist[S] = 0;
    // {Dist: 0, Node: S}인 Initial Pair를 pq에 push
    pq.push({0, S});

    while (!pq.empty())
    {
        // pq로부터 가장 Min의 dist와 node 정보 받기
        auto [dist, node] = pq.top();
        // 정보 받은 Element는 pop하기
        pq.pop();

        // 뽑은 노드가 갈 수 있는 adjNode 중 더 효율적인 경로가 있는지 탐색하기
        for (auto [adjNode, edgeWeight] : vAdjList[node])
        {
            if (dist + edgeWeight < vDist[adjNode])
            {
                // 기존의 dist보다 새로운 dist + edgeWeight가 값이 낮으면 vDist값을 업데이트 하기
                vDist[adjNode] = dist + edgeWeight;
                // vDist값 업데이트 후 pq에도 pair를 push하기
                pq.push({vDist[adjNode], adjNode});
            }
        }
    }

    return vDist;
}

int main()
{
    int N = 6;
    vector<vector<pair<int,int>>> vAdjList(N);
    // 여기서 [=]를 하면 Capture by Value라서 vAdjList의 값이 실제로 바뀌지 않음
    // 꼭 [&]를 해줘야 Capture by Reference라서 vAdjList의 값이 실제로 반영됨
    auto add_edge = [&](int u, int v, int w) {
        vAdjList[u].push_back({v,w});
        vAdjList[v].push_back({u,w}); // comment this line if directed
    };
    add_edge(0,1,7);
    add_edge(0,2,9);
    add_edge(0,5,14);
    add_edge(1,2,10);
    add_edge(1,3,15);
    add_edge(2,3,11);
    add_edge(2,5,2);
    add_edge(3,4,6);
    add_edge(4,5,9);

    int S = 0;
    vector<ll> result = dijkstra(N, S, vAdjList);

    for (auto it : result)
    {
        cout << it << endl;
    }

    return 0;
}
