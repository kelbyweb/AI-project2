#include "Towers.h"
#include <deque>

HanoiTower::HanoiTower(){ total = 0; };

int HanoiTower::getTopDisk() {
    return disks[0];
}

std::deque <int>& HanoiTower::getDisks(){
    return disks;
}

void HanoiTower::setDisks(std::deque <int> &disks){
    this->disks = disks;
}


int HanoiTower::popTopDisk() {
    int temp = disks.front();
    this->disks.pop_front();
    return temp;
}

int HanoiTower::getTotal(){
    total = 0;
    if(!this->getDisks().empty()) {
        for (int i = 0; i < this->getDisks().size(); i++) {
            total += this->getDisks()[i];
        }
    }
    else
        total = 0;
    return total;
}

void HanoiTower::pushDisk(int i) { disks.push_front(i); }