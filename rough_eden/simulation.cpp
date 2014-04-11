//
//  simulation.cpp
//  fast_eden
//
//  Created by Alexander G Anderson on 12/3/13.
//  Copyright (c) 2013 Alexander G Anderson. All rights reserved.
//

#include "simulation.h"

int simulation::run() {
    int m = 1;
    //print_cells();
    while(!q.isEmpty() && m <= MAX_CELLS) {
        ++m;
        grow(false);
        
        //if (m % (L_X * L_Y) == 0)
        //    save_grid(m / (L_X * L_Y));
        
        /*
        if (linear_burning_in and cg.maxy - cg.miny > std::floor(std::sqrt(L_X)) - 10) {
            linear_burning_in = false;
            grow(true);
            ++m;
            
        }
        
        if (cg.linear_growth and !linear_burning_in and q.rq1.getSize() * q.rq0.getSize() == 0)
            break;
        */
    }
    save_grid(0);
    
    add_boundary_leaves();
    //print_cells();
    
    //save polar coords of boundary
    boundary_polar_coords();
    
    
    // If the digraph is not empty, insert tree size counts into frequency tables
    int u = 0;
    if (dg.s != 0) {
        dg.compute();
    
    
        for (int i = 0; i != dg.s; ++i) {
            if (dg.nodes[i].n_s != -1) {
                //if (dg.nodes[i].n_s != -1 and dg.nodes[i].c_b == 1) {
                h.increment(dg.nodes[i].n_s);
                if (dg.nodes[i].c_b == 0)
                    hd.increment(dg.nodes[i].n_s);
                else
                    he.increment(dg.nodes[i].n_s);
            
            
                ++u;
            }
        }
    }
    cout << "There were " << u << " tree sizes recorded." << endl;
    
    
    
    int winner = -1;
    if (q.rq0.getSize() == 0)
        winner = 1;
    else if (q.rq1.getSize() == 0)
        winner = 0;

    //cout << "The total number of mutants is " << cg.get_mut_tot() <<endl;
    //cout << "The winner is " << winner << endl;

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

simulation::simulation(int _L_X, int _L_Y, double s, std::ofstream & _outfile, int digraph_factor): cg(_L_X, _L_Y, _outfile), q(s), dg(_L_X * _L_Y * digraph_factor) {
    srand((int) time(NULL));
    L_X = _L_X; // length of array
    L_Y = _L_Y;
    outfile = &_outfile;
    print = true;
    
}

void simulation::set_s(double _s) {
    q.s = _s;
}

void simulation::clear() {
    
    //Initialize Data Structures
    cg.clear();
    q.clear();
    dg.clear();
}

void simulation::initialize_diffusive_boundary() {
    cg.set_linear_growth(true);
    
    for (int i = 0; i < L_X; ++i) {
        loc l1 = make_pair(i, 0);
        loc l2 = make_pair(i,1);
        cg.set(l1, cg.WT); // adds cell to the grid but doesn't add to queue
        if (i>L_X / 2)
            add_cell(l2, cg.WT);
        else
            add_cell(l2, cg.MT);
        dg.add_root(cg.l2id(l2));
    }
    MAX_CELLS = L_X * L_Y / 2;
}

void simulation::initialize_linear() {
    cg.set_linear_growth(true);
    linear_burning_in = true;

    for (int i = 0; i < L_X; ++i) {
        loc l1 = make_pair(i, 0);
        loc l2 = make_pair(i,1);
        cg.set(l1, cg.WT); // adds cell to the grid but doesn't add to queue
        add_cell(l2, cg.WT);
        dg.add_root(cg.l2id(l2));
    }
    
    
    MAX_CELLS = L_X * L_Y * 10;
    
    
}

void simulation::initialize_circular() {
    loc r = make_pair(L_X / 2, L_Y / 2);
    add_cell(r, cg.WT);
    cg.set_linear_growth(false);
    linear_burning_in = false;
    MAX_CELLS = L_X * L_Y * 3 / 4;
    
    dg.add_root(cg.l2id(r));
    
}

void simulation::initialize_circular_sectorpic() {
    int factor = 10;
    int lx = L_X / factor;
    int ly = L_Y / factor;
    for (int i = -lx; i != lx; ++i) {
        for (int j = -ly; j!= ly; ++j) {
            if (i * i + j * j < lx * ly) {
                loc r = make_pair(L_X / 2 + i, L_Y / 2 + j);
                int v = rand() % 5;
                if (v == 0)
                    add_cell(r, cg.MT);
                else
                    add_cell(r, cg.WT);
            }
        }
    }
    
    
    cg.set_linear_growth(false);
    linear_burning_in = false;
    MAX_CELLS = L_X * L_Y * 5 / 8;
    // initialize roots for DG?
}


void simulation::initialize_sector(int R) {
    if (R < L_X / 2)
        cout << "ERROR: Radius too small";
    
    cg.set_linear_growth(true);
    linear_burning_in = true;
    
    for (int i = 0; i < L_X; ++i) {
        loc l1 = make_pair(i, 0);
        cg.set(l1, cg.WT); // adds cell to the grid but doesn't add to queue
    }
    
    
    int x0 = L_X / 2;
    int y0 = -sqrt(R * R - L_X * L_X / 4) + 1;
    cg.origin = make_pair(x0, y0);
    
    if (R + y0 + sqrt(L_X) > L_Y)
        cout << "May not have allocated enough space in front of the sector" << endl;
    
    for (int i = 0; i < L_X; ++i) {
        for (int j = 1; j < L_Y; ++j) {
            int dsq = (i - x0) * (i - x0) + (j - y0) * (j - y0);
            if (dsq <= R * R) {
                loc l2 = make_pair(i,j);
            
                add_cell(l2, cg.WT);
                if (dsq >= R * R - 2) // TODO: This might cause problems
                    dg.add_root(cg.l2id(l2));
            }
        }
    }
    
    //cout << "Initial Configuration" << endl;
    MAX_CELLS = L_X * L_Y * 10;
    
    
    
}


void simulation::grow(bool inserting_mutant) {
    loc l = q.pop();
    // choose a neighbor randomly to divide to
    vector<loc> em_n;
    cg.em_neighbors(l, em_n);
    
    if (em_n.size() == 0)
        return;
    
    if (em_n.size() > 1)
        q.insert(l, cg.get(l));
    loc l1 = em_n[rand() % em_n.size()];
    
    dg.add_edge(cg.l2id(l), cg.l2id(l1));
    
    
    if (inserting_mutant)
        add_cell(l1, cg.MT);
    else
        add_cell(l1, cg.get(l));

    
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


void simulation::boundary_polar_coords() {
    
    // create vectors to store r,theta
    vector<double> Rs;
    vector<double> thetas;
    
    // get iterator to the locations of the boundary
    for (vector<loc>::iterator i = q.rq0.b.begin(); i != q.rq0.b.end(); ++i) {
        Rs.push_back(cg.dist(cg.origin, *i));
        thetas.push_back(cg.angle(*i));
    }
    for (vector<loc>::iterator i = q.rq1.b.begin(); i != q.rq1.b.end(); ++i) {
        Rs.push_back(cg.dist(cg.origin, *i));
        thetas.push_back(cg.angle(*i));
    }
    
    for (int i = 0; i != Rs.size(); ++i)
        (*outfile) << Rs[i] << " " << thetas[i] << endl;
    
}

void simulation::add_boundary_leaves() {
    for (vector<loc>::iterator i = q.rq0.b.begin(); i != q.rq0.b.end(); ++i) {
        dg.add_boundary_leaf(cg.l2id(*i));
    }
    for (vector<loc>::iterator i = q.rq1.b.begin(); i != q.rq1.b.end(); ++i) {
        dg.add_boundary_leaf(cg.l2id(*i));
    }
}
