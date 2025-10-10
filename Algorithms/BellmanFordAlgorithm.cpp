// Bellman-Ford Algorithm
// 시작점에서부터 갈 수 있는 모든 노드의 최단거리를 탐색함 (Single Source Shortest Path)
// edgeWeight가 음수인 경우도 허용함
// Negative Cycle이 존재하는지도 감지 가능함

#include <iostream>
#include <vector>
#include <queue>

using namespace std;
using ll = long long;

const int INF = 1 << 30;

// N: Number of Nodes
// vEdgeList: 간선 리스트 (u, v, w) 형태로 저장됨 (u에서 v로 가는 weight w의 간선)
// S: Start Node
vector<ll> bellmanford(int N, int S, const vector<tuple<int,int,int>>& vEdgeList)
{
    // 거리 배열 초기화 (모든 노드를 INF로 설정)
    vector<ll> vDist(N, INF);
    // 시작점까지의 거리는 0
    vDist[S] = 0;

    // Bellman-Ford 핵심 로직:
    // 모든 간선을 N-1번 반복하며 Relaxation(거리 갱신)을 시도
    // 이유: 최단 경로는 최대 N-1개의 간선을 포함하기 때문
    //      (a simple path that does not revisit any node can have at most N-1 edges)
    //      예외 없이 모든 경우의 수를 탐색하는 방식이라 생각하면 됨
    for (int i = 0; i < N - 1; i++)
    {
        // 각 간선(u → v, weight w)에 대해
        for (auto [u, v, w] : vEdgeList)
        {
            // 현재 u에서 v로 가는 더 짧은 경로가 발견되면 업데이트
            // vDist[u] != INF를 조건으로 넣는 이유는 vDist[u] == INF인 경우에는 해당 Node가 아직 Unreachable이기 때문
            // 이 조건이 없으면 Overflow날 수도 있음
            if (vDist[u] != INF && vDist[u] + w < vDist[v])
            {
                vDist[v] = vDist[u] + w;
            }
        }
    }

    // Negative Cycle Detection
    // 한 번 더 Relaxation 시도했을 때 값이 줄어들면 Negative Cycle 존재
    // 할 수 있는 모든 조합을 본 상태인데도 값이 줄어들면 무한 음수 사이클에 빠졌다는 것이기 때문
    for (auto [u, v, w] : vEdgeList)
    {
        if (vDist[u] != INF && vDist[u] + w < vDist[v])
        {
            cout << "Negative cycle detected!" << endl;
            break;
        }
    }

    return vDist;
}

int main()
{
    int N = 5;
    vector<tuple<int,int,int>> vEdgeList;

    // Lambda 함수를 사용하여 간선을 쉽게 추가
    // [&]를 써야 실제 edges 벡터가 참조로 캡처되어 수정됨
    auto addEdge = [&](int u, int v, int w) {
        vEdgeList.push_back({u, v, w}); // Directed Edge
        // edges.push_back({v, u, w}); // Uncomment if Undirected
    };

    // 예시 그래프 구성
    addEdge(0, 1, 6);
    addEdge(0, 2, 7);
    addEdge(1, 2, 8);
    addEdge(1, 3, 5);
    addEdge(1, 4, -4);
    addEdge(2, 3, -3);
    addEdge(2, 4, 9);
    addEdge(3, 1, -2);
    addEdge(4, 3, 7);

    int S = 0; // 시작 노드 (0번)
    vector<ll> result = bellmanford(N, S, vEdgeList);

    cout << "Shortest distances from node " << S << ":" << endl;
    for (int i = 0; i < N; i++)
    {
        if (result[i] == INF)
            cout << "Node " << i << ": INF" << endl;
        else
            cout << "Node " << i << ": " << result[i] << endl;
    }

    return 0;
}
