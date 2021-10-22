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
#include <cmath>
#include <vector>
//#include "puzzle.hpp"

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
    //priority_queue<node,vector<node>,compareCost> (*OPERATORS)(node);
    bool (*GOAL_TEST)(vector<vector<int>>);
};
struct compareCost{
    bool operator()(node* a, node *b){
        return a->cost>b->cost;
    }
};

priority_queue<node*,vector<node*>,compareCost> OPERATORS(node*);
bool GOAL_TEST(vector<vector<int>>);
void general_search(problem);//potentially remove function pointer in parameters
node* MAKE_NODE(vector<vector<int>>,node*);
priority_queue<node*,vector<node*>,compareCost> EXPAND(node*); //needs fixing
priority_queue<node*,vector<node*>,compareCost> QUEUEING_FUNCTION(priority_queue<node*,vector<node*>,compareCost>,priority_queue<node*,vector<node*>,compareCost>);//needs fixing
int FIND_H(node* );//to do
int FIND_G(node*);//to do
void print(node*);

int main(int argc, char** argv) {
    vector<vector<int>> test;
    //vector<int>row1={1,2,3};
    //vector<int>row2={4,5,6};
    //vector<int>row3={0,7,8};
    vector<int>row1={7,1,2};
    vector<int>row2={4,8,5};
    vector<int>row3={6,3,0};
    test.push_back(row1);
    test.push_back(row2);
    test.push_back(row3);
    problem puzzle={test,&GOAL_TEST};
    general_search(puzzle);
    return 0;
}

bool GOAL_TEST(vector<vector<int>> test){
    vector<vector<int>>goal;
    vector<int>row1={1,2,3};
    vector<int>row2={4,5,6};
    vector<int>row3={7,8,0};
    goal.push_back(row1);
    goal.push_back(row2);
    goal.push_back(row3);
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            if(test[i][j]!=goal[i][j])return false;
        }
    }
    return true;
}

void print (node* current){
    if (current->parent!=NULL){
        
        print(current->parent);
    }
    for(int i=0; i<3; i++){
        for (int j=0;j<3;j++){
            cout<<current->STATE[i][j]<<" ";
        }
        cout<<endl;
    }
    cout<<endl;
}

int FIND_H(node* a){
    if(a->parent!=NULL)
    return a->parent->h_n+1;
    else return 0;
}

int FIND_G(node* a){
    vector<pair<int,int>>misplaced={{0,0},{0,1},{0,2},{1,0},{1,1},{1,2},{2,0},{2,1},{2,2}};
    int x=0;
    for(int i=0;i<3;i++){
        for (int j=0; j<3;j++){
            if(a->STATE[i][j]!=0)
            x+=abs(misplaced[a->STATE[i][j]-1].first-i)+abs(misplaced[a->STATE[i][j]-1].second-j);
        }
    }
    return x;// different functions for different heuristics needed
}

/*int FIND_G(node* a){
    int x=0;
    for(int i=0;i<3;i++){
        for (int j=0; j<3;j++){
            if(a->STATE[i][j]!=(i*3)+j+1)x++;
        }
    }
    return x;// different functions for different heuristics needed
}*/
//int FIND_G(node* a){
//    return 0;// different functions for different heuristics needed
//}
priority_queue<node*,vector<node*>,compareCost> OPERATORS(node *a){
    priority_queue<node*,vector<node*>,compareCost> nodes;
    for (int i=0;i<3; i++){
        for (int j=0;j<3; j++){
            if(a->STATE[i][j]==0){
                if(i==0){
                    vector<vector<int>>copy=a->STATE;
                    copy[i][j]=copy[1][j];
                    copy[1][j]=0;
                    nodes.push(MAKE_NODE(copy,a));
                }
                if(i==1){
                    vector<vector<int>>copy=a->STATE;
                    copy[i][j]=copy[0][j];
                    copy[0][j]=0;
                    nodes.push(MAKE_NODE(copy,a));
                    copy=a->STATE;
                    copy[i][j]=copy[2][j];
                    copy[2][j]=0;
                    nodes.push(MAKE_NODE(copy,a));
                }
                if(i==2){
                    vector<vector<int>>copy=a->STATE;
                    copy[i][j]=copy[1][j];
                    copy[1][j]=0;
                    nodes.push(MAKE_NODE(copy,a));
                }
                if(j==0){
                    vector<vector<int>>copy=a->STATE;
                    copy[i][j]=copy[i][1];
                    copy[i][1]=0;
                    nodes.push(MAKE_NODE(copy,a)); 
                }
                if(j==1){
                    vector<vector<int>>copy=a->STATE;
                    copy[i][j]=copy[i][0];
                    copy[i][0]=0;
                    nodes.push(MAKE_NODE(copy,a));
                    copy=a->STATE;
                    copy[i][j]=copy[i][2];
                    copy[i][2]=0;
                    nodes.push(MAKE_NODE(copy,a));
                }
                if(j==2){
                    vector<vector<int>>copy=a->STATE;
                    copy[i][j]=copy[i][1];
                    copy[i][1]=0;
                    nodes.push(MAKE_NODE(copy,a));
                }
                
            }
        }
    }
    return nodes;
}

node* MAKE_NODE(vector<vector<int>> givenState,node* parent){
    struct node* n1=new node;//set costs too
    n1->STATE=givenState;
    n1->parent=parent;
    n1->h_n=FIND_H(n1);
    n1->g_n=FIND_G(n1);
    n1->cost=n1->h_n+n1->g_n;//change Function Call for each of the three search types
    return n1;
}

void general_search(problem puzzle){
    priority_queue<node*,vector<node*>, compareCost> nodes;
    vector<node*> history;
    nodes.push(MAKE_NODE(puzzle.INITIAL_STATE,NULL));//nodes=MAKE_QUEUE(MAKE_NODE(problem.INITIAL_STATE,null));create priority queue, make node and set priority queue by cost
    //cout<<FIND_G(nodes.top());
    //cout<<nodes.top()->cost<<endl;
    while(true){
        if (nodes.empty()){cout<<"Failue";return;}
        //cout<<"in loop";
        history.push_back(nodes.top());//node=REMOVE_FRONT(nodes);// pop from priority queue
        node * n=nodes.top();
        //cout<<n->cost<<endl;
        nodes.pop();
        if(puzzle.GOAL_TEST(n->STATE)){print(n);cout<<history.size()<<" "<<nodes.size();return;}
        nodes=QUEUEING_FUNCTION(nodes,EXPAND(n));// created function to call
        //cout<<"End of while loop";
        //print(nodes.top());
        //maybe store popped nodes in another stl structure*/
    }
}

priority_queue<node*,vector<node*>,compareCost> EXPAND(node* a ){
    priority_queue<node*,vector<node*>, compareCost> nodes=OPERATORS(a);
    return nodes;
}

priority_queue<node*,vector<node*>,compareCost> QUEUEING_FUNCTION(priority_queue<node*,vector<node*>,compareCost> nodes,priority_queue<node*,vector<node*>,compareCost> a){
    while(!a.empty()){
        nodes.push(a.top());
        a.pop();
    }
    return nodes;
}