//
//  cell_grid.h
//  rough_eden
//
//  Created by Alexander G Anderson on 1/22/14.
//  Copyright (c) 2014 Alexander G Anderson. All rights reserved.
//


/*  Cell Grid
 
 Stores the status of each simulation square (EMpty, WildType, MuTant)
 
 Has methods for getting neighbors of cells
 
 Maintains summary information
    -- most advanced occupied square
    -- least advanced unoccupied square
    -- number of cells
    -- number of mutants (including those in rows flushed out of memory)
    -- number of occupied cells in each row
 
 Grid Orientation: loc.first corresponds to a the horizontal
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
    public:
        //parameters
        int L_X;
        int L_Y;
        bool clear_filled_rows; // status variable: true if we should remove rows that are filled and cannot grow from memory
        bool isLinear; // status variable: true if a linear growth
        bool isCircular; // status variable: true if a circular growth
    
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
        loc id2l(int i); // given a unique id, converts it to a location
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
    
        // Destructor
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
    
        // Fills vector em_n with locations of full neighbors of loc l
        void full_neighbors(loc l, vector<loc>& full_n);
    
        // Prints the grid of Cells
        void print();
    
        // Saves the grid to the file: outfile
        void save_grid(int i);
    
        // Sets the mode of the graph to linear growth
        void set_clear_filled_rows(bool clear_filled_rows);
    
        // Sets mode to circular or linear
        void set_linear_or_circular(bool isLinear, bool isCircular);
    
        // Return the total number of mutants
        int get_mut_tot();
    

};



#endif /* defined(__rough_eden__cell_grid__) */
