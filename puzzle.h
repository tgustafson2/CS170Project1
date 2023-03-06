#pragma once
#include <vector>
#include <queue>
#include <utility>
#include <iostream>
#include <cstdlib>
#include "problem.h"

using namespace std;


//concrete derived class to solve 8 puzzle using A* search
class puzzle : public problem {
    private:
    vector<vector<int>> state;
    vector<vector<int>> goal;
    public:
    puzzle(vector<vector<int>> s, vector<vector<int>>g) : problem(){
        state = s;
        goal = g;
        findHN();
        this->cost = h_n;
        this->g_n = 0;
    }
    puzzle(vector<vector<int>> s, vector<vector<int>> g, const problem* p) : problem(p){
        state = s;
        goal = g;
        findHN();
        this->g_n = parent->getPast()+1;
        this->cost = h_n + g_n;
    }
    virtual bool goalTest() const;
    virtual priority_queue<problem* , vector<problem*>, decltype(comp)> expand() const;
    virtual void findHN();
    virtual void printStates() const;

};
//function to test if goal is met at a given node
bool puzzle::goalTest() const {
    if(this->goal == state)return true;
    else return false;
}
//find heuristic for distance to goal, in this case using manhattan distance for tiles
void puzzle::findHN() {
    //create vector of pairs to check correct x and y coordinates of each piece
    vector<pair<int,int>>misplaced;
    for(int i=0; i<goal.size(); i++){
        for(int j=0; j<goal[i].size(); j++){
            misplaced.push_back({i,j});
        }
    }
    int x=0;//number to keep track of cost
    for(int i=0;i<3;i++){
        for (int j=0; j<3;j++){
            if(state[i][j]!=0)//take abs value of x_state-x_goal+abs value of y_state-y_goal and add to cost
                x+=abs(misplaced[state[i][j]-1].first-i)+abs(misplaced[state[i][j]-1].second-j);
        }
    }
    this->h_n = x;

}
//expand node based on possible moves
priority_queue<problem* , vector<problem*>, decltype(problem::comp)> puzzle::expand() const{
    priority_queue<problem* , vector<problem*>, decltype(comp)> nodes(problem::comp);
    for(int i = 0; i<state.size(); i++){
        for(int j = 0; j<state[0].size(); j++){
            if(state[i][j]==0){
                //checking each of the 4 directions for possible movement
                if(i==0){
                    vector<vector<int>>copy=state;
                    copy[i][j]=copy[1][j];
                    copy[1][j]=0;
                    if(parent!=nullptr){
                        problem * node = new puzzle(copy,goal,this);
                        nodes.push(node);}
                    else{
                        problem * node = new puzzle(copy,goal,this);
                        nodes.push(new puzzle(copy,goal,this));
                    }
                }
                else if(i==state.size()-1){
                    vector<vector<int>>copy=state;
                    copy[i][j]=copy[state.size()-2][j];
                    copy[state.size()-2][j]=0;
                    if(parent!=nullptr){
                        problem * node = new puzzle(copy,goal,this);
                        nodes.push(node);}
                    else{
                        problem * node = new puzzle(copy,goal,this);
                        nodes.push(new puzzle(copy,goal,this));
                    }
                }
                else{
                    vector<vector<int>>copy=state;
                    copy[i][j]=copy[i-1][j];
                    copy[i-1][j]=0;
                    if(parent!=nullptr){
                        problem * node = new puzzle(copy,goal,this);
                        nodes.push(node);}
                    else{
                        problem * node = new puzzle(copy,goal,this);
                        nodes.push(new puzzle(copy,goal,this));
                    }
                    copy=state;
                    copy[i][j]=copy[i+1][j];
                    copy[i+1][j]=0;
                    if(parent!=nullptr){
                        problem * node = new puzzle(copy,goal,this);
                        nodes.push(node);}
                    else{
                        problem * node = new puzzle(copy,goal,this);
                        nodes.push(new puzzle(copy,goal,this));
                    }
                }
                if(j==0){
                    vector<vector<int>>copy=state;
                    copy[i][j]=copy[i][1];
                    copy[i][1]=0;
                    if(parent!=nullptr){
                        problem * node = new puzzle(copy,goal,this);
                        nodes.push(node);}
                    else{
                        problem * node = new puzzle(copy,goal,this);
                        nodes.push(new puzzle(copy,goal,this));
                    }
                }
                else if(j==state[i].size()-1){
                    vector<vector<int>>copy=state;
                    copy[i][j]=copy[i][state[i].size()-2];
                    copy[i][state[i].size()-2]=0;
                    if(parent!=nullptr){
                        problem * node = new puzzle(copy,goal,this);
                        nodes.push(node);}
                    else{
                        problem * node = new puzzle(copy,goal,this);
                        nodes.push(new puzzle(copy,goal,this));
                    }
                }
                else{
                    vector<vector<int>>copy=state;
                    copy[i][j]=copy[i][j-1];
                    copy[i][j-1]=0;
                    if(parent!=nullptr){
                        problem * node = new puzzle(copy,goal,this);
                        nodes.push(node);}
                    else{
                        problem * node = new puzzle(copy,goal,this);
                        nodes.push(new puzzle(copy,goal,this));
                    }
                    copy=state;
                    copy[i][j]=copy[i][j+1];
                    copy[i][j+1]=0;
                    if(parent!=nullptr){
                        problem * node = new puzzle(copy,goal,this);
                        nodes.push(node);}
                    else{
                        problem * node = new puzzle(copy,goal,this);
                        nodes.push(new puzzle(copy,goal,this));
                    }
                }
            }
        }
    }
    return nodes;
}

void puzzle::printStates() const {
    if(parent!=nullptr)parent->printStates();
    cout<<"\nState: "<<this->g_n<<endl;
    for(auto &row : state){
        for(auto &col : row){
            cout<<col<<" ";
        }
        cout<<endl;
    }

}