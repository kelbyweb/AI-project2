#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <algorithm>
#include <stack>
#include <iomanip>
#include <cmath>
#include <limits>
#include "HanoiNode.h"

bool compareHanoiNodes(HanoiNode*, HanoiNode*);
void generateRoot(std::deque <HanoiNode *>&, int, std::string);
void generateChild(std::deque <HanoiNode *>&, HanoiNode *, bool, int);
void heuristic(HanoiNode*, int, std::string);
HanoiNode * newNode(HanoiNode*, TowerName);
HanoiNode * RBFS(std::deque <HanoiNode *>&, HanoiNode*, int, int, std::string);
void reportGoalFound(unsigned int, unsigned int);

int main() {
    std::deque <HanoiNode *> OPEN, CLOSE, tree;
    HanoiNode *BESTNODE;
    HanoiNode *OLD;
    int n = 0, numOfChildren;
    bool inOPEN = false, inCLOSE = false, nEven;
    n%2 ? nEven = false : nEven = true;
    std::string student, algorithm;

    // User options for h(n) function and algorithm
    std::cout << "Enter the number of disks:" << std::endl;
    std::cin >> n;
    std::cout << "Whose h(n) would you like to see? (Abel, Carter, Kelby)" << std::endl;
    std::cin >> student;
    std::cout << "Which algorithm should the program use? (A* or RBFS)" << std::endl;
    std::cin >> algorithm;

    // Create initial node
    generateRoot(tree, n, student);

    if(algorithm == "A*") {

        // Place root into OPEN
        OPEN.push_front(tree.front());

        while (true) {
            // Sort OPEN, then pop BESTNODE out of OPEN and into CLOSE
            std::sort(OPEN.begin(), OPEN.end(), compareHanoiNodes);
            for (int i = 0; i < OPEN.size(); ++i) {
                inOPEN = false;
                inCLOSE = false;
                std::cout << "\nOPEN at " << i << std::endl;
                std::cout << "====================================" << std::endl;
                OPEN[i]->print();
                std::cout << "====================================" << std::endl;

            }

            // Pop OPEN node and call it BESTNODE; Then place BESTNODE into CLOSE
            BESTNODE = OPEN.front();
            OPEN.pop_front();
            CLOSE.push_back(BESTNODE);
            std::cout << std::setw(19) << "BESTNODE" << std::endl;
            BESTNODE->print();

            // See if BESTNODE is a goal node
            if (BESTNODE->isGoalNode()) {
                reportGoalFound(CLOSE.size()-1, tree.size());
                break;
            }
                // Generate children of BESTNODE
            else {
                // If BESTNODE is root, only 2 children nodes will be generated
                if (!BESTNODE->getParent())
                    numOfChildren = 2;
                else
                    numOfChildren = 3;

                for (int i = 0; i < numOfChildren; ++i) {
                    std::cout << "\n" << std::setw(18) << "CHILD " << i + 1 << "\n" << std::endl;
                    generateChild(tree, BESTNODE, nEven, i + 1);
                    HanoiNode *child = tree.back();
                    heuristic(child, n, student);
                    child->setF();
                    child->print();

                    // Check if child generated is already inside OPEN
                    for (int j = 0; j < OPEN.size(); ++j) {
                        if (child->isEqual(OPEN[j])) {
                            std::cout << "FOUND IN OPEN" << std::endl;
                            OLD = OPEN[j];
                            BESTNODE->setChild(OLD);
                            // Check if f value for newly generated child is better than it's duplicate
                            if (!OLD->getParent()) {} // If root, do nothing
                            else if (OLD->getF() > child->getF()) {
                                // Replace OLD with lower-cost node
                                std::cout << "NEW VALUE BETTER THAN OLD VALUE - REPLACE OLD WITH NEW" << std::endl;
                                OLD->setParent(BESTNODE);
                                OLD->setG(child->getG() - 1);
                                OLD->setH(child->getH());
                                OLD->setF();
                            } else {
                                std::cout << "OLD VALUE BETTER THAN NEW VALUE - DO NOTHING" << std::endl;
                            }
                            inOPEN = true;
                            break;
                        }
                    }

                    // If child was not in OPEN, see if it is in CLOSE
                    if (!inOPEN) {
                        for (int k = 0; k < CLOSE.size(); ++k) {
                            if (child->isEqual(CLOSE[k])) {
                                std::cout << "FOUND IN CLOSE" << std::endl;
                                OLD = CLOSE[k];
                                BESTNODE->setChild(OLD);
                                // Check if f value for newly generated child is better than it's duplicate
                                if (!OLD->getParent()) {} // If root, do nothing
                                else if (OLD->getF() > child->getF()) {
                                    // Delete OLD from CLOSE and place new child in OPEN
                                    std::cout << "NEW VALUE BETTER THAN OLD VALUE - DELETE OLD AND ADD NEW TO OPEN"
                                              << std::endl;
                                    CLOSE.erase(CLOSE.begin() + i);
                                    OPEN.push_back(child);
                                } else {
                                    std::cout << "OLD VALUE BETTER THAN NEW VALUE - DO NOTHING" << std::endl;
                                }
                                inCLOSE = true;
                                break;
                            }

                        }
                    }
                    // If child was not in either already, add to OPEN
                    if (!inOPEN && !inCLOSE) {
                        std::cout << "PUSHING TO OPEN" << std::endl;
                        OPEN.push_front(child);
                        BESTNODE->setChild(child);
                    }
                }
            }
        }
    }
    else if(algorithm == "RBFS"){
        HanoiNode *bestNode = tree.front();
        int f_limit = std::numeric_limits<int>::max(); // "infinity" for this example
        bestNode = RBFS(tree, tree.front(), f_limit, n, student);
        if(bestNode->isGoalNode()){
            reportGoalFound(tree.size()/3, tree.size());
        }
    }
    else{
        // User input validation
    }

}


