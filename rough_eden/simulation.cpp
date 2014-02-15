//
//  simulation.cpp
//  fast_eden
//
//  Created by Alexander G Anderson on 12/3/13.
//  Copyright (c) 2013 Alexander G Anderson. All rights reserved.
//

#include "simulation.h"

int simulation::run() {
    int m = 0;
    
    while(!q.isEmpty() && m <= MAX_CELLS) {
        ++m;
        grow(false);
        
        if (linear_burning_in and cg.maxy - cg.miny > std::floor(std::sqrt(L_X))) {
            linear_burning_in = false;
            grow(true);
            ++m;
        }
        
        
        if (!linear_burning_in and q.rq1.getSize() * q.rq0.getSize() == 0)
            break;
    }
    //print_cells();
    
    //save_grid(0);
    int winner = -1;
    if (q.rq0.getSize() == 0)
        winner = 1;
    else if (q.rq1.getSize() == 0)
        winner = 0;

    //cout << "The total number of mutants is " << cg.get_mut_tot() <<endl;
    //cout << "The winner is " << winner << endl;
    
    
    //return cg.get_mut_tot();
    return winner;
};



void simulation::print_cells() {
    cg.print();
}

void simulation::save_grid(int i) {
    cg.save_grid(i);
}

int simulation::get_mut_num() {
    return cg.get_mut_tot();
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
    MAX_CELLS = L_X * L_Y * 100;
}

/*void simulation::initialize1() {
    int L = 4;
    if (L >= L_X-2)
        cout << "ERROR: Width too small to fit mutants" << endl;
    for (int i = 0; i < L_X; ++i) {
        cg.set(make_pair(i, 0), cg.WT);
        if (i < (L_X/2- L/2) or i > (L_X/2 + L/2))
            add_cell(make_pair(i,1), cg.WT);
        else
            add_cell(make_pair(i,1), cg.MT);
    }
    cg.set_linear_growth(true);
}*/

void simulation::initialize() {
    for (int i = 0; i < L_X; ++i) {
        cg.set(make_pair(i, 0), cg.WT);
        add_cell(make_pair(i,1), cg.WT);
    }
    cg.set_linear_growth(true);
    linear_burning_in = true;
}

void simulation::initialize_circular() {
    cg.set(make_pair(L_X / 2, L_Y / 2), cg.WT);
    cg.set_linear_growth(false);
    
}


void simulation::grow(bool inserting_mutant) {
    loc l = q.pop();
    // choose a neighbor randomly to divide to
    vector<loc> em_n;
    cg.em_neighbors(l, em_n);
    
    if (em_n.size() > 1)
        q.insert(l, cg.get(l));
    
    if (inserting_mutant)
        add_cell(em_n[rand() % em_n.size()], cg.MT);
    else
        add_cell(em_n[rand() % em_n.size()], cg.get(l));

    
}


void simulation::add_cell(loc l, char type) {
    if (cg.get(l) != cg.EM)
        cout << "Tried to add a cell to an already occupied space" << endl;
    if (type == cg.EM)
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
    if (cg.linear_growth)
        cg.remove_dead_cells();
}

void simulation::remove_cell(loc l) {
    if (cg.get(l) == cg.EM)
        cout << "Tried to remove a cell from an empty spot" << endl;
    
    cg.set_empty(l);
    q.remove(l);
    
    // Checks to see if any cells are now part of the boundary
    vector<loc> n;
    cg.neighbors(l, n);
    for (vector<loc>::iterator i = n.begin(); i != n.end(); ++i) {
        if (on_boundary(*i)) {
            q.insert(*i, cg.get(*i));
        }
    }
}

bool simulation::on_boundary(loc l) {
    if(cg.get(l) == cg.EM) {
        return false;
    }
    
    vector<loc> n;
    cg.neighbors(l, n);
    for (vector<loc>::iterator i = n.begin(); i != n.end(); ++i) {
        if (cg.get(*i) == cg.EM)
            return true;
    }
    return false;
}


int simulation::relax() {
    // take a cell at the edge
    loc l = q.pop();
    
    vector<loc> em_n;
    cg.em_neighbors(l, em_n);
    
    int unfilled_neighbor_threshold = 4;
    
    if(em_n.size() < unfilled_neighbor_threshold or em_n.size() == 8) {
        q.insert(l, cg.get(l));
        return 1;
    }
    
    
    // Get a neighbor to help flatten the front
    loc l1 = cg.flatten_neighbor(l);
    
    
    // Move the cells (simulation commands update boundary, cell grid)
    char type = cg.get(l);
    add_cell(l1, type);
    remove_cell(l);
    return 0;
    
}
