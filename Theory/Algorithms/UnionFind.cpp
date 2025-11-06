/*
Union-Find, also called Disjoint Set Union (DSU), is a data structure that efficiently keeps track of a collection of disjoint (non-overlapping) sets.
It’s especially useful when you need to:
- Group items into sets
- Quickly check whether two items belong to the same set
- Efficiently merge two sets together
*/

#include <iostream>
#include <vector>

class UnionFind {
private:
    // Parent라고 하는 Root 개념의 Vector가 있고, Rank라고 하는 Tree의 Rough Height를 보여주는 개념이 있다
    // Rank는 Optimization에 사용됨
    std::vector<int> parent;
    std::vector<int> rank;

public:
    // Constructor: create n disjoint sets (0 to n-1)
    // Disjoint set는 말 그대로 연결성이 없는 집합이다
    UnionFind(int n) {
        parent.resize(n);
        rank.resize(n, 0);
        for (int i = 0; i < n; ++i)
            parent[i] = i; // each node is its own parent initially
    }

    // Find the representative (root) of the set that x belongs to
    int find(int x) {
        if (parent[x] != x) {
            // Path compression: make the parent of x directly the root
            // 이렇게 Recursive하게 Parent를 같은 Root로 설정함으로써 Compress됨
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }

    // Union the sets containing x and y
    void unite(int x, int y) {
        int rootX = find(x);
        int rootY = find(y);

        if (rootX == rootY)
            return; // already in the same set

        // Union by rank: attach smaller tree under the root of the larger tree
        if (rank[rootX] < rank[rootY]) {
            parent[rootX] = rootY;
        } else if (rank[rootX] > rank[rootY]) {
            parent[rootY] = rootX;
        // Rank is a rough height of the tree
        // The rank of the tree should be increased when two sets of the same height are merged
        } else {
            parent[rootY] = rootX;
            rank[rootX]++;
        }
    }

    // Check if x and y are in the same set
    bool connected(int x, int y) {
        return find(x) == find(y);
    }
};

// Example usage
int main() {
    UnionFind uf(10); // create 10 elements (0..9)

    uf.unite(1, 2);
    uf.unite(2, 3);
    uf.unite(4, 5);

    std::cout << std::boolalpha;
    std::cout << "1 and 3 connected? " << uf.connected(1, 3) << "\n"; // true
    std::cout << "1 and 4 connected? " << uf.connected(1, 4) << "\n"; // false

    uf.unite(3, 4); // connect two groups

    std::cout << "1 and 4 connected (after unite)? " << uf.connected(1, 4) << "\n"; // true

    return 0;
}