bool compareHanoiNodes(HanoiNode* node1, HanoiNode* node2){
    return (node1->getF() < node2->getF());
}

void generateRoot(std::deque <HanoiNode *> &tree, int n, std::string s){
    HanoiNode *root = newNode(nullptr, TowerName(0%3));

    // Push n disks to tower A
    for(int i=n;i>0;--i){
        root->getTower(A).pushDisk(i);
    }
    // Set h(n) value and add to tree
    heuristic(root, n, s);
    root->setF();
    tree.push_back(root);
}

void generateChild(std::deque <HanoiNode *>& tree, HanoiNode * node, bool nEven, int i){
    int g = node->getG(), disk;
    // 1 illegal move tower, 2 legal move towers
    TowerName IMT = node->calculateIllegalMoveTower(g + 1, nEven), LMT1, LMT2;

    // Decide what legal moves can be made
    node->setIllegalMoveTower(IMT);
    switch(IMT){
        case A: LMT1 = B; LMT2 = C; break;
        case B: LMT1 = A; LMT2 = C; break;
        default:LMT1 = A; LMT2 = B;
    }

    // Give new child node duplicate towers to manipulate
    HanoiNode *child = newNode(node, IMT);
    child->setTower(A, node->getTower(A).getDisks());
    child->setTower(B, node->getTower(B).getDisks());
    child->setTower(C, node->getTower(C).getDisks());

    if(!node->getParent() && i == 1)
        if(!nEven){
            disk = child->getTower(A).popTopDisk();
            child->getTower(C).pushDisk(disk);
        }
        else{
            disk = child->getTower(A).popTopDisk();
            child->getTower(B).pushDisk(disk);
        }
    else
        switch(i){
            case 1: if(child->getTower(LMT1).getDisks().empty()){
                        disk = child->getTower(LMT2).popTopDisk();
                        child->getTower(LMT1).pushDisk(disk);
                    }
                    else if(child->getTower(LMT2).getDisks().empty()){
                        disk = child->getTower(LMT1).popTopDisk();
                        child->getTower(LMT2).pushDisk(disk);
                    }
                    else if(child->getTower(LMT1).getTopDisk() < child->getTower(LMT2).getTopDisk()){
                        disk = child->getTower(LMT1).popTopDisk();
                        child->getTower(LMT2).pushDisk(disk);
                    } else {
                        disk = child->getTower(LMT2).popTopDisk();
                        child->getTower(LMT1).pushDisk(disk);
                      }
                      break;
            case 2: if(child->getTower(LMT1).getDisks().empty()){
                        disk = child->getTower(IMT).popTopDisk();
                        child->getTower(LMT1).pushDisk(disk);
                    }
                    else if(child->getTower(IMT).getDisks().empty()){
                        disk = child->getTower(LMT1).popTopDisk();
                        child->getTower(IMT).pushDisk(disk);
                    }
                    else if(child->getTower(LMT1).getTopDisk() < child->getTower(IMT).getTopDisk()){
                        disk = child->getTower(LMT1).popTopDisk();
                        child->getTower(IMT).pushDisk(disk);
                    } else {
                        disk = child->getTower(IMT).popTopDisk();
                        child->getTower(LMT1).pushDisk(disk);
                    }
                    break;
            default: if(child->getTower(LMT2).getDisks().empty()){
                        disk = child->getTower(IMT).popTopDisk();
                        child->getTower(LMT2).pushDisk(disk);
                    }
                    else if(child->getTower(IMT).getDisks().empty()){
                        disk = child->getTower(LMT2).popTopDisk();
                        child->getTower(IMT).pushDisk(disk);
                    }
                    else if(child->getTower(LMT2).getTopDisk() < child->getTower(IMT).getTopDisk()){
                        disk = child->getTower(LMT2).popTopDisk();
                        child->getTower(IMT).pushDisk(disk);
                    } else {
                        disk = child->getTower(IMT).popTopDisk();
                        child->getTower(LMT2).pushDisk(disk);
                    }
                    break;
        }
    tree.push_back(child);
}

