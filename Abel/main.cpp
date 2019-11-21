#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <algorithm>
#include <stack>
#include <iomanip>
#include <cmath>
#include <limits>
#include <chrono>
#include <windows.h>
#include <psapi.h>
#include "HanoiNode.h"

bool compareHanoiNodes(HanoiNode*, HanoiNode*);
void generateRoot(std::deque <HanoiNode *>&, int, std::string);
HanoiNode * generateChild(HanoiNode *, bool, int);
void heuristic(HanoiNode*, int, std::string&);
bool recursiveBestFirstSearch(HanoiNode *BESTNODE, int f_limit, int n, std::string &student);
bool RBFS(HanoiNode *BESTNODE, int f_limit, int n, std::string &student);
void reportGoalFound(unsigned int, unsigned int);

int main() {

    std::deque <HanoiNode *> tree;
    std::deque <HanoiNode *> OPEN, CLOSE;
    HanoiNode *BESTNODE = new HanoiNode();
    HanoiNode *OLD = new HanoiNode();
    int n = 0, numOfChildren = 0;
    bool inOPEN = false, inCLOSE = false, nEven;
    n%2 ? nEven = false : nEven = true;
    std::string student, algorithm;
    PROCESS_MEMORY_COUNTERS_EX pmc;
    SIZE_T memory_start, memory_stop, algorithmMemoryUsage;

    // User options for h(n) function and algorithm
//    std::cout << "Enter the number of disks:" << std::endl;
//    std::cin >> n;
    std::cout << "Which h(n) would you like to see? (h1, h2, h3)" << std::endl;
    std::cin >> student;
    std::cout << "Which algorithm should the program use? (A* or RBFS)" << std::endl;
    std::cin >> algorithm;

    auto time_limit = std::chrono::minutes(30);
    auto total_time = std::chrono::nanoseconds(0);

    while(total_time < std::chrono::duration_cast<std::chrono::microseconds>(time_limit)) {

        GetProcessMemoryInfo(GetCurrentProcess(), reinterpret_cast<PPROCESS_MEMORY_COUNTERS>(&pmc), sizeof(pmc));
        memory_start = pmc.WorkingSetSize;

        static int n = 3;

        auto start = std::chrono::high_resolution_clock::now();
        auto stop = std::chrono::high_resolution_clock::now();

        // Create initial node

        if (algorithm == "A*") {

            start = std::chrono::high_resolution_clock::now();

            generateRoot(tree, n, student);

            // Place root into OPEN
            OPEN.push_front(tree.front());

            while (true) {

                // Sort OPEN, then pop BESTNODE out of OPEN and into CLOSE
                std::sort(OPEN.begin(), OPEN.end(), compareHanoiNodes);
                for (int i = 0; i < OPEN.size(); ++i) {
                    inOPEN = false;
                    inCLOSE = false;
//                    std::cout << "\nOPEN at " << i << std::endl;
//                    std::cout << "====================================" << std::endl;
//                    //OPEN[i]->print();
//                    std::cout << "====================================" << std::endl;

                }

                // Pop OPEN node and call it BESTNODE; Then place BESTNODE into CLOSE
                BESTNODE = OPEN.front();
                OPEN.pop_front();
                CLOSE.push_back(BESTNODE);
//                std::cout << std::setw(19) << "BESTNODE" << std::endl;
//                BESTNODE->print();

                // See if BESTNODE is a goal node
                if (BESTNODE->isGoalNode()) {
                    reportGoalFound(CLOSE.size() - 1, tree.size());
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
//                        std::cout << "\n" << std::setw(18) << "CHILD " << i + 1 << "\n" << std::endl;
                        tree.push_back(generateChild(BESTNODE, nEven, i + 1));
                        HanoiNode *child = tree.back();
                        heuristic(child, n, student);
                        child->setF();
//                        child->print();

                        // Check if child generated is already inside OPEN
                        for (int j = 0; j < OPEN.size(); ++j) {
                            if (child->isEqual(OPEN[j])) {
//                                std::cout << "FOUND IN OPEN" << std::endl;
                                OLD = OPEN[j];
                                // Check if g value for newly generated child is better than it's duplicate
                                if (!OLD->getParent()) {} // If root, do nothing
                                else if (OLD->getG() > child->getG()) {
                                    // Replace OLD with lower-cost node
//                                    std::cout << "NEW VALUE BETTER THAN OLD VALUE - REPLACE OLD WITH NEW" << std::endl;
                                    OLD->setParent(BESTNODE);
                                    OLD->setG(child->getG() - 1);
                                    OLD->setH(child->getH());
                                    OLD->setF();
                                } else {
//                                    std::cout << "OLD VALUE BETTER THAN NEW VALUE - DO NOTHING" << std::endl;
                                }
                                inOPEN = true;
                                break;
                            }
                        }

                        // If child was not in OPEN, see if it is in CLOSE
                        if (!inOPEN) {
                            for (int k = 0; k < CLOSE.size(); ++k) {
                                if (child->isEqual(CLOSE[k])) {
//                                    std::cout << "FOUND IN CLOSE" << std::endl;
                                    OLD = CLOSE[k];
//                                    BESTNODE->setChild(OLD);
                                    // Check if g value for newly generated child is better than it's duplicate
                                    if (!OLD->getParent()) {} // If root, do nothing
                                    else if (OLD->getG() > child->getG()) {
                                        // Delete OLD from CLOSE and place new child in OPEN
//                                        std::cout << "NEW VALUE BETTER THAN OLD VALUE - DELETE OLD AND ADD NEW TO OPEN"
//                                                  << std::endl;
                                        CLOSE.erase(CLOSE.begin() + i);
                                        OPEN.push_back(child);
                                    } else {
//                                        std::cout << "OLD VALUE BETTER THAN NEW VALUE - DO NOTHING" << std::endl;
                                    }
                                    inCLOSE = true;
                                    break;
                                }

                            }
                        }
                        // If child was not in either already, add to OPEN
                        if (!inOPEN && !inCLOSE) {
                            OPEN.push_front(child);
                        }

                    }
                }
            }

             stop = std::chrono::high_resolution_clock::now();

        } else if (algorithm == "RBFS") {

            start = std::chrono::high_resolution_clock::now();

            generateRoot(tree, n, student);

            BESTNODE = tree.front();
            int f_limit = std::numeric_limits<int>::max(); // "infinity" for this example
            recursiveBestFirstSearch(BESTNODE, f_limit, n, student);

            stop = std::chrono::high_resolution_clock::now();

        } else {
            // User input validation
        }

        GetProcessMemoryInfo(GetCurrentProcess(), reinterpret_cast<PPROCESS_MEMORY_COUNTERS>(&pmc), sizeof(pmc));
        memory_stop = pmc.WorkingSetSize;

        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
        total_time += duration;
        algorithmMemoryUsage = (memory_stop - memory_start);

        OPEN.clear();
        CLOSE.clear();
        tree.clear();
        OPEN.shrink_to_fit();
        CLOSE.shrink_to_fit();
        tree.shrink_to_fit();

        std::cout << "Time taken with " << n << " disks: "
                  << duration.count() << " nanoseconds" << std::endl;

        ++n;

        std::cout << "Total time so far: "
                  << std::chrono::duration_cast<std::chrono::seconds>(total_time).count()
                  << " seconds" << std::endl;

        std::cout << "Memory usage for run: "
                  << algorithmMemoryUsage
                  << std::endl;

    }
}


