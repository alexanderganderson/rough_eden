//
//  WeightedRandomQueue.h
//  rough_eden
//
//  Created by Alexander G Anderson on 1/24/14.
//  Copyright (c) 2014 Alexander G Anderson. All rights reserved.
//

#ifndef __rough_eden__WeightedRandomQueue__
#define __rough_eden__WeightedRandomQueue__

#include <iostream>
#include "random_queue.h"

/* Weighted Random Queue:
 
 Queue to support Eden Model Simulation:
 Stores the locations of cells on the boundary of the growing colony
 Allows for two cell types with different growth rates, 1, 1 + s
 Supports the following operations:
 pop()
 insert(loc l)
 remove(loc l)
 
 in O(1) time.
 
 Contains two RandomQueues
 
 */


class WeightedRandomQueue {
public:
    RandomQueue rq0;
    RandomQueue rq1;
    double s;
    
    WeightedRandomQueue(double _s);
    bool isEmpty();
    void insert(loc l, char type);
    loc pop();
    int remove(loc l);
    void print();
    void clear();
    int getSize();
    
};

#endif /* defined(__rough_eden__WeightedRandomQueue__) */

