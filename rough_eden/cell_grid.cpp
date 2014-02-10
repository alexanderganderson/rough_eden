//
//  cell_grid.cpp
//  rough_eden
//
//  Created by Alexander G Anderson on 1/22/14.
//  Copyright (c) 2014 Alexander G Anderson. All rights reserved.
//

#include "cell_grid.h"

int CellGrid::mod(int i, int b) {
    if (i > 0) {
        return i % b;
    }
    else {
        return mod(i + b, b);
    }
}

int CellGrid::l2m(loc l) {
    if (linear_growth) {
        return l.first + (l.second % L_Y) * L_X;
    }
    else {
        return l.first + l.second * L_X;
    }
}

CellGrid::CellGrid(int _L_X, int _L_Y) {
    L_X = _L_X;
    L_Y = _L_Y;
    cells = new char[L_X * L_Y];
    row_tot = new int[L_Y];
    clear();
}

void CellGrid::clear() {
    for (int i = 0; i != L_X * L_Y; ++i)
        cells[i]= EM;
    for (int i = 0; i != L_Y; ++i)
        row_tot[i] = 0;
    mut_tot = 0;
}

CellGrid::~CellGrid() {
//    delete cells;
}

char CellGrid::get(loc l) {
    return cells[l2m(l)];
}

void CellGrid::set(loc l, char type) {
    cells[l2m(l)] = type;
    if (type == MT) {
        if (linear_growth)
            row_tot[l.second % L_Y] += 1; //FIXME
        else
            row_tot[l.second] += 1;
        mut_tot += 1;
    }
}


void CellGrid::neighbors(loc l, vector<loc>& neigh) {
    neigh.clear();
    int i = l.first;
    int j = l.second;
    for (int ii = -1; ii <= 1; ++ii) {
        for (int jj = -1; jj <= 1; ++jj) {
            //if (ii == 0 and jj == 0)
            if (!(ii == 0 xor jj == 0))
                continue;
            loc l_n;
            if (linear_growth)
                l_n = make_pair(mod(i + ii, L_X), j + jj);
            else
                l_n = make_pair(mod(i + ii, L_X), j + jj);
            neigh.push_back(l_n);
        }
    }
    
}

void CellGrid::em_neighbors(loc l, vector<loc>& em_n) {
    em_n.clear();
    vector<loc> n;
    neighbors(l, n);
    for (vector<loc>::iterator i = n.begin(); i != n.end(); ++i) {
        if (get(*i) == EM)
            em_n.push_back(*i);
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

void CellGrid::set_linear_growth(bool b) {
    linear_growth = b;
}

int CellGrid::get_mut_tot() {
    return mut_tot;
}