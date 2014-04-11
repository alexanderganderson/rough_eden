//
//  DiGraph.cpp
//  rough_eden
//
//  Created by Alexander G Anderson on 2/24/14.
//  Copyright (c) 2014 Alexander G Anderson. All rights reserved.
//

#include "DiGraph.h"

Node::Node() {
    vector<int> c;

    clear();
}
void Node::clear() {
    p = -1;
    n_s = -1;
    n_p = -1;
    c_b = -1;
    c.clear();
}

void Node::print() {
    cout << "P:" << p << " n_s:" << n_s << " n_p:" << n_p << " c_b:" << c_b << endl;
}

DiGraph::DiGraph(int _s) {
    s = _s;
    vector<int> roots;
    nodes = new Node[s];
    clear();
}

void DiGraph::clear() {
    for (int i = 0; i != s; ++i)
        nodes[i].clear();
    roots.clear();
}

void DiGraph::add_root(int i) {
    roots.push_back(i);
}

void DiGraph::add_edge(int i, int j) {
    if (i >= s or j >= s)
        return;
    nodes[i].c.push_back(j);
    nodes[j].p = i;
}

void DiGraph::add_boundary_leaf(int i) {
    if (i >= s)
        return;
    nodes[i].c_b = 1;
}

void DiGraph::compute() {
    compute_s();
    compute_p();
    compute_cb();
}

void DiGraph::compute_p() {
    vector<int> active;
    for (vector<int>::iterator i = roots.begin(); i != roots.end(); ++i)
        active.push_back(*i);
    int n;
    while (active.size() != 0) {
        n = active.back();
        active.pop_back();
        if (nodes[n].p == -1)
            nodes[n].n_p = 0;
        else
            nodes[n].n_p = nodes[nodes[n].p].n_p + 1;
        for (vector<int>::iterator i = nodes[n].c.begin(); i != nodes[n].c.end(); ++i)
            active.push_back(*i);
        
    }
}

void DiGraph::compute_s() {
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
        for (vector<int>::iterator i = nodes[n].c.begin(); i != nodes[n].c.end(); ++i) {
            if (nodes[*i].n_s == -1) {
                s.push_back(*i);
                children_evaluated = false;
            }
            else
                child_sum += nodes[*i].n_s;
        }
        if (children_evaluated) {
            nodes[n].n_s = child_sum+1;
            s.pop_back();
        }
    }
}

void DiGraph::compute_cb() {
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
        for (vector<int>::iterator i = nodes[n].c.begin(); i != nodes[n].c.end(); ++i) {
            if (nodes[*i].c_b == -1) {
                s.push_back(*i);
                children_evaluated = false;
            }
            else
                child_sum += nodes[*i].c_b;
        }
        if (children_evaluated) {
            nodes[n].c_b = child_sum;
            s.pop_back();
        }
    }
}


void DiGraph::print() {
    
    for (int i = 0; i != s; ++i) {
        cout << i << " ";
        nodes[i].print();
    }
}



