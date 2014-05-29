//
//  CompressedDiGraph.cpp
//  rough_eden
//
//  Created by Alexander G Anderson on 4/11/14.
//  Copyright (c) 2014 Alexander G Anderson. All rights reserved.
//

#include "CompressedDiGraph.h"


CompressedDiGraph::CompressedDiGraph(int _s, int _MAX_N, CellGrid * _cg) {
    only_subtree = true;
    s = _s;
    MAX_N = _MAX_N;
    p = new int[s];
    c = new char[s * MAX_N];
    cg = _cg;
    n_s = new int[s];
    if (!only_subtree) {
        n_p = new int[s];
        c_b = new int[s];
    }
    clear();
    
    
}

void CompressedDiGraph::clear() {
    for (int i = 0; i != s; ++i) {
        p[i] = -1;
        
        n_s[i] = -1;
        if (!only_subtree) {
            n_p[i] = -1;
            c_b[i] = -1;
        }
        for (int j = 0; j != MAX_N; ++j)
            c[i * MAX_N + j] = 0;
    }
}

void CompressedDiGraph::add_root(int i) {
    roots.push_back(i);
}

void CompressedDiGraph::add_edge(int i, int j) {
    if (i >= s or j >= s)
        return;
    p[j] = i;
    
    loc li = cg->id2l(i);
    loc lj = cg->id2l(j);
    vector<loc> neigh;
    cg->neighbors(li, neigh);
    int q = -1;
    for (int k = 0; k != neigh.size(); ++k){
        if (neigh[k] == lj) {
            q = k;
            break;
        }
    }
    
    if (q != -1) {
        c[i * MAX_N + q] = 1;
    }
    
}

void CompressedDiGraph::children(int i, vector<int>& ch) {
    vector<loc> neigh;
    cg->neighbors(cg->id2l(i), neigh);
    for (int j = 0; j != neigh.size(); ++j) {
        if (c[i * MAX_N + j] == 1) {
            ch.push_back(cg->l2id(neigh[j]));
        }
    }
}


void CompressedDiGraph::add_boundary_leaf(int i) {
    if (i >= s)
        return;
    if (!only_subtree)
        c_b[i] = 1;
}

void CompressedDiGraph::compute() {
    compute_s();
    if (!only_subtree) {
        compute_p();
        compute_cb();
    }
}


void CompressedDiGraph::compute_p() {
    vector<int> active;
    for (vector<int>::iterator i = roots.begin(); i != roots.end(); ++i)
        active.push_back(*i);
    int n;
    while (active.size() != 0) {
        n = active.back();
        active.pop_back();
        if (p[n] == -1)
            n_p[n] = 0;
        else
            n_p[n] = n_p[p[n]] + 1;
        vector<int> ch;
        children(n, ch);
        for (vector<int>::iterator i = ch.begin(); i != ch.end(); ++i)
            active.push_back(*i);
        
    }
}

void CompressedDiGraph::compute_s() {
    vector<int> s;
    for (vector<int>::iterator i = roots.begin(); i != roots.end(); ++i)
        s.push_back(*i);
    int n;
    bool children_evaluated;
    int child_sum;
    while (s.size() != 0) {
        n = s.back();
        children_evaluated = true;
        child_sum = 0;
        vector<int> ch;
        children(n, ch);
        for (vector<int>::iterator i = ch.begin(); i != ch.end(); ++i) {
            if (n_s[*i] == -1) {
                s.push_back(*i);
                children_evaluated = false;
            }
            else
                child_sum += n_s[*i];
        }
        if (children_evaluated) {
            n_s[n] = child_sum + 1;
            s.pop_back();
        }
    }
}

void CompressedDiGraph::compute_cb() {
    vector<int> s; // stack
    for (vector<int>::iterator i = roots.begin(); i != roots.end(); ++i)
        s.push_back(*i);
    int n;
    bool children_evaluated;
    int child_sum;
    while (s.size() != 0) {
        n = s.back();
        children_evaluated = true;
        child_sum = 0;
        vector<int> ch;
        children(n, ch);
        for (vector<int>::iterator i = ch.begin(); i != ch.end(); ++i) {
            if (c_b[*i] == -1) {
                s.push_back(*i);
                children_evaluated = false;
            }
            else
                child_sum += c_b[*i];
        }
        if (children_evaluated) {
            c_b[n] = child_sum;
            s.pop_back();
        }
    }
}

void CompressedDiGraph::print() {
    for (int i = 0; i != s; i++) {
        cout << "ID: " << i << " ";
        cout << "P: " << p[i] << " ";
        cout << "C: ";
        for (int j = 0; j != MAX_N; ++j)
            cout << c[i * MAX_N + j] << " ";
        cout << "NS: " << n_s[i] << " ";
        cout << "NP: " << n_p[i] << " ";
        cout << "CB: " << c_b[i] << endl;
    }
}