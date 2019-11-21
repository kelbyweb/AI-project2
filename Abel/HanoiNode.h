#include <vector>
#include "Towers.h"

class HanoiNode {

private:
    HanoiNode *parent;
    HanoiTower towers[3];
    TowerName IMT; // Illegal move tower
    int f, g, h;
    bool goalNode;
public:
    HanoiNode();
    ~HanoiNode();
    TowerName calculateIllegalMoveTower(int i, bool isEven);
    TowerName getIllegalMoveTower(){ return IMT; }
    HanoiTower& getTower(TowerName name){ return towers[name]; }
    HanoiNode * getParent(){ return parent; };
    int getF(){ return f; }
    int getG(){ return g; }
    int getH(){ return h; }
    bool isGoalNode();
    bool isEqual(HanoiNode * node);
    void setTower(TowerName name, std::deque <int> &disks);
    void setParent(HanoiNode * node);
    void setIllegalMoveTower(TowerName IMT);
    void setF();
    void setG(int g);
    void setH(int h);
    void print();
};
