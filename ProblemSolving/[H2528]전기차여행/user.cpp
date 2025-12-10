//user.cpp
#include <unordered_map>
#include <vector>
#include <queue>
 
using namespace std;

 
constexpr int MAXN = 505;                       // 도시의 최대 개수 +5
constexpr int MAXR = 7005;                      // 도로의 최대 개수 +5
constexpr int MAXB = 305;                       // 배터리 최대 용량 +5
constexpr int INF = 1 << 30;
 
int idCnt;                                      // 도로의 새로운 ID 부여 변수 (도로의 개수)
unordered_map<int, int> idMap;                    // 도로의 새로운 ID 부여 mapping hash
 
int N;                                          // 도시의 개수
struct City {
    int charge;                                 // 시간 당 충전량
    int spreadT;                                // 전염병이 퍼진 시간
    vector<int> adj;                          // 도로 ID 리스트 (그래프)
} cityList[MAXN];
 
struct Road {
    int mId;                                    // mId
    int s, e;                                   // 출발, 도착 도시 번호
    int t;                                      // 소요 시간
    int p;                                      // 전력 소모량
} roadList[MAXR];
 
// dijkstra 변수
struct Data {
    int time;                                   // 시간
    int city;                                   // 도시 번호
    int battery;                                // 충전 용량
 
    bool operator< (const Data& t) const {
        return time > t.time;
    }
};
 
priority_queue<Data> pq;
int dist[MAXN][MAXB];
 
void add(int mId, int sCity, int eCity, int mTime, int mPower);
 
void init(int N, int mCharge[], int K, int mId[], int sCity[], int eCity[], int mTime[], int mPower[]) {
    // 변수 초기화
    ::N = N;
    idCnt = 0; idMap.clear();
    for (int i = 0; i < N; ++i) {
        cityList[i].adj.clear();
        cityList[i].charge = mCharge[i];
    }
    for (int i = 0; i < K; ++i) add(mId[i], sCity[i], eCity[i], mTime[i], mPower[i]);
}
 
void add(int mId, int sCity, int eCity, int mTime, int mPower) {
    // 새로운 ID 부여 및 도로 정보 저장
    int newID = idCnt++;
    idMap[mId] = newID;
    roadList[newID] = { mId, sCity, eCity, mTime, mPower };
 
    // 그래프 업데이트 (단방향 그래프)
    cityList[sCity].adj.push_back(newID);
}
 
void remove(int mId) {
    // 그래프에서 도로 삭제
    int rId = idMap[mId];
    int sCity = roadList[rId].s;
    vector<int>& vec = cityList[sCity].adj;
    for (int i = 0; i < vec.size(); ++i) {
        if (vec[i] == rId) {
            vec[i] = vec.back();
            vec.pop_back();
            return;
        }
    }
}
 
int cost(int B, int sCity, int eCity, int M, int mCity[], int mTime[]) {
    // 전염병이 도시에 퍼지는 시간을 구하기 위한 dijkstra 수행
    pq = {};
    for (int i = 0; i < MAXN; ++i) cityList[i].spreadT = INF;
 
    // 초기 전염병을 PQ에 삽입
    for (int i = 0; i < M; ++i) {
        pq.push({ mTime[i], mCity[i] });
        cityList[mCity[i]].spreadT = mTime[i];
    }
 
    while (!pq.empty()) {
        Data d = pq.top();
        pq.pop();
        int curT = d.time, cur = d.city;
        if (cityList[cur].spreadT < curT) continue;          // 이미 전염된 경우
 
        for (int rId : cityList[cur].adj) {
            int next = roadList[rId].e;                     // 다음 이동 도시
            int nextT = curT + roadList[rId].t;             // 다음 도시까지 전파되는 시간
            if (cityList[next].spreadT > nextT) {
                cityList[next].spreadT = nextT;
                pq.push({ nextT, next });
            }
        }
    }
 
    // dijkstra를 사용하여 전기차가 eCity까지 갈 수 있는 최단 시간 경로 구하기
    pq = {};
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < MAXB; ++j)
            dist[i][j] = INF;
 
    pq.push({ 0, sCity, B });                   // { 소요 시간, 도시, 남은 충전 용량 }
    dist[sCity][B] = 0;
 
    while (!pq.empty()) {
        Data d = pq.top();
        pq.pop();
        int curT = d.time, cur = d.city, curB = d.battery;  // 소요 시간, 도시, 충전 용량
        if (dist[cur][curB] < curT) continue;                // 이미 방문한 적이 있는 경우
 
        // 전기차가 eCity에 도달한 경우 최소 시간 반환
        if (cur == eCity) return curT;
 
        for (int rId : cityList[cur].adj) {
            int next = roadList[rId].e;
            int p = roadList[rId].p;
 
            // 배터리가 부족한 경우
            if (curB < p) continue;
 
            // 배터리가 충분한 경우 도로 지나가기
            int nextT = curT + roadList[rId].t;
            if (cityList[next].spreadT <= nextT) continue;       // nexT에 이미 도시가 전염 된 경우
 
            int nextB = curB - p;
            if (dist[next][nextB] > nextT) {
                // pruning : 불필요한 노드는 방문하지 않도록 dist 배열 값 업데이트
                int b = nextB;
                while (b >= 0 && dist[next][b] >= nextT) dist[next][b--] = nextT;
                pq.push({ nextT, next, nextB });
            }
        }
 
        // cur에서 배터리를 충전하는 경우
        if (curB < B) {
            // 시간을 1 증가시키고 충전 수행
            int nextT = curT + 1;
            if (cityList[cur].spreadT <= nextT) continue;        // 도시가 전염 된 경우
 
            int nextB = min(curB + cityList[cur].charge, B);
            if (dist[cur][nextB] > nextT) {
                // pruning : 불필요한 노드는 방문하지 않도록 dist 배열 값 업데이트
                int b = nextB;
                while (b >= 0 && dist[cur][b] >= nextT) dist[cur][b--] = nextT;
                pq.push({ nextT, cur, nextB });
            }
        }
    }
    return -1;          // 전기차가 eCity에 도달하지 못하는 경우
}
