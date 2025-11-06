#include <bits/stdc++.h>
using namespace std;

// Hopcroft-Karp algorithm for maximum bipartite matching
// Left side nodes are 1..nL, right side nodes are 1..nR (we store right indices in adjacency lists for left nodes).
// Complexity: O(E * sqrt(V))

struct HopcroftKarp {
    int nL, nR;
    vector<vector<int>> adj; // adj[u] = list of v (1..nR) connected to left node u
    vector<int> pairU, pairV, dist;
    const int INF = 1e9;

    HopcroftKarp(int nL_, int nR_) : nL(nL_), nR(nR_) {
        adj.assign(nL + 1, {});
        pairU.assign(nL + 1, 0);
        pairV.assign(nR + 1, 0);
        dist.assign(nL + 1, 0);
    }

    void addEdge(int u, int v) {
        if (u < 1 || u > nL || v < 1 || v > nR) throw runtime_error("addEdge: node out of range");
        adj[u].push_back(v);
    }

    bool bfs() {
        queue<int> q;
        for (int u = 1; u <= nL; ++u) {
            if (pairU[u] == 0) {
                dist[u] = 0;
                q.push(u);
            } else {
                dist[u] = INF;
            }
        }
        int foundDist = INF;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            if (dist[u] >= foundDist) continue;
            for (int v : adj[u]) {
                int pu = pairV[v];
                if (pu == 0) {
                    // free node on right, we found an augmenting layer
                    foundDist = dist[u] + 1;
                } else if (dist[pu] == INF) {
                    dist[pu] = dist[u] + 1;
                    q.push(pu);
                }
            }
        }
        return foundDist != INF;
    }

    bool dfs(int u) {
        for (int v : adj[u]) {
            int pu = pairV[v];
            if (pu == 0 || (dist[pu] == dist[u] + 1 && dfs(pu))) {
                pairU[u] = v;
                pairV[v] = u;
                return true;
            }
        }
        dist[u] = INF; // mark as visited in this phase
        return false;
    }

    int maxMatching() {
        int matching = 0;
        while (bfs()) {
            for (int u = 1; u <= nL; ++u) {
                if (pairU[u] == 0 && dfs(u))
                    ++matching;
            }
        }
        return matching;
    }

    // Optional: return vector of pairs (u,v) for matched edges (u in left, v in right)
    vector<pair<int,int>> getMatchingPairs() {
        vector<pair<int,int>> res;
        for (int u = 1; u <= nL; ++u) if (pairU[u] != 0) res.emplace_back(u, pairU[u]);
        return res;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Example:
    // Left side: 1..4
    // Right side: 1..4
    // Edges:
    // 1 - {1,2}
    // 2 - {1}
    // 3 - {2,3}
    // 4 - {2,4}
    //
    // Maximum matching size is 4 in this example (one possible matching: 1-1, 2-?, 3-3, 4-4)
    // We'll build a graph and compute it.

    int nL = 4, nR = 4;
    HopcroftKarp hk(nL, nR);

    hk.addEdge(1, 1);
    hk.addEdge(1, 2);
    hk.addEdge(2, 1);
    hk.addEdge(3, 2);
    hk.addEdge(3, 3);
    hk.addEdge(4, 2);
    hk.addEdge(4, 4);

    int result = hk.maxMatching();
    cout << "Maximum matching size = " << result << "\n";

    auto pairs = hk.getMatchingPairs();
    for (auto &p : pairs) {
        cout << "Left " << p.first << " -- Right " << p.second << "\n";
    }

    return 0;
}
