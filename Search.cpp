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

priority_queue<node*,vector<node*>,compareCost> OPERATORS(node*,int);
bool GOAL_TEST(vector<vector<int>>);
void general_search(problem,int);//potentially remove function pointer in parameters
node* MAKE_NODE(vector<vector<int>>,node*,int);
priority_queue<node*,vector<node*>,compareCost> EXPAND(node*,int); //needs fixing
priority_queue<node*,vector<node*>,compareCost> QUEUEING_FUNCTION(priority_queue<node*,vector<node*>,compareCost>,priority_queue<node*,vector<node*>,compareCost>);//needs fixing
int FIND_H(node* );//to do
int FIND_GMD(node*);//to do
int FIND_GMT(node*);//to do
int FIND_GUC(node*);//to do
void print(node*);
void printState(vector<vector<int>>);
vector<vector<int>> getPuzzle();

int main(int argc, char** argv) {
    vector<vector<int>> test;
    int decision,solve;
    //vector<int>row1={1,2,3};
    //vector<int>row2={4,5,6};
    //vector<int>row3={0,7,8};
    cout<<"Enter 1 to use the given puzzle, enter 2 to input your own 8 puzzle starting state."<<endl;
    cin>>decision;
    cout<<"Enter 1 to solve using uniform cost, 2 to solve using A* with misplaced tile heuristic, and 3 to solve using A* with Manhattan Distance Heuristic"<<endl;
    cin>>solve;
    if(decision==1){
        vector<int>row1={7,1,2};
        vector<int>row2={4,8,5};
        vector<int>row3={6,3,0};
        test.push_back(row1);
        test.push_back(row2);
        test.push_back(row3);
        problem puzzle={test,&GOAL_TEST};
        general_search(puzzle,solve);
    }
    else{
        test=getPuzzle();
        problem puzzle={test,&GOAL_TEST};
        general_search(puzzle,solve);
    }
    return 0;
}

vector<vector<int>> getPuzzle(){
    vector<vector<int>>p;
    for(int i=1;i<=3;i++){
        vector<int>row;
        int x,y,z;
        cout<<"Enter the numbers for row "<<i<<endl;
        cin>>x>>y>>z;
        row.push_back(x);
        row.push_back(y);
        row.push_back(z);
        p.push_back(row);
    }
    cout<<endl;
    return p;
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
        cout<<"The next optimal state is\n";
    }
    for(int i=0; i<3; i++){
        for (int j=0;j<3;j++){
            cout<<current->STATE[i][j]<<" ";
        }
        cout<<endl;
    }
    cout<<endl;
}

int FIND_G(node* a){
    if(a->parent!=NULL)
    return a->parent->g_n+1;
    else return 0;
}

