#include <vector>
#include <deque>

enum TowerName {A, B, C};

class HanoiTower {
private:
    std::deque <int> disks;
    int total;
public:
    HanoiTower();
    int getTotal();
    int getTopDisk();
    std::deque <int>& getDisks();
    void setDisks(std::deque <int> &disks);
    int popTopDisk();
    void pushDisk(int i);
};
