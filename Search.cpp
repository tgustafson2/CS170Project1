/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Search.cpp
 * Author: Thomas Gustafson
 *
 * Created on October 9, 2021, 2:19 PM
 */

#include <cstdlib>
#include <iostream>
#include <queue>
#include <utility>
#include <vector>
#include "puzzle.hpp"

using namespace std;

/*
 * 
 */
struct node{
    int cost;//might need to make two variables h_n and g_n
    int h_n;
    int g_n;
    vector<vector<int>> STATE;
    //priority_queue<node> (*OPERATORS)(node);
    node * parent;
};
struct problem{
    vector<vector<int>> INITIAL_STATE;
    priority_queue<node> (*OPERATORS)(node);
    bool (*GOAL_TEST)(vector<vector<int>>);
};

node general_search(problem,int * QUEUING_FUNCTION);
node MAKE_NODE(vector<vector<int>>,node);
priority_queue<node> EXPAND(node, int *); //need comparison struct for priority queue to do
priority_queue<node> QUEUEING_FUNCTION(priority_queue<node>, int *);//to do
int FIND_H(node *);//to do
int FIND_G(node *);//to do
//to do overload greater

int main(int argc, char** argv) {

    return 0;
}

node MAKE_NODE(vector<vector<int>> givenState,node parent){
    struct node n1={0,0,0,givenState, &parent};//set costs too
    n1.h_n=FIND_H(n1);
    n1.g_n=FIND_G(n1);
    n1.cost=n1.h_n+n1.g_n;//change Function Call for each of the three search types
    return n1;
}

node general_search(problem puzzle,priority_queue<node> * QUEUEING_FUNCTION){
    priority_queue<node,vector<node>, greater<node>> nodes;
    vector<node> history;
    nodes.push(MAKE_NODE(problem.INITIAL_STATE,NULL));//nodes=MAKE_QUEUE(MAKE_NODE(problem.INITIAL_STATE,null));create priority queue, make node and set priority queue by cost
    while(true){
        if (nodes.empty())return NULL;
        history.push_back(nodes.top());//node=REMOVE_FRONT(nodes);// pop from priority queue
        nodes.pop();
        if(problem.GOAL_TEST(node.STATE))return node;
        nodes=QUEUEING_FUNCTION(nodes,EXPAND(node,problem.OPERATORS));// created function to call
        //maybe store popped nodes in another stl structure
    }
}
