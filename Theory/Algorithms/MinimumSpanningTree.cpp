#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const ll INF = (1LL<<62);

// ===== Kruskal (Union-Find) =====
struct DSU {
    int n;
    vector<int> p, r;
    DSU(int n=0): n(n), p(n+1), r(n+1,0) {
        for (int i=0;i<=n;i++) p[i]=i;
    }
    int find(int x){
        if (p[x]==x) return x;
        return p[x] = find(p[x]);
    }
    bool unite(int a, int b){
        a = find(a); b = find(b);
        if (a==b) return false;
        if (r[a] < r[b]) swap(a,b);
        p[b]=a;
        if (r[a]==r[b]) r[a]++;
        return true;
    }
};

struct Edge {
    int u, v;
    ll w;
    bool operator<(Edge const& other) const {
        return w < other.w;
    }
};

ll kruskal(int N, vector<Edge>& edges, vector<Edge>* out_mst = nullptr){
    sort(edges.begin(), edges.end());
    DSU dsu(N);
    ll total = 0;
    int used = 0;
    for (auto &e : edges){
        if (dsu.unite(e.u, e.v)){
            total += e.w;
            used++;
            if (out_mst) out_mst->push_back(e);
            if (used == N-1) break;
        }
    }
    // 그래프가 연결되어 있지 않으면 (MST 존재 X) INF 반환
    if (used != N-1) return INF;
    return total;
}

// ===== Prim (priority_queue) =====
// 인접리스트 사용 (1-indexed)
ll prim(int N, const vector<vector<pair<int,ll>>>& adj){
    vector<char> vis(N+1, 0);
    vector<ll> dist(N+1, INF);
    priority_queue<pair<ll,int>, vector<pair<ll,int>>, greater<pair<ll,int>>> pq;

    // 시작 정점 1 (임의의 정점으로 시작)
    dist[1] = 0;
    pq.push({0, 1});
    ll total = 0;
    int cnt = 0;

    while(!pq.empty()){
        auto [d, u] = pq.top(); pq.pop();
        if (vis[u]) continue;
        vis[u] = 1;
        total += d;
        cnt++;
        for (auto &ed : adj[u]){
            int v = ed.first;
            ll w = ed.second;
            if (!vis[v] && w < dist[v]){
                dist[v] = w;
                pq.push({dist[v], v});
            }
        }
    }
    if (cnt != N) return INF; // 연결 안됨
    return total;
}

// ===== 예제 main =====
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int N, M;
    if (!(cin >> N >> M)) return 0;
    vector<Edge> edges;
    vector<vector<pair<int,ll>>> adj(N+1);

    for (int i=0;i<M;i++){
        int u,v; ll w;
        cin >> u >> v >> w;
        // 무향 그래프 가정
        edges.push_back({u,v,w});
        adj[u].push_back({v,w});
        adj[v].push_back({u,w});
    }

    // Kruskal
    vector<Edge> mst_edges;
    ll kruskal_cost = kruskal(N, edges, &mst_edges);
    if (kruskal_cost == INF) {
        cout << "Kruskal: 그래프가 연결되어 있지 않아 MST가 존재하지 않습니다.\n";
    } else {
        cout << "Kruskal MST 비용 = " << kruskal_cost << "\n";
        // 필요하면 MST 간선 출력
        // cout << "간선들:\n";
        // for (auto &e : mst_edges) cout << e.u << ' ' << e.v << ' ' << e.w << '\n';
    }

    // Prim
    ll prim_cost = prim(N, adj);
    if (prim_cost == INF) {
        cout << "Prim: 그래프가 연결되어 있지 않아 MST가 존재하지 않습니다.\n";
    } else {
        cout << "Prim MST 비용 = " << prim_cost << "\n";
    }

    return 0;
}