void heuristic(HanoiNode *node, int n, std::string s){
    int h = 0;
    bool nEven;
    n%2 ? nEven = true : nEven = false;

    if(s == "Abel") {
        HanoiNode *p = node->getParent();
        TowerName IMT = node->getIllegalMoveTower();
//        if(!nEven){
//                if(IMT == C) IMT == B;
//                else if(IMT == B) IMT == C;
//                std::deque <int> temp;
//                temp = node->getTower(B).getDisks();
//                node->setTower(B, node->getTower(C).getDisks());
//                node->setTower(C, temp);
//        }
        bool illegalTowerChange = false;
        int changeInt = 0;
        int optimalMoves = pow(2, n) - 1;
        if(!node->getG()){}
        else if(node->getTower(IMT).getTotal() != p->getTower(IMT).getTotal()) {
            changeInt = 2;
        };
        h = optimalMoves - node->getG() + changeInt;
    }
    else if(s == "Carter"){
    }
    else if(s == "Kelby"){
    }

    node->setH(h);
}

HanoiNode* newNode(HanoiNode* node, TowerName IMT) {
    HanoiNode *temp = new HanoiNode;
    temp->setParent(node);
    temp->setIllegalMoveTower(IMT);
    node ? temp->setG(node->getG()) : temp->setG(-1); // If root set g(n) to 0 - Else g(n) = parent->g(n)+1
    temp->setF();
    return temp;
}

HanoiNode * RBFS(std::deque <HanoiNode *>& tree, HanoiNode *BESTNODE, int f_limit, int n, std::string student) {

    int numOfChildren;
    bool nEven;

    n%2 ? nEven = false : nEven = true;

    if (BESTNODE->isGoalNode()) {
        BESTNODE->print();
        return BESTNODE;
    } else {

        // Generate children of BESTNODE
        if (!BESTNODE->getParent()) // If BESTNODE is root, only 2 children nodes will be generated
            numOfChildren = 2;
        else
            numOfChildren = 3;

        for (int i = 0; i < numOfChildren; ++i) {
            std::cout << "\n" << std::setw(18) << "CHILD " << i + 1 << "\n" << std::endl;
            generateChild(tree, BESTNODE, nEven, i + 1);
            HanoiNode *child = tree.back();
            heuristic(child, n, student);
            child->setF();
            child->print();
        }

        // Sort list of children based on f(n) values
        std::sort(tree.end()-(numOfChildren-1), tree.end(), compareHanoiNodes);

        // Check f limit on children generated to see if it's lower
        if(tree.front()->getF() < f_limit){
            BESTNODE = tree[tree.size() - numOfChildren]; // Make BESTNODE lowest f-cost child
            std::cout << "\nRECURSIVELY CALLING ON BEST CHILD" << std::endl;
            std::cout << "vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv" << std::endl;
            BESTNODE->print();
            std::cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^" << std::endl;
//            return RBFS(tree, BESTNODE, tree[tree.size() - (numOfChildren - 1)]->getF(), n, student);
        }
        else {
            // Change BESTNODE f(n) value to lowest child
            BESTNODE->setG(tree[tree.size() - numOfChildren]->getG() - 1);
            BESTNODE->setH(tree[tree.size() - numOfChildren]->getH());
            BESTNODE->setF();
            std::cout << "\nDELETING UNUSED CHILDREN" << std::endl;
            std::cout << "vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv" << std::endl;
            tree[tree.size() - numOfChildren]->print();
            tree[tree.size() - numOfChildren+1]->print();
            tree[tree.size() - numOfChildren+2]->print();
            std::cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^" << std::endl;
            // Delete all newly generated children from memory
            tree.erase(tree.end()-(numOfChildren-1), tree.end());
//            return BESTNODE;
        }
       return RBFS(tree, BESTNODE, tree[tree.size() - (numOfChildren - 1)]->getF(), n, student);
    }

}

void reportGoalFound(unsigned int expanded, unsigned int generated){
    std::cout << "Goal node reached!" << std::endl;
    std::cout << "Nodes expanded: " << expanded << std::endl;
    std::cout << "Nodes generated: " << generated << std::endl; // Contains all nodes generated
}