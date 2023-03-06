#include "aStarSearch.h"
#include "puzzle.h"
#include "problem.h"
#include <vector>
#include <iostream>

using namespace std;

int main(int argc, char** argv){
    vector<vector<int>>example = {{1,2,3},
                                 {5,0,6},
                                 {4,7,8}};
    vector<vector<int>> goal = {{1,2,3},
                                {4,5,6},
                                {7,8,0}};
    problem * demo = new puzzle(example,goal);
    aStarSearch search(demo);
    search.generalSearch();
}