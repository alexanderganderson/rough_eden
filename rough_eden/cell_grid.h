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

using std::cout;
using std::endl;
using std::vector;
using std::make_pair;

class CellGrid {
    private:
        //constants
        char WT = 0; //wild type
        char MT = 1; //mutant type
        char EM = 2; //empty

        //parameters
        int L_X;
        int L_Y;
        bool linear_growth;
    
        //data structures
        char* cells;
        int* row_tot;
        int mut_tot;
    
        //private methods
        int mod(int i, int b);
        int l2m(loc l); //given a location, converts it to a memory location
    
    public:
        //public
    
    
    
        CellGrid();
        CellGrid(int _L_X, int _L_Y);
        ~CellGrid();
        void clear();
        char get(loc l);
        void set(loc l, char type);
        void neighbors(loc l, vector<loc>& neigh);
        void em_neighbors(loc l, vector<loc>& em_n);
        void print();
        void set_linear_growth(bool isLinearGrowth);
        int get_mut_tot();
};


#endif /* defined(__rough_eden__cell_grid__) */