bool compareHanoiNodes(HanoiNode* node1, HanoiNode* node2){
    return (node1->getF() < node2->getF());
}

void generateRoot(std::deque <HanoiNode *> &tree, int n, std::string s){
    HanoiNode *root = new HanoiNode();
    root->setParent(nullptr);
    root->setIllegalMoveTower(TowerName(0%3));
    root->setG(-1);

    // Push n disks to tower A
    for(int i=n;i>0;--i){
        root->getTower(A).pushDisk(i);
    }
    // Set h(n) value and add to tree
    heuristic(root, n, s);
    root->setF();
    tree.push_back(root);
}

HanoiNode * generateChild(HanoiNode * node, bool nEven, int i){
    int g = node->getG(), disk;
    // 1 illegal move tower, 2 legal move towers
    TowerName IMT = node->calculateIllegalMoveTower(g + 1, nEven), LMT1, LMT2;

    // Decide what legal moves can be made
    switch(IMT){
        case A: LMT1 = B; LMT2 = C; break;
        case B: LMT1 = A; LMT2 = C; break;
        default:LMT1 = A; LMT2 = B;
    }

    // Give new child node duplicate towers to manipulate
    HanoiNode *child = new HanoiNode();

    child->setParent(node);
    child->setIllegalMoveTower(IMT);
    child->setG(node->getG());

    child->setTower(A, node->getTower(A).getDisks());
    child->setTower(B, node->getTower(B).getDisks());
    child->setTower(C, node->getTower(C).getDisks());

    if(!node->getTower(B).getTotal() && !node->getTower(C).getTotal() && i == 1)
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
    return child;
}

