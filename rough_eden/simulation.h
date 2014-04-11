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
#include <fstream>
#include "DiGraph.h"
#include "HashMapHistogram.h"

using std::vector;
using std::pair;





class simulation {
public:
    // Simulation Parameters
    int MAX_CELLS;
    int L_X;
    int L_Y;
    double s;
    

    // Data Structures
    CellGrid cg;
    WeightedRandomQueue q;
    std::ofstream * outfile;
    bool linear_burning_in;
    DiGraph dg;
    HashMapHistogram h;
    HashMapHistogram he; //establish
    HashMapHistogram hd; //die
    
    // Options
    bool print;
    
    // Methods
    simulation(int _L_X, int _L_Y, double s, std::ofstream & outfile, int digraph_factor); //initializes the simulation
    //~simulation();
    void clear(); //clears the simulation
    void initialize_linear(); //initializes the cells
    void initialize_circular(); // initialize cells for circular growth
    void initialize_circular_sectorpic(); // initialize small circle randomly to grow a dish
    void initialize_diffusive_boundary(); // initialize simulation to look at boundary
    void initialize_sector(int R); // initialize simulation for a circular growth with initial radius R
    int run(); //runs the simulation
    void print_cells(); //current row
    void save_grid(int i);
    int get_mut_num();
    void set_s(double); // sets the value of s for the simulation
private:
    void add_cell(loc l, char type);
    void remove_cell(loc l);
    bool on_boundary(loc l);
    void grow(bool inserting_mutant);
    void add_boundary_leaves();
    
    void boundary_polar_coords(); // iterates through boundary to get (R,theta) for each point
    
    // Does one local rearrangement, returns 0 if successful, returns 1 if it did nothing
    int relax();
    
};

#endif /* defined(__fast_eden__simulation__) */