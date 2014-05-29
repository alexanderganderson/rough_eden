//
//  simulation.cpp
//  fast_eden
//
//  Created by Alexander G Anderson on 12/3/13.
//  Copyright (c) 2013 Alexander G Anderson. All rights reserved.
//

#include "Simulation.h"

int Simulation::run() {
    int m = 1;
    //print_cells();
    while(!q.isEmpty() && m <= MAX_CELLS) {
        ++m;
        grow(false);
        
        
        // For circular fixation probabilities
        //if (m == m0) {
        //    grow(true);
        //    burning_in = false;
        //}
        
        
        // for sector growth
        if (sector_simulation and burning_in and front_width() > std::floor(std::sqrt(L_X)) * 0.7) {
            //cout << "Threshold met" << endl;
            int val = grow(true);
            if (val == 0) {
                burning_in = false;
                ++m;
            }
        }
        
        // for linear growth
        if (linear_fixation_simulation and burning_in and front_width() > std::floor(std::sqrt(L_X)) - 5) {
            burning_in = false;
            grow(true);
            ++m;
            
        }
        
        
        if (linear_fixation_simulation and !burning_in and q.rq0.getSize() * q.rq1.getSize() == 0)
            break;
        
        if (!burning_in and q.rq1.getSize() == 0) {
            break;
        }
    }
    
    
    add_boundary_leaves();
    
    
    // If the digraph is not empty, insert tree size counts into frequency tables
    int u = 0;
    if (dg.s != 0) {
        dg.compute();
    
    
        for (int i = 0; i != dg.s; ++i) {
            if (dg.n_s[i] != -1) {
                //if (dg.nodes[i].n_s != -1 and dg.nodes[i].c_b == 1) {
                h.increment(dg.n_s[i]);
                /*
                if (dg.c_b[i] == 0)
                    hd.increment(dg.n_s[i]);
                else
                    he.increment(dg.n_s[i]);
                */
            
                ++u;
            }
        }
        cout << "There were " << u << " tree sizes recorded." << endl;
    }
    
    /*
    // Print Number of children who are on boundary
    for (int i = 0; i != dg.s; i++) {
        (*outfile) << dg.c_b[i];
        if (i % L_X != L_X - 1)
            (*outfile) << ",";
        if (i % L_X == L_X - 1 and i > 0)
            (*outfile) << endl;
    }
    */
    
    int winner = -1;
    if (q.rq0.getSize() == 0)
        winner = 1;
    else if (q.rq1.getSize() == 0)
        winner = 0;

    //cout << "The total number of mutants is " << cg.get_mut_tot() <<endl;
    //cout << "The winner is " << winner << endl;
    
    if (linear_fixation_simulation)
        h.increment(get_mut_num());
    
    return winner;
};



void Simulation::print_cells() {
    cg.print();
}

void Simulation::save_grid(int i) {
    cg.save_grid(i);
}

int Simulation::get_mut_num() {
    return cg.get_mut_tot();
}

Simulation::Simulation(int _L_X, int _L_Y, std::ofstream & _outfile, int digraph_factor, double s): cg(_L_X, _L_Y, _outfile), q(s), dg(_L_X * _L_Y * digraph_factor, 8, &cg) {
    srand((int) time(NULL));
    L_X = _L_X; // length of array
    L_Y = _L_Y;
    outfile = &_outfile;
    print = true;
    m0 = 0;
    
}

void Simulation::set_s(double _s) {
    q.s = _s;
}

void Simulation::set_p(double _p) {
    p = _p;
}

void Simulation::set_m0(int _m0) {
    m0 = _m0;
}

void Simulation::clear() {
    // Set data structures to empty
    cg.clear();
    q.clear();
    dg.clear();
}

void Simulation::initialize_diffusive_boundary() {
    cg.set_clear_filled_rows(true);
    cg.set_linear_or_circular(true, false);
    burning_in = false; //Should be true??
    sector_simulation = false;
    linear_fixation_simulation = false;
    
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
    MAX_CELLS = L_X * L_Y * 20;
}