void heuristic(HanoiNode *node, int n, std::string &s){

    int optimalMoves = pow(2, n) - 1;
    int normalize = optimalMoves - node->getG();
    int h = normalize;

    if(s == "h1") {
        // The Illegal Move Change Heuristic
        TowerName IMT = node->getIllegalMoveTower();
        int illegalChangeInt = 0;

        if(node->getParent())
            if(node->getTower(IMT).getDisks() != node->getParent()->getTower(IMT).getDisks())
                illegalChangeInt = 2;

        h += illegalChangeInt;

    }
    else if(s == "h2"){
        // The Duplicate Factor Heuristic
        HanoiNode *p = node->getParent();
        int duplicateFactor = 0;

        while(p){
            if(p->isEqual(node))
                ++duplicateFactor;
            p = p->getParent();
        }

        h += duplicateFactor;
        delete p;

    }
    else if(s == "h3") {
        // The Initial State Avoidance Heuristic
        if (!node->getTower(B).getTotal() && !node->getTower(C).getTotal())
            h = optimalMoves;
    }

    node->setH(h);

}

bool recursiveBestFirstSearch(HanoiNode *BESTNODE, int f_limit, int n, std::string &student){
    return RBFS(BESTNODE, f_limit, n, student);
}

bool RBFS(HanoiNode *BESTNODE, int f_limit, int n, std::string &student){


    static std::deque<HanoiNode *> children;
    children.clear();
    children.shrink_to_fit();

    static int numOfChildren, alternative;
    static bool nEven, result;
    result = false;
    n%2 ? nEven = false : nEven = true;
    static int expansions = 0;
    static int generated = 1;

    if (BESTNODE->isGoalNode()) {
        reportGoalFound(expansions, generated);
        expansions = 0;
        generated = 1;
        return true; // Solution found
    }

        // Generate children of BESTNODE
        // If BESTNODE is root or root duplicate, only 2 children nodes will be generated
        if (!BESTNODE->getTower(B).getTotal() && !BESTNODE->getTower(C).getTotal())
            numOfChildren = 2;
        else
            numOfChildren = 3;

        generated += numOfChildren;

        // Generate children and sort based on f values
        for (int i = 0; i < numOfChildren; ++i) {

            children.push_back(generateChild(BESTNODE, nEven, i + 1));
            heuristic(children[i], n, student);


            if((children[i]->getG() + children[i]->getH()) < BESTNODE->getF()) {
                children[i]->setG(BESTNODE->getG() - 1);
                children[i]->setH(BESTNODE->getH());
            }

            children[i]->setF();

        }




        // Sort list of children based on f(n) values
        std::sort(children.begin(), children.end(), compareHanoiNodes);

        while(!result) {

            // If f(n) exceeds f_limit, then return failure
            if (children[0]->getF() > f_limit) {

                delete children[0];
                delete children[1];
                delete children[2];

                children.clear();
                children.shrink_to_fit();
                return false;

            }

                ++expansions;
                alternative = children[1]->getF();
                if (alternative < f_limit) f_limit = alternative;
                result = RBFS(children[0], f_limit, n, student);

        }

        return result;

}

void reportGoalFound(unsigned int expanded, unsigned int generated){
    std::cout << "Goal node reached!" << std::endl;
    std::cout << "Nodes expanded: " << expanded << std::endl;
    std::cout << "Nodes generated: " << generated << std::endl; // Contains all nodes generated
}

