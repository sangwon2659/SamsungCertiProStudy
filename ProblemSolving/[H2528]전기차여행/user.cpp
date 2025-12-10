#include <unordered_map>
#include <vector>
#include <queue>
#include <cstring>

using namespace std;

// 최대 도시 개수
constexpr int MAX_N = 500;
// 주어질 수 있는 배터리 최대 용량
constexpr int MAX_B = 300;

typedef unsigned long long ull;

struct Road {
    // 도로 ID
    int id;
    // 도착도시
    int to;
    // 소요시간, 전력소모량
    int time, power;
};

// 도로 ID를 Indexing해서 출발도시를 찾을 수 있는 저장소
unordered_map<int, int> Hash;
// AdjacentNode
vector<Road> Graph[MAX_N];
// 도시의 시간당 충전량
int Charge[MAX_N];

// N: 도시의 개수 mCharge: 시간당 충전량
// K: 도로 개수 mId: 도로 ID sCity: 출발도시 eCity: 도착도시 mTime: 소요시간 mPower: 전력소모량
// 1 <= mId[i] <= 1,000,000,000
void init(int N, int mCharge[], int K, int mId[], int sCity[], int eCity[], int mTime[], int mPower[]) {
    Hash.clear();
    for (int i = 0; i < N; ++i) {
        Graph[i].clear();
        Charge[i] = mCharge[i];
    }
    for (int i = 0; i < K; ++i) {
        // Adjacent Node에 정보 저장
        Graph[sCity[i]].push_back({ mId[i], eCity[i], mTime[i], mPower[i] });
        // Hash에 도시 ID 저장
        Hash[mId[i]] = sCity[i];
    }
}

void add(int mId, int sCity, int eCity, int mTime, int mPower) {
    // 도로 추가
    Graph[sCity].push_back({ mId, eCity, mTime, mPower });
    // 도시 ID를 기준으로 출발도시 추가
    Hash[mId] = sCity;
}

void remove(int mId) {
    // 도로 ID를 기준으로 Hash에서 출발도시 탐색
    int sCity = Hash[mId];

    // Adjacent Node에서 지우기
    for (int i = 0; i < Graph[sCity].size(); ++i) {
        if (mId == Graph[sCity][i].id) {
            Graph[sCity].erase(Graph[sCity].begin() + i);
            return;
        }
    }
}

