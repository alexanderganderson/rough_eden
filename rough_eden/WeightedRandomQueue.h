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

#endif /* defined(__rough_eden__WeightedRandomQueue__) */

class WeightedRandomQueue {
public:
    RandomQueue rq0;
    RandomQueue rq1;
    double s;
    
//public:
    WeightedRandomQueue(double _s);
    bool isEmpty();
    void insert(loc l, char type);
    loc pop();
    int remove(loc l, char type);
    void print();
    void clear();
    int getSize();
    
};