void Simulation::initialize_linear() {
    cg.set_clear_filled_rows(true);
    cg.set_linear_or_circular(true, false);
    
    burning_in = true;
    sector_simulation = false;
    linear_fixation_simulation = true;
    
    for (int i = 0; i < L_X; ++i) {
        loc l1 = make_pair(i, 0);
        loc l2 = make_pair(i,1);
        cg.set(l1, cg.WT); // adds cell to the grid but doesn't add to queue
        add_cell(l2, cg.WT);
        dg.add_root(cg.l2id(l2));
    }
    
    
    MAX_CELLS = L_X * L_Y * 100;
    
    
}

void Simulation::initialize_circular() {
    cg.set_clear_filled_rows(false);
    cg.set_linear_or_circular(false, true);
    
    burning_in = true;
    sector_simulation = false;
    linear_fixation_simulation = false;
    
    loc r = make_pair(L_X / 2, L_Y / 2);
    cg.origin = r;
    
    add_cell(r, cg.WT);
    
    
    MAX_CELLS = L_X * L_Y * 3 / 4;
    
    dg.add_root(cg.l2id(r));
    
}

void Simulation::initialize_circular_sectorpic() {
    cg.set_clear_filled_rows(false);
    cg.set_linear_or_circular(false, true);
    
    burning_in = false;
    sector_simulation = false;
    linear_fixation_simulation = false;
    
    int mut_frac = 2; // percent mutants = 1 / mut_frac
    
    loc r = make_pair(L_X / 2, L_Y / 2);
    cg.origin = r;
    
    int factor = 10;
    int lx = L_X / factor;
    int ly = L_Y / factor;
    for (int i = -lx; i != lx; ++i) {
        for (int j = -ly; j!= ly; ++j) {
            int dsq = i * i + j * j;
            loc r = make_pair(L_X / 2 + i, L_Y / 2 + j);
            int v = rand() % mut_frac;
            char t;
            if (v == 0)
                t = cg.MT;
            else
                t = cg.WT;
            if (dsq < lx * ly and dsq >= (lx - 2) * (ly - 2))
                add_cell(r, t);
            if (dsq < (lx - 2) * (ly - 2))
                cg.set(r, t);
            
        }
    }
    
    
    
    
    MAX_CELLS = L_X * L_Y * 5 / 8;
    // initialize roots for DG?
    
    
    fix_boundary();
}


void Simulation::initialize_sector(int R) {
    if (R < L_X / 2)
        cout << "ERROR: Radius too small";
    
    cg.set_clear_filled_rows(true);
    cg.set_linear_or_circular(false, true);
    
    burning_in = true;
    sector_simulation = true;
    linear_fixation_simulation = false;
    
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
                if (dsq <= (R - 2) * (R - 2))
                    cg.set(l2, cg.WT);
                else {
                    add_cell(l2, cg.WT);
                    dg.add_root(cg.l2id(l2));
                }
            }
        }
    }
    
    MAX_CELLS = L_X * L_Y * 100;
    
    
    
}


int Simulation::grow(bool inserting_mutant) {
    loc l = q.pop();
    // choose a neighbor randomly to divide to
    vector<loc> em_n;
    cg.em_neighbors(l, em_n);
    
    if (em_n.size() == 0)
        return -1;
    
    if (sector_simulation and inserting_mutant) {
        double ang = cg.angle(l);
        loc lmax = make_pair(0, l.second);
        double ang_max = cg.angle(lmax);
        
        // If the angle of the new location is too close to edge, undo pop, and return failure
        double angle_threshold = 0.7;
        
        if ( ang / ang_max > angle_threshold or ang / ang_max < -angle_threshold) {
            //cout << "Mutant too close to edge" << endl;
            q.insert(l, cg.get(l));
            return -1;
        }
        
    }
    
    if (em_n.size() > 1)
        q.insert(l, cg.get(l));
    loc l1 = em_n[rand() % em_n.size()];
    
    //if (inserting_mutant)
    //    cout << "Mutant placed at a radius of " << cg.dist(l1, cg.origin) << endl;
    
    dg.add_edge(cg.l2id(l), cg.l2id(l1));
    
    double a = (double) rand() / ((double) RAND_MAX + 1);
    
    if (a < p or inserting_mutant)
        add_cell(l1, cg.MT);
    else
        add_cell(l1, cg.get(l));
    

    return 0;
}


void Simulation::add_cell(loc l, char type) {
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
    
    if (cg.clear_filled_rows)
        cg.remove_dead_cells();
}