// B: 배터리 최대 용량
// sCity: 출발도시 eCity: 도착도시 M: 바이러스 개수 mCity: 바이러스 전염 시작도시 mTime: 바이러스 전염 시작시간
int cost(int B, int sCity, int eCity, int M, int mCity[], int mTime[]) {

    // 바이러스에 대한 Dijakstra

    // 최단경로를 저장하기 위한 저장소 (크기: 최대 도시 개수)
    int virusDist[MAX_N];
    // 초기화 (memset은 바이트 단위로 값을 채우는 함수, 따라서 0x7f 즉 0b0111 1111을 넣는 건 매우 큰 값으로 초기화하는 것)
    memset(virusDist, 0x7f, sizeof(virusDist));

    // Min-Heap
    priority_queue<int, vector<int>, greater<int>> virusPq;

    for (int i = 0; i < M; ++i) {
        // virus는 항상 Time = 0부터 시작하는 것이 아니기 때문에 mTime[i]를 입력
        virusDist[mCity[i]] = mTime[i];
        // 바이러스 전염 시작 시간 | 바이러스 전염 시작 도시를 Packing해서 virusPq에 emplace
        // 이렇게 하면 Byte를 한번에 전체 비교할 수 있기 때문에 priority_queue처럼 지속적인 비교가 필요할 때 훨씬 유리
        virusPq.emplace(mTime[i] << 16 | mCity[i]);
    }

    while (!virusPq.empty()) {
        // Parse virusPq
        // 경과한 시간
        int time = virusPq.top() >> 16;
        // 도시
        int u = virusPq.top() & 0xffff;
        virusPq.pop();

        // 이미 더 짧은 시간에 도달할 수 있는 경로가 탐색되었다면 continue
        if (virusDist[u] < time) continue;

        // 아니라면 Adjacent List에서 더 짧은 시간에 가능한지 새로 탐색
        for (auto val : Graph[u]) {
            // 근접 도시를 가는 데 걸리는 시간
            int v = val.to;
            // 현재까지의 시간 + 근접 도시를 가는 데 걸리는 시간
            int nextTime = time + val.time;

            // 만약 이 합이 기존에 저장해놓은 시간보다 짧다면 Update하고 virusPq에 추가
            if (virusDist[v] > nextTime) {
                virusDist[v] = nextTime;
                virusPq.emplace(nextTime << 16 | v);
            }
        }
    }

    // 전기차에 대한 Dijakstra
    
    // 최대 도시 개수 & 주어질 수 있는 배터리 최대 용량의 dist 2D Array 선언
    // MAX_B가 아닌 MAX_B + 1인 이유는 배터리 값이 1~300이기 때문
    int dist[MAX_N][MAX_B + 1];
    // 초기화 (memset은 바이트 단위로 값을 채우는 함수, 따라서 0x7f 즉 0b0111 1111을 넣는 건 매우 큰 값으로 초기화하는 것)
    memset(dist, 0x7f, sizeof(dist));

    // Min-Heap
    priority_queue<ull, vector<ull>, greater<ull>> carPq;

    // 전기차는 항상 Time = 0부터 시작하기 때문에 초기값을 0으로 설정
    dist[sCity][B] = 0;
    // 0의 Time | 시작도시 | 배터리 정보 넣기를 Packing해서 carPq에 emplace
    // 이렇게 하면 Byte를 한번에 전체 비교할 수 있기 때문에 priority_queue처럼 지속적인 비교가 필요할 때 훨씬 유리
    carPq.emplace(sCity << 16 | B);

    while (!carPq.empty()) {
        // Parse carPq
        // 경과한 시간
        int time = carPq.top() >> 32;
        // 도시
        int u = (carPq.top() >> 16) & 0xffff;
        // 배터리 잔량
        int battery = carPq.top() & 0xffff;
        carPq.pop();

        // 이미 더 짧은 시간에 도달할 수 있는 경로가 탐색되었다면 continue
        if (dist[u][battery] < time) continue;

        // 도착했다면 time을 return
        if (u == eCity) return time;

        // 아니라면 Adjacent List에서 더 짧은 시간에 가능한지 새로 탐색
        for (auto val : Graph[u]) {
            // 전력소모량 대비 배터리가 남아있는 경우 if문 진입
            if (battery >= val.power) {
                // 근접 도시를 가는 데 걸리는 시간
                int v = val.to;
                // 현재까지의 시간 + 근접 도시를 가는 데 걸리는 시간
                ull nextTime = time + val.time;

                // 만약 이 합이 기존에 저장해놓은 시간보다 길다면 가볼 필요가 없으니까 continue
                if (virusDist[v] <= nextTime) continue;

                // 배터리 소모량 반영
                int nextBattery = battery - val.power;

                if (dist[v][nextBattery] > nextTime) {
                    // 어떤 도시를 v라는 시간에 nextBattery를 가지고 방문할 수 있다면
                    // 그보다 낮은 nextBattery를 가지고 방문하는 경우의 수는 다 지우는 것
                    // 어차피 의미가 없기에
                    for (int i = nextBattery; i >= 0; --i) {
                        if (dist[v][i] < nextTime) break;
                        dist[v][i] = nextTime;
                    }

                    // carPq에 추가
                    carPq.emplace(nextTime << 32 | v << 16 | nextBattery);
                }
            }
        }

        if (battery < B) {
            // 충전을 하면 1시간이 흐르기 때문에 +1
            ull nextTime = time + 1;

            // 기존의 가장 짧은 시간 대비 충전 후 시간이 아직 짧을 경우에만 고려
            if (virusDist[u] > nextTime) {
                int nextBattery = battery + Charge[u];

                // 배터리 최대 용량까지만 충전 (시간당 충전량이 도시별로 다르기 때문에)
                if (nextBattery > B) nextBattery = B;

                if (dist[u][nextBattery] > nextTime) {
                    // 어떤 도시를 v라는 시간에 nextBattery를 가지고 방문할 수 있다면
                    // 그보다 낮은 nextBattery를 가지고 방문하는 경우의 수는 다 지우는 것
                    // 어차피 의미가 없기에
                    for (int i = nextBattery; i >= 0; --i) {
                        if (dist[u][i] < nextTime) break;
                        dist[u][i] = nextTime;
                    }

                    // carPq에 추가
                    carPq.emplace(nextTime << 32 | u << 16 | nextBattery);
                }
            }
        }
    }

    return -1;
}
