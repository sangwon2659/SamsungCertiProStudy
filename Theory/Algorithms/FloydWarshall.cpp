// Floyd–Warshall Algorithm
// 모든 노드 쌍 (u, v)에 대해 최단 거리를 탐색함
// 음수 가중치도 허용하지만, 음수 사이클(Negative Cycle)이 있는 경우에는 올바른 최단거리 계산 불가
// 모든 노드 간의 최단 거리를 한 번에 구할 수 있음 (All-Pairs Shortest Path Algorithm)
// 중간에 pre-computed 값들을 사용하는 개념이기에 Dynamic Programming의 일종임

#include <iostream>
#include <vector>
#include <iomanip> // 자동으로 align을 해주는 setw를 사용을 위해
using namespace std;
using ll = long long;

const int INF = 1e9;

// N: Number of Nodes
// vDistMatrix: 인접 행렬 (distance matrix)
vector<vector<ll>> floydWarshall(int N, vector<vector<ll>>& vDistMatrix)
{
    // Floyd-Warshall 핵심 로직:
    // 각 노드를 중간 노드(k)로 설정하고,
    // i → j로 가는 최단 경로가 i → k → j를 통해 더 짧아지는지 확인함
    // 세 개의 반복문으로 모든 조합을 탐색
    for (int k = 0; k < N; k++) // 중간 노드
    {
        for (int i = 0; i < N; i++) // 시작 노드
        {
            for (int j = 0; j < N; j++) // 도착 노드
            {
                // i → j 경로가 i → k → j를 거쳐 더 짧다면 업데이트
                if (vDistMatrix[i][k] != INF && vDistMatrix[k][j] != INF)
                {
                    if (vDistMatrix[i][j] > vDistMatrix[i][k] + vDistMatrix[k][j])
                    {
                        vDistMatrix[i][j] = vDistMatrix[i][k] + vDistMatrix[k][j];
                    }
                }
            }
        }
    }

    // Negative Cycle Detection
    // dist[i][i]가 0보다 작다면, 음수 사이클 존재
    for (int i = 0; i < N; i++)
    {
        if (vDistMatrix[i][i] < 0)
        {
            cout << "Negative cycle detected!" << endl;
            break;
        }
    }

    return vDistMatrix;
}

int main()
{
    int N = 4; // 노드 개수
    vector<vector<ll>> vDistMatrix(N, vector<ll>(N, INF));

    // 자기 자신으로의 거리는 항상 0으로 설정
    for (int i = 0; i < N; i++)
        vDistMatrix[i][i] = 0;

    // Lambda 함수를 사용하여 간선을 쉽게 추가
    auto addEdge = [&](int u, int v, int w) {
        vDistMatrix[u][v] = w; // Directed edge (방향 그래프)
        // dist[v][u] = w; // Uncomment if Undirected
    };

    // 예시 그래프 구성
    addEdge(0, 1, 5);
    addEdge(0, 3, 10);
    addEdge(1, 2, 3);
    addEdge(2, 3, 1);

    // Floyd-Warshall 수행
    vector<vector<ll>> result = floydWarshall(N, vDistMatrix);

    // 결과 출력
    cout << "All-Pairs Shortest Distances:" << endl;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (result[i][j] == INF)
                cout << setw(7) << "INF";
            else
                cout << setw(7) << result[i][j];
        }
        cout << endl;
    }

    return 0;
}
