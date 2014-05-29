//
//  random_queue.h
//  rough_eden
//
//  Created by Alexander G Anderson on 12/10/13.
//  Copyright (c) 2013 Alexander G Anderson. All rights reserved.
//

#ifndef __rough_eden__random_queue__
#define __rough_eden__random_queue__

#include <iostream>
#include <unordered_map>
#include <random>
#include <string>
#include "loc.h"

using std::cout;
using std::endl;
using std::vector;
using std::make_pair;




/* Random Queue:

Queue to support Eden Model Simulation:
 Stores the locations of cells on the boundary of the growing colony
 Supports the following operations:
 pop()
 insert(loc l)
 remove(loc l)
 
 in O(1) time.
 
 Note the important feature that the pop returns an item from the 
  queue uniform randomly.
 
*/
class RandomQueue {
public:
    std::unordered_map<loc, int, LocHasher> loc_map; //loc, position in array
    std::vector<loc> b; //locations of cells in boundary
    
public:
    RandomQueue();
    bool isEmpty();
    int insert(loc l); //Insert cell into boundary, return 1 if already in boundary
    loc pop(); //Pop a random cell from boundary
    int remove(loc l); //remove a cell from the Queue given location
    void print();
    void clear();
    int getSize(); // Returns number of cells in the Queue
};

#endif /* defined(__rough_eden__random_queue__) */
