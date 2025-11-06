#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const ll INF = (1LL<<60);

struct Edge {
    int to;     // 도착 정점
    int rev;    // 역간선의 인덱스 (to의 adjacency 리스트에서)
    ll cap;     // 남은 용량
    Edge(int _to, int _rev, ll _cap) : to(_to), rev(_rev), cap(_cap) {}
};

struct Dinic {
    int N;
    vector<vector<Edge>> G;
    vector<int> level;   // 레벨 그래프
    vector<int> it;      // current edge iterator (work array)

    Dinic(int n = 0) { init(n); }

    void init(int n) {
        N = n;
        G.assign(N+1, {});
        level.assign(N+1, -1);
        it.assign(N+1, 0);
    }

    // 무향(용량 양방향) 아닌, 일반적인 유향 간선 추가.
    // 만약 같은 간선을 여러 번 추가하면 누적됩니다.
    void add_edge(int u, int v, ll cap) {
        G[u].emplace_back(v, (int)G[v].size(), cap);
        G[v].emplace_back(u, (int)G[u].size() - 1, 0); // 역간선: 초기 용량 0
    }

    // BFS로 레벨 그래프 구성 (S에서의 거리)
    bool bfs(int s, int t) {
        fill(level.begin(), level.end(), -1);
        queue<int> q;
        level[s] = 0;
        q.push(s);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (const Edge &e : G[u]) {
                if (e.cap > 0 && level[e.to] < 0) {
                    level[e.to] = level[u] + 1;
                    q.push(e.to);
                }
            }
        }
        return level[t] >= 0;
    }

    // DFS로 blocking flow 찾기 (재귀)
    ll dfs(int u, int t, ll f) {
        if (u == t) return f;
        for (int &i = it[u]; i < (int)G[u].size(); ++i) {
            Edge &e = G[u][i];
            if (e.cap > 0 && level[e.to] == level[u] + 1) {
                ll ret = dfs(e.to, t, min(f, e.cap));
                if (ret > 0) {
                    e.cap -= ret;
                    G[e.to][e.rev].cap += ret;
                    return ret;
                }
            }
        }
        return 0LL;
    }

    // s에서 t로의 최대 유량 반환
    ll max_flow(int s, int t) {
        ll flow = 0;
        while (bfs(s, t)) {
            fill(it.begin(), it.end(), 0);
            while (true) {
                ll f = dfs(s, t, INF);
                if (f == 0) break;
                flow += f;
            }
        }
        return flow;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    if (!(cin >> N >> M)) return 0;
    int S, T;
    cin >> S >> T;

    Dinic dinic(N);
    for (int i = 0; i < M; ++i) {
        int u, v;
        long long c;
        cin >> u >> v >> c;
        // 유향 간선으로 추가 (무향이면 add_edge(u,v,c); add_edge(v,u,c);)
        dinic.add_edge(u, v, c);
    }

    long long maxflow = dinic.max_flow(S, T);
    cout << maxflow << '\n';

    return 0;
}
