//
//  DiGraph.h
//  rough_eden
//
//  Created by Alexander G Anderson on 2/24/14.
//  Copyright (c) 2014 Alexander G Anderson. All rights reserved.
//

#ifndef __rough_eden__DiGraph__
#define __rough_eden__DiGraph__

#include <iostream>
#include <vector>
#include <queue>

using std::vector;
using std::cout;
using std::endl;

/*
 DiGraph -- Depreciated
 
 Stores Coalescent Digraph corresponding to Eden model growth
 
 Computes Relevant quantities corresponding to that graph
 
 Each node is labeled by an integer
 
   --  Depreciated (Now use CompressedDiGraph)
        Note this code is simpler because it doesn't reference the cell grid
 */

class Node {
public:
    Node();
    void clear();
    void print();
    int p; //parent
    vector<int> c; // children
    int n_s; // number of elements in subtree rooted at node
    int n_p; // number of parents
    int c_b; // number of nodes in subtree rooted at that node with contact with the boundary, -1 if uninitialized
};

class DiGraph {
public:
    int s;
    vector<int> roots;
    Node * nodes;
    DiGraph(int s); //s = number nodes in the graph
    void clear(); // empties graph, keeps size
    void add_root(int); // adds a root
    void add_edge(int, int); //integers in [0,s)
    void add_boundary_leaf(int); // adds leaves that correspond to the colony boundary
    
    // note does not support add_leaf then add_edge
    void compute(); // calls all computes
    void compute_p(); // computes n_p for all nodes in the graph
    void compute_s(); // computes n_s for all nodes in the graph
    void compute_cb(); // computes c_b for all nodes in the graph
    void print();
    void save();
    
    
};


#endif /* defined(__rough_eden__DiGraph__) */
