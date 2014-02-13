//
//  simulation.h
//  fast_eden
//
//  Created by Alexander G Anderson on 12/10/13.
//  Copyright (c) 2013 Alexander G Anderson. All rights reserved.
//

#ifndef __fast_eden__simulation__
#define __fast_eden__simulation__

#include <iostream>
#include "WeightedRandomQueue.h"
//#include "random_queue.h"
#include <vector>
#include "cell_grid.h"

using std::vector;
using std::pair;



typedef pair<int,int> corner;
typedef int dir; //value of 0,1,2,3, index to vector dirs

struct edge {
    corner c;
    dir d;
};

class simulation {
public:
    // Simulation Parameters
    int L_X;
    int L_Y;
    double s;
    FILE * outfile;


    //char WT = 0; //wild type
    //char MT = 1; //mutant type
    //char EM = 2; //empty

    // Data Structures
    CellGrid cg;
    WeightedRandomQueue q;
    
    
    // Options
    bool print;
    
    // Methods
    simulation(int _L_X, int _L_Y, double s); //initializes the simulation
    //~simulation();
    void clear(); //clears the simulation
    void initialize(); //initializes the cells
    void initialize_circular(); // initialize cells for circular growth
    int run(); //runs the simulation
    void print_cells(); //current row
    void save_grid(int i);
    
private:
    void add_cell(loc l, char type);
    void remove_cell(loc l);
    bool on_boundary(loc l);
    void grow();
    
    // Does one local rearrangement, returns 0 if successful, returns 1 if it did nothing
    int relax();
    
};

#endif /* defined(__fast_eden__simulation__) */