//
//  cell_grid.h
//  rough_eden
//
//  Created by Alexander G Anderson on 1/22/14.
//  Copyright (c) 2014 Alexander G Anderson. All rights reserved.
//


/*
 
 We consider a grid, and loc.first corresponds to a the horizontal
    direction and loc.second gives the vertical direction. Increasing
    is going down and right. 
 
 
 */

#ifndef __rough_eden__cell_grid__
#define __rough_eden__cell_grid__

#include <iostream>
#include <vector>
#include "loc.h"
#include <fstream>
#include <math.h>

using std::cout;
using std::endl;
using std::vector;
using std::make_pair;

class CellGrid {
    private:
    public:
        //parameters
        int L_X;
        int L_Y;
        bool linear_growth;
    
        loc origin; // location for calculating an angle 
    
    
        //data structures
        std::ofstream * outfile;
        char* cells;
        int* row_tot;
        int mut_tot;
        // for linear growth
        int miny; //for an advancing front, store the min y dimension for the active cells
        int maxy; // variable to hold y-val of most advanced cell, not compatible with set_empty
    
        //private methods
        int mod(int i, int b);
        int l2m(loc l); //given a location, converts it to a memory location
        int l2id(loc l); // given a location, converts it to a unique id
        void remove_dead_cells();
    
    
    
    public:
    
    
        // returns distance between locations, given cyclic boundaries in X direction
        int dist_squared(loc l1, loc l2);
    
        // returns sqrt of dist_squared
        double dist(loc l1, loc l2);
        // returns angle between l, origin, and (0,1) direction
        double angle(loc l);
    
        //constants
        const char WT = 0; //wild type
        const char MT = 1; //mutant type
        const char EM = 2; //empty
    
        // Initializes a grid with dimensions _L_X and _L_Y
        CellGrid(int _L_X, int _L_Y, std::ofstream & _outfile);
    
        ~CellGrid();
    
        //sets all of the cells to empty and clears all counts
        void clear();
    
        // Gets the type of the cell at location l
        char get(loc l);
    
        // Sets a location to empty assuming that it wasn't empty previously
        void set_empty(loc l);
    
        // Sets the type of the cell at loc l to type
        //  where the types are WT = 0, MT = 1, EM = 2
        void set(loc l, char type);
    
        // Clears, then fills neigh with the locations of the neighbors
        //  of loc l
        void neighbors(loc l, vector<loc>& neigh);
    
        // Fills vector em_n with locations of empty neighbors of loc l
        void em_neighbors(loc l, vector<loc>& em_n);
    
        // Fills vector em_n with locations of empty neighbors of loc l
        void full_neighbors(loc l, vector<loc>& full_n);
    
        // Gets location to move loc to flatten front
        loc flatten_neighbor(loc l);
    
    
        // Prints the grid of Cells
        void print();
    
        // Saves the grid to a file
        void save_grid(int i);
    
        // Sets the mode of the graph to linear growth
        void set_linear_growth(bool isLinearGrowth);
    
        // Return the total number of mutants
        int get_mut_tot();
};



#endif /* defined(__rough_eden__cell_grid__) */
