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
#include <iostream>//for input/output
#include <queue>//for the priority queue to determine which nodes to expand
#include <utility>//for pair to check Manhattan Distance
#include <cmath>//for calculating Manhattan Distance
#include <vector>//to keep track of nodes popped off of priority queue
//#include "puzzle.hpp" to switch over to a header file eventually

using namespace std;

/*
 * 
 */
struct node{
    int cost;//total cost combining movement and heuristic
    int h_n;//heuristic value
    int g_n;//number of past moves
    vector<vector<int>> STATE;//current state of puzzle
    //priority_queue<node> (*OPERATORS)(node);
    node * parent;//pointer to parent node
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
void general_search(problem,int);
node* MAKE_NODE(vector<vector<int>>,node*,int);
priority_queue<node*,vector<node*>,compareCost> EXPAND(node*,int); 
priority_queue<node*,vector<node*>,compareCost> QUEUEING_FUNCTION(priority_queue<node*,vector<node*>,compareCost>,priority_queue<node*,vector<node*>,compareCost>);
int FIND_H(node* );
int FIND_GMD(node*);
int FIND_GMT(node*);
int FIND_GUC(node*);
void print(node*);
void printState(vector<vector<int>>);
vector<vector<int>> getPuzzle();

int main(int argc, char** argv) {
    vector<vector<int>> test;//2 d vector for starting state
    //take user input for solving method and puzzle
    int decision,solve;
    cout<<"Enter 1 to use the given puzzle, enter 2 to input your own 8 puzzle starting state."<<endl;
    cin>>decision;
    cout<<"Enter 1 to solve using uniform cost, 2 to solve using A* with misplaced tile heuristic, and 3 to solve using A* with Manhattan Distance Heuristic"<<endl;
    cin>>solve;
    if(decision==1){

        vector<int>row1={1,2,3};
        vector<int>row2={5,0,6};//depth4
        vector<int>row3={4,7,8};
        test.push_back(row1);
        test.push_back(row2);
        test.push_back(row3);
        //creating problem struct and calling search
        problem puzzle={test,&GOAL_TEST};
        general_search(puzzle,solve);
    }
    else{
        //take user input for puzzle starting state
        test=getPuzzle();
        //create problem struct and calling search
        problem puzzle={test,&GOAL_TEST};
        general_search(puzzle,solve);
    }
    return 0;
}
//get input for numbers in each row
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
//check if given state is the goal
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
//recursively call print till we get to root node, then output path from node to goal
void print (node* current){
    if (current->parent!=NULL){
        
        print(current->parent);
        cout<<"The next optimal state is\n";
    }
    cout<<"Node has a g(n) ="<<current->g_n<<" and a h(n) ="<<current->h_n<<endl;
    for(int i=0; i<3; i++){
        for (int j=0;j<3;j++){
            cout<<current->STATE[i][j]<<" ";
        }
        cout<<endl;
    }
    cout<<endl;
}
//calculate g(n) by checking parent g(n) and adding 1, if no parent set to 0
int FIND_G(node* a){
    if(a->parent!=NULL)
    return a->parent->g_n+1;
    else return 0;
}
//Manhattan Distance
int FIND_HMD(node* a){
    //create vector of pairs to check correct x and y coordinates of each piece
    vector<pair<int,int>>misplaced={{0,0},{0,1},{0,2},{1,0},{1,1},{1,2},{2,0},{2,1},{2,2}};
    int x=0;//number to keep track of cost
    for(int i=0;i<3;i++){
        for (int j=0; j<3;j++){
            if(a->STATE[i][j]!=0)//take abs value of x_state-x_goal+abs value of y_state-y_goal and add to cost
                x+=abs(misplaced[a->STATE[i][j]-1].first-i)+abs(misplaced[a->STATE[i][j]-1].second-j);
        }
    }
    return x;
}
//Misplaced Tile
int FIND_HMT(node* a){
    int x=0;//number to keep track of cost
    for(int i=0;i<3;i++){
        for (int j=0; j<3;j++){
            if(a->STATE[i][j]!=(i*3)+j+1)x++;//incrementing for each misplaced tile
        }
    }
    return x;
}
int FIND_HUC(node* a){
    return 0;// Uniform Cost hard sets h(n) to 0
}
//expanding each node and returning a priority queue of those expanded
priority_queue<node*,vector<node*>,compareCost> OPERATORS(node *a,int solve){
    priority_queue<node*,vector<node*>,compareCost> nodes;
    for (int i=0;i<3; i++){
        for (int j=0;j<3; j++){
            if(a->STATE[i][j]==0){
                //checking each of the 4 directions for possible movement
                if(i==0){
                    vector<vector<int>>copy=a->STATE;
                    copy[i][j]=copy[1][j];
                    copy[1][j]=0;
                    //if a move is possible checking to make sure it is not a repeat of the parent node
                    if(a->parent!=NULL){
                        if(copy!=a->parent->STATE)
                            nodes.push(MAKE_NODE(copy,a,solve));}
                    else
                        nodes.push(MAKE_NODE(copy,a,solve));
                }
                if(i==1){
                    vector<vector<int>>copy=a->STATE;
                    copy[i][j]=copy[0][j];
                    copy[0][j]=0;
                    //if a move is possible checking to make sure it is not a repeat of the parent node
                    if(a->parent!=NULL){
                        if(copy!=a->parent->STATE)
                            nodes.push(MAKE_NODE(copy,a,solve));}
                    else
                        nodes.push(MAKE_NODE(copy,a,solve));
                    copy=a->STATE;
                    copy[i][j]=copy[2][j];
                    copy[2][j]=0;
                    //if a move is possible checking to make sure it is not a repeat of the parent node
                    if(a->parent!=NULL){
                        if(copy!=a->parent->STATE)
                            nodes.push(MAKE_NODE(copy,a,solve));}
                    else
                        nodes.push(MAKE_NODE(copy,a,solve));
                }
                if(i==2){
                    vector<vector<int>>copy=a->STATE;
                    copy[i][j]=copy[1][j];
                    copy[1][j]=0;
                    //if a move is possible checking to make sure it is not a repeat of the parent node
                    if(a->parent!=NULL){
                        if(copy!=a->parent->STATE)
                            nodes.push(MAKE_NODE(copy,a,solve));}
                    else
                        nodes.push(MAKE_NODE(copy,a,solve));
                }
                if(j==0){
                    vector<vector<int>>copy=a->STATE;
                    copy[i][j]=copy[i][1];
                    copy[i][1]=0;
                    //if a move is possible checking to make sure it is not a repeat of the parent node
                    if(a->parent!=NULL){
                        if(copy!=a->parent->STATE)
                            nodes.push(MAKE_NODE(copy,a,solve));}
                    else
                        nodes.push(MAKE_NODE(copy,a,solve)); 
                }
                if(j==1){
                    vector<vector<int>>copy=a->STATE;
                    copy[i][j]=copy[i][0];
                    copy[i][0]=0;
                    //if a move is possible checking to make sure it is not a repeat of the parent node
                    if(a->parent!=NULL){
                        if(copy!=a->parent->STATE)
                            nodes.push(MAKE_NODE(copy,a,solve));}
                    else
                        nodes.push(MAKE_NODE(copy,a,solve));
                    copy=a->STATE;
                    copy[i][j]=copy[i][2];
                    copy[i][2]=0;
                    //if a move is possible checking to make sure it is not a repeat of the parent node
                    if(a->parent!=NULL){
                        if(copy!=a->parent->STATE)
                            nodes.push(MAKE_NODE(copy,a,solve));}
                    else
                        nodes.push(MAKE_NODE(copy,a,solve));
                }
                if(j==2){
                    vector<vector<int>>copy=a->STATE;
                    copy[i][j]=copy[i][1];
                    copy[i][1]=0;
                    //if a move is possible checking to make sure it is not a repeat of the parent node
                    if(a->parent!=NULL){
                        if(copy!=a->parent->STATE)
                            nodes.push(MAKE_NODE(copy,a,solve));}
                    else
                        nodes.push(MAKE_NODE(copy,a,solve));
                }
                
            }
        }
    }
    return nodes;
}
//Create a new node and return a pointer to it, calculate heuristic based on solve choice
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
//A* search
void general_search(problem puzzle,int solve){
    priority_queue<node*,vector<node*>, compareCost> nodes;//priority queue for nodes yet to be expanded and checked
    vector<node*> history;//vector to keep popped nodes
    int x=0;
    nodes.push(MAKE_NODE(puzzle.INITIAL_STATE,NULL,solve));//nodes=MAKE_QUEUE(MAKE_NODE(problem.INITIAL_STATE,null));create priority queue, make node and set priority queue by cost
    while(true){
        if(x<nodes.size())x=nodes.size();
        if (nodes.empty()){cout<<"Failue";return;}//if no more nodes to expand output failure and exit
        //cout<<"in loop";
        history.push_back(nodes.top());//node=REMOVE_FRONT(nodes);// pop from priority queue
        node * n=nodes.top();//assign top of stack pointer to n for testing
        nodes.pop();//pop top of stack off
        //check if n is the goal
        if(puzzle.GOAL_TEST(n->STATE)){
            cout<<"Goal! Depth is "<<n->g_n <<" The optimal path with a starting state of\n";
            //recursively print nodes on optimal path
            print(n);
            //output number of nodes expanded, subtract 1 from size of history vector as goal node was already added
            cout<<"The number of nodes expanded was "<<history.size()-1<<"\nThe largest number of nodes in the queue was "<<x;
            return;
        }
        //Expand n and add new nodes to priority queue
        nodes=QUEUEING_FUNCTION(nodes,EXPAND(n,solve));
    }
}

priority_queue<node*,vector<node*>,compareCost> EXPAND(node* a, int solve ){
    priority_queue<node*,vector<node*>, compareCost> nodes=OPERATORS(a,solve);
    return nodes;   
}

//print state for testing purposes
void printState(vector<vector<int>> s){
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            cout<<s[i][j]<<" ";
        }
        cout<<endl;
    }
}
//combine two priority queues to return as one
priority_queue<node*,vector<node*>,compareCost> QUEUEING_FUNCTION(priority_queue<node*,vector<node*>,compareCost> nodes,priority_queue<node*,vector<node*>,compareCost> a){
    while(!a.empty()){
        nodes.push(a.top());
        a.pop();
    }
    return nodes;
}