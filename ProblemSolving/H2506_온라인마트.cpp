#include <set>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

struct RESULT
{
    int cnt;
    int IDs[5];
};

struct Item
{
    int id, ca, co, pr;
    set<Item*>::iterator it;

    bool alive;

    void init(int _id, int _ca, int _co, int _pr)
    {
        id = _id; ca = _ca; co = _co; pr = _pr;
        alive = true;
    }
};

int nn;
Item items[50'000];

Item* getItem()
{
    return &items[nn++];
}

struct comp
{
    bool operator() (Item* const& a, Item* const& b)
    {
        return a->pr != b->pr ? a->pr < b->pr : a->id < b->id;
    }
};

struct Bucket
{
    int ca, co;
    set<Item*, comp> itemlist;
    int base;

    void init(int _ca, int _co)
    {
        ca = _ca; co = _co;
        itemlist.clear();

        base = 0;
    }

    void add(Item* p)
    {
        p->it = itemlist.insert(p).first;
    }

    void erase(Item* p)
    {
        itemlist.erase(p->it);
    }

    void update()
    {
        auto it = itemlist.begin();
        while (it != itemlist.end() && (*it)->pr + base <= 0)
        {
            (*it)->alive = false;
            it = itemlist.erase(it);
        }
    }

    int size()
    {
        return itemlist.size();
    }
};

Bucket buc[6][6];
unordered_map<int, Item*> cdb;

void init()
{
    nn = 0;
    for (int i = 1; i <= 5; ++i)
        for (int j = 1; j <= 5; ++j)
            buc[i][j].init(i, j);

    cdb.clear();
}

int sell(int mID, int mCategory, int mCompany, int mPrice)
{
    Item* p = getItem();
    p->init(mID, mCategory, mCompany, mPrice - buc[mCategory][mCompany].base);
    buc[mCategory][mCompany].add(p);

    cdb[mID] = p;

    return buc[mCategory][mCompany].size();
}

int closeSale(int mID)
{
    Item* p = cdb[mID];

    if (!p || !p->alive) return -1;
    p->alive = false;

    buc[p->ca][p->co].erase(p);

    return p->pr + buc[p->ca][p->co].base;
}

int discount(int mCategory, int mCompany, int mAmount)
{
    buc[mCategory][mCompany].base -= mAmount;
    buc[mCategory][mCompany].update();

    return buc[mCategory][mCompany].size();
}

RESULT show(int mHow, int mCode)
{
    RESULT res;
    vector<pair<int, int>> con;

    if (mHow == 0)
    {
        for (int i = 1; i <= 5; ++i)
            for (int j = 1; j <= 5; ++j)
            {
                int c = 0;
                for (auto it = buc[i][j].itemlist.begin(); it != buc[i][j].itemlist.end() && c < 5; ++c, ++it)
                {
                    Item* p = (*it);
                    con.push_back({ p->pr + buc[i][j].base, p->id });
                }
            }
    }
    else if (mHow == 1)
    {
        for (int i = 1; i <= 5; ++i)
        {
            int c = 0;
            for (auto it = buc[mCode][i].itemlist.begin(); it != buc[mCode][i].itemlist.end() && c < 5; ++c, ++it)
            {
                Item* p = (*it);
                con.push_back({ p->pr + buc[mCode][i].base, p->id });
            }
        }
    }
    else
    {
        for (int i = 1; i <= 5; ++i)
        {
            int c = 0;
            for (auto it = buc[i][mCode].itemlist.begin(); it != buc[i][mCode].itemlist.end() && c < 5; ++c, ++it)
            {
                Item* p = (*it);
                con.push_back({ p->pr + buc[i][mCode].base, p->id });
            }
        }
    }

    sort(con.begin(), con.end());

    res.cnt = min(5, (int)con.size());
    for (int i = 0; i < res.cnt; ++i)
        res.IDs[i] = con[i].second;

    return res;
}
