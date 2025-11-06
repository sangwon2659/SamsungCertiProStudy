#include <bits/stdc++.h>
using namespace std;

// Use long long for coordinates. Use __int128 for area accumulation to avoid overflow.
using ll = long long;
struct Event {
    ll x;
    int y1i, y2i; // indices in compressed y array (y1 index, y2 index)
    int type;     // +1 for entering, -1 for leaving
    bool operator<(Event const& o) const {
        if (x != o.x) return x < o.x;
        return type > o.type; // enter before leave at same x (not strictly necessary)
    }
};

struct SegTree {
    int n;                   // number of intervals (m-1 if m y-coords)
    vector<int> cover;       // cover count
    vector<ll> length;       // covered length in that node
    vector<ll> ys;           // reference to compressed y-coordinates

    SegTree(int _n = 0, const vector<ll> &_ys = {}) { init(_n, _ys); }

    void init(int _n, const vector<ll> &_ys) {
        n = _n;
        ys = _ys;
        cover.assign(4 * n, 0);
        length.assign(4 * n, 0);
    }

    // node covers interval [l, r) in index space; ql, qr are update ranges
    void update(int node, int l, int r, int ql, int qr, int delta) {
        if (ql >= r || qr <= l) return;            // no overlap
        if (ql <= l && r <= qr) {
            cover[node] += delta;
        } else {
            int mid = (l + r) >> 1;
            update(node<<1, l, mid, ql, qr, delta);
            update(node<<1|1, mid, r, ql, qr, delta);
        }

        if (cover[node] > 0) {
            // Entire segment of this node is covered
            length[node] = ys[r] - ys[l];
        } else {
            if (l + 1 == r) {
                length[node] = 0; // leaf interval not covered
            } else {
                length[node] = length[node<<1] + length[node<<1|1];
            }
        }
    }

    // convenience wrapper
    void updateRange(int ql, int qr, int delta) {
        if (ql >= qr) return;
        update(1, 0, n, ql, qr, delta);
    }

    // total covered length is at root
    ll coveredLength() const {
        return length[1];
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Input format:
    // n
    // for each rectangle: x1 y1 x2 y2
    // rectangle assumed axis-aligned, x1 < x2, y1 < y2 (if not, normalize)
    int n;
    if (!(cin >> n)) {
        cerr << "Failed to read n\n";
        return 0;
    }

    vector<array<ll,4>> rects;
    rects.reserve(n);
    vector<ll> ys;
    ys.reserve(2*n);
    for (int i = 0; i < n; ++i) {
        ll x1,y1,x2,y2;
        cin >> x1 >> y1 >> x2 >> y2;
        if (x1 > x2) swap(x1, x2);
        if (y1 > y2) swap(y1, y2);
        // Ignore degenerate rectangles
        if (x1 == x2 || y1 == y2) continue;
        rects.push_back({x1,y1,x2,y2});
        ys.push_back(y1);
        ys.push_back(y2);
    }

    if (rects.empty()) {
        cout << 0 << '\n';
        return 0;
    }

    // coordinate compress y
    sort(ys.begin(), ys.end());
    ys.erase(unique(ys.begin(), ys.end()), ys.end());

    // build events
    vector<Event> events;
    events.reserve(rects.size()*2);
    for (auto &r : rects) {
        ll x1 = r[0], y1 = r[1], x2 = r[2], y2 = r[3];
        int y1i = int(lower_bound(ys.begin(), ys.end(), y1) - ys.begin());
        int y2i = int(lower_bound(ys.begin(), ys.end(), y2) - ys.begin());
        // The segment tree will index intervals between ys[i] and ys[i+1], so we use [y1i, y2i)
        events.push_back({x1, y1i, y2i, +1});
        events.push_back({x2, y1i, y2i, -1});
    }

    sort(events.begin(), events.end());

    // segtree over intervals count = ys.size()-1
    int intervals = (int)ys.size() - 1;
    SegTree st(intervals, ys);

    // sweep
    __int128 area = 0;
    ll prevX = events[0].x;
    for (size_t i = 0; i < events.size(); ) {
        ll currX = events[i].x;
        // accumulate area from prevX to currX
        ll coveredY = st.coveredLength(); // covered y-length at prevX
        ll dx = currX - prevX;
        if (dx > 0 && coveredY > 0) {
            __int128 add = ( __int128 ) coveredY * dx;
            area += add;
        }

        // process all events at currX
        while (i < events.size() && events[i].x == currX) {
            st.updateRange(events[i].y1i, events[i].y2i, events[i].type);
            ++i;
        }
        prevX = currX;
    }

    // print __int128 area
    // convert to string
    if (area == 0) {
        cout << 0 << '\n';
    } else {
        bool neg = area < 0;
        __int128 tmp = area;
        if (neg) tmp = -tmp;
        string s;
        while (tmp > 0) {
            int digit = int(tmp % 10);
            s.push_back(char('0' + digit));
            tmp /= 10;
        }
        if (s.empty()) s = "0";
        if (neg) s.push_back('-');
        reverse(s.begin(), s.end());
        cout << s << '\n';
    }
    return 0;
}
