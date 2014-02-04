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



simulation::simulation() {
    srand((int) time(NULL));
    
    L_X = 20; // length of array
    L_Y = 15;
    s = 0.0; //mutant fitness advantage
    cg = CellGrid(L_X, L_Y);
    
    print = true;

    //dirs.push_back(make_pair(1,0));
    //dirs.push_back(make_pair(0,1));
    //dirs.push_back(make_pair(-1,0));
    //dirs.push_back(make_pair(0,-1));
    
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

int simulation::run() {
    int m = 0;
    while(!q.isEmpty()) {
        ++m;
        grow();
        
        if (m == 100)
            break;
    }
    /*
    // walk along the boundary
    loc b1 = q.pop();
    int i = b1.first;
    int j = b1.second;
    for (int ii = -1; ii <= 1; ++ii) {
        for (int jj = -1; jj <= 1; ++jj) {
            loc loc_n = make_pair(mod(i + ii, L_X), mod(j + jj, L_Y));
            if (cells[loc_n.first][loc_n.second] == EM)
                
        }
    }
    */
    
    print_cells();
    //q.print();
    return cg.get_mut_tot();
};


void simulation::grow() {
    loc l = q.pop();
    // choose a neighbor randomly to divide to
    vector<loc> em_n;
    cg.em_neighbors(l, em_n);
    
    if (em_n.size() > 1)
        q.insert(l);
    
    add_cell(em_n[rand() % em_n.size()], cg.get(l));

    
}


void simulation::add_cell(loc l, char type) {
    if (cg.get(l) != EM)
        cout << "Tried to add a cell to an already occupied space" << endl;
    if (type == EM)
        cout << "Use another method to 'add' an empty cell" <<endl;
    
    cg.set(l, type);
    q.insert(l);
    
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
    
}



//bool simulation::isBoundaryEdge(edge e) {
//
//}

vector<pair<int, int>> simulation::dirs;