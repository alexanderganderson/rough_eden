//
//  simulation.cpp
//  fast_eden
//
//  Created by Alexander G Anderson on 12/3/13.
//  Copyright (c) 2013 Alexander G Anderson. All rights reserved.
//

#include "simulation.h"


void simulation::print_cells() {
    cg.print();
}



simulation::simulation(int _L_X, int _L_Y, double s): cg(_L_X, _L_Y), q(s) {
    srand((int) time(NULL));
    
    L_X = _L_X; // length of array
    L_Y = _L_Y;
    
    print = true;
    
}

void simulation::clear() {
    
    //Initialize Data Structures
    cg.clear();
    q.clear();
}

void simulation::initialize() {
    for (int i = 0; i < L_X; ++i) {
        cg.set(make_pair(i, 0), WT);
        if (i != L_X / 2)
            add_cell(make_pair(i,1), WT);
    }
    add_cell(make_pair(L_X / 2,1), MT);
    cg.set_linear_growth(true);
}

void simulation::initialize_circular() {
    cg.set(make_pair(L_X / 2, L_Y / 2), WT);
    cg.set_linear_growth(false);
    
}

int simulation::run() {
    int m = 0;
    while(!q.isEmpty()) {
        ++m;
        grow();
        
        if (m % 200 == 0) {
            print_cells();
            break;
        }
    }
    
    

    print_cells();
    
    relax();
    
    //q.print();
    return cg.get_mut_tot();
};


void simulation::grow() {
    loc l = q.pop();
    // choose a neighbor randomly to divide to
    vector<loc> em_n;
    cg.em_neighbors(l, em_n);
    
    if (em_n.size() > 1)
        q.insert(l, cg.get(l));
    
    add_cell(em_n[rand() % em_n.size()], cg.get(l));

    
}


void simulation::add_cell(loc l, char type) {
    if (cg.get(l) != EM)
        cout << "Tried to add a cell to an already occupied space" << endl;
    if (type == EM)
        cout << "Use another method to 'add' an empty cell" <<endl;
    
    cg.set(l, type);
    q.insert(l, type);
    
    //remove any cells that might no longer be on the boundary 
    vector<loc> n;
    cg.neighbors(l, n);
    n.push_back(l);
    for (vector<loc>::iterator i = n.begin(); i != n.end(); ++i) {
        if (!on_boundary(*i)) {
            q.remove(*i);
        }
    }
     
    
}

bool simulation::on_boundary(loc l) {
    if(cg.get(l) == EM) {
        return false;
    }
    
    vector<loc> n;
    cg.neighbors(l, n);
    for (vector<loc>::iterator i = n.begin(); i != n.end(); ++i) {
        if (cg.get(*i) == EM)
            return true;
    }
    return false;
}


void simulation::relax() {
    // take a cell at the edge
    
    // find a neighbor
    
    // look at neighbors adjacent to that neighbor
    
    // try and fill those
    
    // update the data structures given the movement...
}



//bool simulation::isBoundaryEdge(edge e) {
//
//}

vector<pair<int, int>> simulation::dirs;