int FIND_HMD(node* a){
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

int FIND_HMT(node* a){
    int x=0;
    for(int i=0;i<3;i++){
        for (int j=0; j<3;j++){
            if(a->STATE[i][j]!=(i*3)+j+1)x++;
        }
    }
    return x;// different functions for different heuristics needed
}
int FIND_HUC(node* a){
    return 0;// different functions for different heuristics needed
}
priority_queue<node*,vector<node*>,compareCost> OPERATORS(node *a,int solve){
    priority_queue<node*,vector<node*>,compareCost> nodes;
    for (int i=0;i<3; i++){
        for (int j=0;j<3; j++){
            if(a->STATE[i][j]==0){
                if(i==0){
                    vector<vector<int>>copy=a->STATE;
                    copy[i][j]=copy[1][j];
                    copy[1][j]=0;
                    nodes.push(MAKE_NODE(copy,a,solve));
                }
                if(i==1){
                    vector<vector<int>>copy=a->STATE;
                    copy[i][j]=copy[0][j];
                    copy[0][j]=0;
                    nodes.push(MAKE_NODE(copy,a,solve));
                    copy=a->STATE;
                    copy[i][j]=copy[2][j];
                    copy[2][j]=0;
                    nodes.push(MAKE_NODE(copy,a,solve));
                }
                if(i==2){
                    vector<vector<int>>copy=a->STATE;
                    copy[i][j]=copy[1][j];
                    copy[1][j]=0;
                    nodes.push(MAKE_NODE(copy,a,solve));
                }
                if(j==0){
                    vector<vector<int>>copy=a->STATE;
                    copy[i][j]=copy[i][1];
                    copy[i][1]=0;
                    nodes.push(MAKE_NODE(copy,a,solve)); 
                }
                if(j==1){
                    vector<vector<int>>copy=a->STATE;
                    copy[i][j]=copy[i][0];
                    copy[i][0]=0;
                    nodes.push(MAKE_NODE(copy,a,solve));
                    copy=a->STATE;
                    copy[i][j]=copy[i][2];
                    copy[i][2]=0;
                    nodes.push(MAKE_NODE(copy,a,solve));
                }
                if(j==2){
                    vector<vector<int>>copy=a->STATE;
                    copy[i][j]=copy[i][1];
                    copy[i][1]=0;
                    nodes.push(MAKE_NODE(copy,a,solve));
                }
                
            }
        }
    }
    return nodes;
}

node* MAKE_NODE(vector<vector<int>> givenState,node* parent,int solve){
    struct node* n1=new node;//set costs too
    n1->STATE=givenState;
    n1->parent=parent;
    n1->g_n=FIND_G(n1);
    if(solve==1){
        n1->h_n=FIND_HUC(n1);
    }
    else if(solve==2){
        n1->h_n=FIND_HMT(n1);
    }
    else{
        n1->h_n=FIND_HMD(n1);
    }
    n1->cost=n1->h_n+n1->g_n;//change Function Call for each of the three search types
    return n1;
}

void general_search(problem puzzle,int solve){
    priority_queue<node*,vector<node*>, compareCost> nodes;
    vector<node*> history;
    nodes.push(MAKE_NODE(puzzle.INITIAL_STATE,NULL,solve));//nodes=MAKE_QUEUE(MAKE_NODE(problem.INITIAL_STATE,null));create priority queue, make node and set priority queue by cost
    //cout<<FIND_G(nodes.top());
    //cout<<nodes.top()->cost<<endl;
    while(true){
        if (nodes.empty()){cout<<"Failue";return;}
        //cout<<"in loop";
        history.push_back(nodes.top());//node=REMOVE_FRONT(nodes);// pop from priority queue
        node * n=nodes.top();
        //cout<<n->cost<<endl;
        nodes.pop();
        if(puzzle.GOAL_TEST(n->STATE)){
            cout<<"Goal! The optimal path is\n";
            print(n);
            cout<<"The number of nodes expanded was "<<history.size();
            return;
        }
        nodes=QUEUEING_FUNCTION(nodes,EXPAND(n,solve));// created function to call
        //cout<<"End of while loop";
        //print(nodes.top());
        //maybe store popped nodes in another stl structure*/
    }
}

priority_queue<node*,vector<node*>,compareCost> EXPAND(node* a, int solve ){
    //cout<<"The best state to expand with a g(n)="<<a->g_n<<" and a h(n)="<<a->h_n<<" is\n";
    //printState(a->STATE);
    priority_queue<node*,vector<node*>, compareCost> nodes=OPERATORS(a,solve);
    return nodes;   
}
void printState(vector<vector<int>> s){
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            cout<<s[i][j]<<" ";
        }
        cout<<endl;
    }
}

priority_queue<node*,vector<node*>,compareCost> QUEUEING_FUNCTION(priority_queue<node*,vector<node*>,compareCost> nodes,priority_queue<node*,vector<node*>,compareCost> a){
    while(!a.empty()){
        nodes.push(a.top());
        a.pop();
    }
    return nodes;
}