void Simulation::remove_cell(loc l) {
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

bool Simulation::on_boundary(loc l) {
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


double Simulation::boundary_range() {
    vector<double> Rs;
    // get iterator to the locations of the boundary
    for (vector<loc>::iterator i = q.rq0.b.begin(); i != q.rq0.b.end(); ++i)
        Rs.push_back(cg.dist(cg.origin, *i));
    for (vector<loc>::iterator i = q.rq1.b.begin(); i != q.rq1.b.end(); ++i)
        Rs.push_back(cg.dist(cg.origin, *i));
    
    if (Rs.size() == 0)
        return 0.0;
    
    int min = *std::min_element(Rs.begin(), Rs.end());
    int max = *std::max_element(Rs.begin(), Rs.end());
    
    return max - min;
}


void Simulation::boundary_polar_coords() {
    
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
    
    //for (int i = 0; i != Rs.size(); ++i)
    //    (*outfile) << Rs[i] << " " << thetas[i] << endl;
    for (int i = 0; i != thetas.size(); ++i)
        cout << thetas[i] << " ";
}

void Simulation::fix_boundary() {
    vector<loc> v;
    for (vector<loc>::iterator i = q.rq0.b.begin(); i != q.rq0.b.end(); ++i) {
        vector<loc> em_n;
        cg.em_neighbors(*i, em_n);
        if (em_n.size() == 0)
            v.push_back(*i);
    }
    for (vector<loc>::iterator i = q.rq1.b.begin(); i != q.rq1.b.end(); ++i) {
        vector<loc> em_n;
        cg.em_neighbors(*i, em_n);
        if (em_n.size() == 0)
            v.push_back(*i);
    }
    
    for (vector<loc>::iterator i = v.begin(); i != v.end(); ++i)
        q.remove(*i);
    
    //cout << "The number of cells that had to be removed was " << v.size() << endl;
}


int Simulation::mutant_sector_number() {
    
    vector<double> ts;
    
    for (vector<loc>::iterator i = q.rq1.b.begin(); i != q.rq1.b.end(); ++i)
        ts.push_back(cg.angle(*i));
    
    std::sort(ts.begin(), ts.end());
    
    //for (vector<double>::iterator i = ts.begin(); i != ts.end(); ++i)
    //    cout << *i << endl;
    
    vector<double> diffs;
    
    for (int i = 0; i <= ts.size() - 1; ++i)
        diffs.push_back(ts[i + 1] - ts[i]);
    
    
    diffs.push_back(ts[0] - ts[ts.size() - 1] + 2 * 3.1415926);
    
    //cout << "One difference is: " << ts[0] - ts[ts.size() - 1] + 2 * 3.1415926 << endl;
    

    std::sort(diffs.begin(), diffs.end());
    
    //cout << "Printing differences" << endl;
    int result = 0;
    for (vector<double>::iterator i = diffs.begin(); i != diffs.end(); ++i) {
        if (*i > 0.05) {
            result++;
    //        cout << *i << endl;
        }
    }
    
    //result = 2 * result;
    
    cout << "Total Sector number is " << result << endl;
    
    return result;
}

double Simulation::boundary_mutant_fraction() {
    int n_mut;
    for (vector<loc>::iterator i = q.rq0.b.begin(); i != q.rq0.b.end(); ++i) {
        if (cg.get(*i) == cg.MT)
            n_mut = n_mut + 1;
    }
    for (vector<loc>::iterator i = q.rq1.b.begin(); i != q.rq1.b.end(); ++i) {
        if (cg.get(*i) == cg.MT)
            n_mut = n_mut + 1;
    }
    if (q.getSize() == 0)
        return 0;
    else
        return n_mut / ((double) q.getSize());
}



double Simulation::front_width() {
    if (cg.isLinear == true)
        return cg.maxy - cg.miny;
    if (cg.isCircular == true)
        return boundary_range();
    return 0.0;
}


void Simulation::add_boundary_leaves() {
    for (vector<loc>::iterator i = q.rq0.b.begin(); i != q.rq0.b.end(); ++i) {
        dg.add_boundary_leaf(cg.l2id(*i));
    }
    for (vector<loc>::iterator i = q.rq1.b.begin(); i != q.rq1.b.end(); ++i) {
        dg.add_boundary_leaf(cg.l2id(*i));
    }
}
