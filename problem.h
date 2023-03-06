#pragma once
#include <vector>
#include <queue>

using namespace std;

//abstract base class for the problem A* Search is seeking to solve
class problem{
    protected:
    int cost;//total cost combining movement and heuristic
    int h_n;//heuristic value
    int g_n;//number of past moves
    const problem * parent;//pointer to parent node
    public:
    problem(){parent = nullptr;}
    problem(const problem * p){parent = p;}
    const int getCost() const{return cost;}
    const int getHeuristic() const {return h_n;}
    const int getPast() const{return g_n;}
    constexpr static auto comp = [] (problem* a, problem* b) -> bool{
            return a->getCost()>b->getCost();
    };
    //virtual functions to be implemented in concrete derived class
    virtual bool goalTest() const = 0;
    virtual void printStates() const = 0;
    virtual priority_queue<problem*, vector<problem*>, decltype(comp)> expand() const = 0;
    virtual void findHN() = 0;
};