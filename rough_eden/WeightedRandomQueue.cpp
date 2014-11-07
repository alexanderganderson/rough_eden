//
//  WeightedRandomQueue.cpp
//  rough_eden
//
//  Created by Alexander G Anderson on 1/24/14.
//  Copyright (c) 2014 Alexander G Anderson. All rights reserved.
//

#include "WeightedRandomQueue.h"

// Initializes a weighted random queue
// _s = the fitness advantage corresponding to type 1
WeightedRandomQueue::WeightedRandomQueue(double _s) {
    s = _s;
    RandomQueue rq0;
    RandomQueue rq1;
}

bool WeightedRandomQueue::isEmpty() {
    return rq0.isEmpty() and rq1.isEmpty();
}

// Insert a location of cell type 'type'
//  type = 0 or 1
void WeightedRandomQueue::insert(loc l, char type) {
    if (type == 0)
        rq0.insert(l);
    else if (type == 1)
        rq1.insert(l);
    else
        cout << "This queue only supports two types." << endl;
}

// Return a location randomly proportional to the growth rate of the cell
loc WeightedRandomQueue::pop() {
    
    int a = rq0.getSize();
    int b = rq1.getSize();
    if (a + b == 0)
        cout << "ERROR: Cannot pop from an empty queue." << endl;
    
    double r = (double) rand() / ( (double) RAND_MAX);
    double p = a / (a + b + b * s);
    loc ret;
    
    if (r <= p) {
        ret = rq0.pop();
    }
    else {
        ret = rq1.pop();
    }
    return ret;
}

// Remove a location from the queue
int WeightedRandomQueue::remove(loc l) {
    rq0.remove(l);
    rq1.remove(l);
    return 1;
}

void WeightedRandomQueue::print() {
    rq0.print();
    rq1.print();
}

void WeightedRandomQueue::clear() {
    rq0.clear();
    rq1.clear();
}

int WeightedRandomQueue::getSize() {
    return rq0.getSize() + rq1.getSize();
}

