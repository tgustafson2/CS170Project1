#pragma once
#include <vector>
#include <cstdlib>
#include <iostream>
#include <queue>
#include <utility>
#include "problem.h"

using namespace std;

class aStarSearch{
    private:
    vector<problem*> history;
    priority_queue<problem*, vector<problem*>, decltype(problem::comp)> pq{problem::comp};//priority queue that contains pointers and uses lambda function from problem class
    int queueSize;
    public:
    aStarSearch(problem* start){
        pq.push(start);
        queueSize=0;
    };
    void generalSearch();
};
//implementation of A* Search expand top node and add new nodes to queue repeat process until reaching goal
void aStarSearch::generalSearch(){
    while(true){
        if(queueSize<pq.size())queueSize=pq.size();
        if(pq.empty()){cout<<"Failure";return;}
        history.push_back(pq.top());
        problem * n = pq.top();
        pq.pop();
        if(n->goalTest()){
            cout<<"Goal! Depth is "<<n->getCost() <<" The optimal path with a starting state of\n";
            //recursively print nodes on optimal path
            n->printStates();
            //output number of nodes expanded, subtract 1 from size of history vector as goal node was already added
            cout<<"The number of nodes expanded was "<<history.size()-1<<"\nThe largest number of nodes in the queue was "<<queueSize;
            return;
        }
        priority_queue<problem*, vector<problem*>, decltype(problem::comp)>temp = n->expand();
        while(!temp.empty()){
            pq.push(temp.top());
            temp.pop();
        }
    }
}