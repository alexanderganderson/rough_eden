//
//  Simulation.h
//  fast_eden
//
//  Created by Alexander G Anderson on 12/10/13.
//  Copyright (c) 2013 Alexander G Anderson. All rights reserved.
//

#ifndef __fast_eden__simulation__
#define __fast_eden__simulation__

#include <iostream>
#include "WeightedRandomQueue.h"
#include <vector>
#include "CellGrid.h"
#include <fstream>
#include "CompressedDiGraph.h"
#include "HashMapHistogram.h"
#include "math.h"
#include <algorithm>

using std::vector;
using std::pair;

/*
 Simulation
 
 Pulls together the CellGrid and Boundary (weighted random queue)
    into a simulation.
 
 
 
 
 
 */



class Simulation {
public:
    // Simulation Parameters
    int MAX_CELLS;  // Maximum number of steps of the simulation
    int L_X;        // X dimension of grid
    int L_Y;        // Y dimension of grid
    double s;       // Fitness advantage of MT
    double p;       // Mutation prob going from WT to MT
    double m0;      // Place a mutant at the m0th cell

    // Simulation Options
    bool sector_simulation;
    bool linear_fixation_simulation;
    bool print;
    
    
    
    // Data Structures
    CellGrid cg;
    WeightedRandomQueue q;
    std::ofstream * outfile;
    bool burning_in;
    
    //DiGraph dg;
    CompressedDiGraph dg;
    HashMapHistogram h;
    HashMapHistogram he; //establish
    HashMapHistogram hd; //die
    
    // Options
    
    // Methods
    Simulation(int _L_X, int _L_Y, std::ofstream & outfile, int digraph_factor, double s = 0.0); //initializes the simulation

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
    void set_s(double _s); // sets the value of s for the simulation
    void set_p(double _p);
    void set_m0(int _m0);
public:
    void add_cell(loc l, char type);
    void remove_cell(loc l);
    bool on_boundary(loc l);
    int grow(bool inserting_mutant); // return 0 if successful, return -1 if fail
    void add_boundary_leaves();
    
    
    double boundary_range();
    void boundary_polar_coords(); // iterates through boundary to get (R,theta) for each point
    
    void fix_boundary();
    
    int mutant_sector_number(); // looks at the angles for the mutant sectors
    double boundary_mutant_fraction(); // returns fraction of mutants on boundary
    
    double front_width(); // returns front width depending on if it is a linear or circular growth
    
    
};

#endif /* defined(__fast_eden__simulation__) */