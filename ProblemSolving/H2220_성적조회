#include <stdio.h>
#include <set>
#include <unordered_map>
using namespace std;
 
#define _D // printf
 
struct Info {
    int mid;
    int grade;
    int gender;
    int score;
    bool operator<(const struct Info& t) const {
        if (score != t.score) return score < t.score;
        return mid < t.mid;
    }
};
set<Info> db[3][2]; // [학년][성별]
unordered_map<int, Info> um;
 
void init() {
    _D("[INIT]\n");
    for (int i = 0; i < 3; ++i) for (int j = 0; j < 2; ++j) db[i][j].clear();
    um.clear();
}
 
int gender_to_gid(char mGender[]) {
    if (mGender[0] == 'm') return 0;
    return 1;
}
 
int add(int mId, int mGrade, char mGender[7], int mScore) {
    _D("[ADD] %d\n", mId);
    Info info = {mId, mGrade - 1, gender_to_gid(mGender), mScore};
    auto& cdb = db[mGrade - 1][gender_to_gid(mGender)];
    cdb.insert(info);
    um[mId] = info;
    return cdb.rbegin()->mid;
}
 
int remove(int mId) {
    _D("[REM] %d\n", mId);
    if (um.find(mId) == um.end()) return 0;
    auto info = um[mId];
    um.erase(mId);
    auto& cdb = db[info.grade][info.gender];
    cdb.erase(info);
    if (cdb.size() == 0) return 0;
    return cdb.begin()->mid;
}
 
int query(int mGradeCnt, int mGrade[], int mGenderCnt, char mGender[][7], int mScore) {
    _D("[QUERY] %d\n", mScore);
    Info dummy_student = { 0, 0, 0, mScore };
    set<Info> found;
    for (int i = 0; i < mGradeCnt; ++i) {
        for (int j = 0; j < mGenderCnt; ++j) {
            auto& cdb = db[mGrade[i] - 1][gender_to_gid(mGender[j])];
            auto it = cdb.lower_bound(dummy_student);
            if (it != cdb.end()) {
                found.insert(*it);
            }
        }
    }
 
    if (found.size() == 0) return 0;
    return found.begin()->mid;
}
