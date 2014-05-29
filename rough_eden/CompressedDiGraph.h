//
//  CompressedDiGraph.h
//  rough_eden
//
//  Created by Alexander G Anderson on 4/11/14.
//  Copyright (c) 2014 Alexander G Anderson. All rights reserved.
//

#ifndef __rough_eden__CompressedDiGraph__
#define __rough_eden__CompressedDiGraph__

#include <iostream>
#include <vector>
#include <queue>
#include "CellGrid.h"

using std::vector;
using std::cout;
using std::endl;


/*
 Compressed DiGraph
 
 
 
 
 */



class CompressedDiGraph {
public:
    CellGrid * cg;
    int s; // Accepts nodes with ID's in range [0, s - 1]
    int MAX_N; // Maximum number of neighbors for each cell
    vector<int> roots; // List of root nodes
    bool only_subtree; // Option: True if only compute subtree sizes
    int * p; // Array of parents
    char * c; // Array of Children
    int * n_s; // Array of Subtree sizes
    int * n_p; // Array of number of Parents
    int * c_b; // Array of number of cells in subtree that are in contact with the front of the colony
    
    
    
    CompressedDiGraph(int s, int MAX_N, CellGrid * cg);
    void clear();
    void add_root(int i); // Adds i to list of roots
    void add_edge(int i, int j); // Adds an edge from i to j (throws no error if fails)
    void add_boundary_leaf(int i); // Denotes i as a node on the boundary
    void children(int i, vector<int>& ch); // creates an array containing the ids of the children of node i
    
    
    void compute(); // Computes p,s,cb
    void compute_p(); // Computes number of parents for each node
    void compute_s(); // Computes subtree sizes for each node
    void compute_cb(); // Computes number of nodes in subtree in contact with boundary
    void print();
};



#endif /* defined(__rough_eden__CompressedDiGraph__) */
