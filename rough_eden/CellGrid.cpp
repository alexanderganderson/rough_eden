//
//  cell_grid.cpp
//  rough_eden
//
//  Created by Alexander G Anderson on 1/22/14.
//  Copyright (c) 2014 Alexander G Anderson. All rights reserved.
//

#include "CellGrid.h"

int CellGrid::mod(int i, int b) {
    if (i > 0) {
        return i % b;
    }
    else {
        return mod(i + b, b);
    }
}

int CellGrid::l2m(loc l) {
    if (clear_filled_rows) {
        return l.first + (l.second % L_Y) * L_X;
    }
    else {
        return l.first + l.second * L_X;
    }
}

int CellGrid::l2id(loc l) {
    return l.first + l.second * L_X;
}

loc CellGrid::id2l(int i) {
    loc l;
    l.first = i % L_X;
    l.second = i / L_X;
    return l;
}

void CellGrid::remove_dead_cells() {
    int i1 = miny % L_Y;
    int i2 = (miny + 1) % L_Y;
    int i3 = (miny + 2) % L_Y;
    
    if (row_tot[i1] == L_X and row_tot[i2] == L_X and row_tot[i3] == L_X) {
        int mutants = 0;
        // Clear row i1
        for (int i = 0; i != L_X; ++i) {
            if (cells[i + i1 * L_X] == MT) {
                mutants += 1;
            }
            cells[i + i1 * L_X] = EM;
        }
        ++miny;
        row_tot[i1] = 0;
        //cout << "Row " << miny - 1 << " got eliminated" << endl;
        //(*outfile) << mutants << ",";
        //cout << "There were " << mutants << " in the eliminated row." << endl;
        //print();
    }
    
}

CellGrid::CellGrid(int _L_X, int _L_Y, std::ofstream & _outfile) {
    outfile = &_outfile;
    L_X = _L_X;
    L_Y = _L_Y;
    cells = new char[L_X * L_Y];
    row_tot = new int[L_Y];
    clear_filled_rows = false;
    clear();
}

void CellGrid::clear() {
    for (int i = 0; i != L_X * L_Y; ++i)
        cells[i]= EM;
    for (int i = 0; i != L_Y; ++i)
        row_tot[i] = 0;
    mut_tot = 0;
    if (clear_filled_rows) {
        miny = 0;
        maxy = 0;
    }
    origin = make_pair(0, 0);
}

char CellGrid::get(loc l) {
    return cells[l2m(l)];
}

void CellGrid::set_empty(loc l) {
    char prev = cells[l2m(l)];
    if (prev == EM)
        cout << "ERROR: tried to set an empty location to empty " << l.first << " " << l.second << endl;
    
    row_tot[l.second % L_Y] -=1;
    
    if (prev == MT)
        mut_tot -= 1;
    cells[l2m(l)] = EM;
}

void CellGrid::set(loc l, char type) {
    cells[l2m(l)] = type;
    if (l.second >= miny + L_Y - 1 and clear_filled_rows) {
        cout << "ERROR: Not enough memory allocated to fit the front" << endl;
    }
    row_tot[l.second % L_Y] += 1; //linear growth??
    if (clear_filled_rows)
        maxy = std::max(maxy, l.second);
    
    if (type == MT)
        mut_tot += 1;
}


void CellGrid::neighbors(loc l, vector<loc>& neigh) {
    neigh.clear();
    int i = l.first;
    int j = l.second;
    for (int ii = -1; ii <= 1; ++ii) {
        for (int jj = -1; jj <= 1; ++jj) {
            
            //8 neighbors
            if (ii == 0 and jj == 0)
                continue;
            
            // 4 neighbors
            //if (!(ii == 0 xor jj == 0))
            //    continue;

            // hexagonal lattice
            //if (ii == 0 and jj == 0)
            //    continue;
            //if (i%2 == 0 and ii == 1 and jj != 0)
            //    continue;
            //if (i%2 == 1 and ii == -1 and jj != 0)
            //    continue;
            
            
            
            
            loc l_n;
            if (clear_filled_rows)
                l_n = make_pair(mod(i + ii, L_X), j + jj);
            else
                l_n = make_pair(mod(i + ii, L_X), j + jj);
            neigh.push_back(l_n);
        }
    }
    
}

int CellGrid::dist_squared(loc l1, loc l2) {
    int dx = abs(l1.first - l2.first);
    dx = std::min(dx, L_X-dx);
    int dy = abs(l1.second - l2.second);
    
    return dx * dx + dy * dy;
    
}

double CellGrid::dist(loc l1, loc l2) {
    return sqrt( dist_squared(l1, l2));
}
/*
double CellGrid::angle(loc l) {
    int dx = l.first - origin.first;
    int dy = l.second - origin.second;
    if (dy == 0)
        return 0;
    else {
        double res = atan(dx / (1.0 * dy));
        if (dy < 0) {
            double pi = 3.141592653;
            res = res + pi;
        }
        return res;
    }
}
*/
void CellGrid::em_neighbors(loc l, vector<loc>& em_n) {
    em_n.clear();
    vector<loc> n;
    neighbors(l, n);
    for (vector<loc>::iterator i = n.begin(); i != n.end(); ++i) {
        if (get(*i) == EM)
            em_n.push_back(*i);
    }
}

void CellGrid::full_neighbors(loc l, vector<loc>& full_n) {
    full_n.clear();
    vector<loc> n;
    neighbors(l, n);
    for (vector<loc>::iterator i = n.begin(); i != n.end(); ++i) {
        if (get(*i) != EM)
            full_n.push_back(*i);
    }
}


void CellGrid::print() {
    for (int j = 0; j < L_Y; ++j) {
        for (int i = 0; i < L_X; ++i) {
            char id = cells[i + L_X * j];
            if (id == MT)
                cout << '1';
            else if (id == WT)
                cout << '0';
            else
                cout << ' ';
            if (i == L_X - 1)
                cout << endl;
        }
    }
    cout << endl;
}

void CellGrid::save_grid(int i) {
    std::string datapath = "/Users/alex/Dropbox/Berkeley/Hallatschek/simulation_data/";
    std::string filename = "grid" + std::to_string(i) + ".txt";
    std::ofstream outfile(datapath + filename);
    
    for (int i = 0; i < L_X * L_Y; i++) {
        outfile << (int)(cells[i]);
        if ((i + 1) % L_X != 0) {
            outfile << ",";
        }
        if ((i + 1) % L_X == 0) {
            outfile << endl;
        }
    }
    outfile.close();
}


void CellGrid::set_clear_filled_rows(bool b) {
    clear_filled_rows = b;
    miny = 0;
    maxy = 0;
}

void CellGrid::set_linear_or_circular(bool _isLinear, bool _isCircular) {
    if (_isCircular and _isLinear)
        cout << "Cannot be both linear and circular" << endl;
    isLinear = _isLinear;
    isCircular = _isCircular;
}


int CellGrid::get_mut_tot() {
    return mut_tot;
}


