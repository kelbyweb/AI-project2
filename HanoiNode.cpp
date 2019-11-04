#include "HanoiNode.h"
#include <iostream>
#include <iomanip>


HanoiNode::HanoiNode() {
    parent = nullptr;
    IMT = A;
    f = 0;
    g = 0;
    h = 0;
    goalNode = false;
}


TowerName HanoiNode::calculateIllegalMoveTower(int i, bool isEven) {
    TowerName illegalMoveTower;
    switch(i%3){
        case 1: if(isEven) illegalMoveTower = C; else illegalMoveTower = B; break; // Legal move between A & B | A & C if n is even
        case 2: if(isEven) illegalMoveTower = B; else illegalMoveTower = C; break; // Legal move between A & C | A & B if n is even
        default: illegalMoveTower = A; // Legal move between B & C
    }
    return illegalMoveTower;
}

bool HanoiNode::isEqual(HanoiNode *node) {

    bool equal = false;

    if(this->getTower(A).getDisks() == node->getTower(A).getDisks()) {
        if (this->getTower(B).getDisks() == node->getTower(B).getDisks()) {
            if (this->getTower(C).getDisks() == node->getTower(C).getDisks()) {
                equal = true;
            }
        }
    }

    return equal;
}

void HanoiNode::setTower(TowerName name, std::deque <int> disks){
    towers[name].setDisks(disks);
}
void HanoiNode::setIllegalMoveTower(TowerName IMT){ this->IMT = IMT; };
void HanoiNode::setParent(HanoiNode * node){ parent = node; };
void HanoiNode::setChild(HanoiNode * node){ children.push_back(node); };
void HanoiNode::setF(){ f = g + h; };
void HanoiNode::setG(int g){ this->g = g + 1; };
void HanoiNode::setH(int h){ this->h = h; };
bool HanoiNode::isGoalNode(){
    int towerATotal = 0;
    int towerBTotal = 0;
    int towerCTotal = 0;

    if(!this->getTower(A).getDisks().empty())
        towerATotal = this->getTower(A).getTotal();
    if(!this->getTower(B).getDisks().empty())
        towerBTotal = this->getTower(B).getTotal();
    if(!this->getTower(C).getDisks().empty())
        towerCTotal = this->getTower(C).getTotal();

    goalNode = ((towerBTotal && !towerATotal && !towerCTotal) || (!towerBTotal && !towerATotal && towerCTotal));
    return goalNode;
}
void HanoiNode::print(){
    std::deque <int> towerA = this->getTower(A).getDisks();
    std::deque <int> towerB = this->getTower(B).getDisks();
    std::deque <int> towerC = this->getTower(C).getDisks();
    int largest = towerA.size();
    if(towerB.size() > largest) largest = towerB.size();
    if(towerC.size() > largest) largest = towerC.size();

    std::cout << "f(n)" << " = " << this->getF() << std::endl;
    std::cout << "g(n)" << " = " << this->getG() << std::endl;
    std::cout << "h(n)" << " = " << this->getH() << "\n" << std::endl;

    std::cout << "Tower A: " << std::setw(11);
    std::cout << "Tower B: " << std::setw(11);
    std::cout << "Tower C: " << std::endl;
    std::cout << "--------" << std::setw(11);
    std::cout << "--------" << std::setw(11);
    std::cout << "--------" << std::endl;


    for(int i=0;i<largest;++i){

        if(!towerA.empty() && towerA.size() > i)
            std::cout << std::setw(4) << towerA[i];
        else
            std::cout << std::setw(4)<< " ";

        if (!towerB.empty() && towerB.size() > i)
            std::cout << std::setw(11) << towerB[i];
        else
            std::cout << std::setw(11) << " ";

        if (!towerC.empty() && towerC.size() > i)
            std::cout << std::setw(11) << towerC[i];
        else
            std::cout << std::setw(11) << " ";

        std::cout << std::endl;
    }
}