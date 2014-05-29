//
//  HashMapHistogram.h
//  rough_eden
//
//  Created by Alexander G Anderson on 3/16/14.
//  Copyright (c) 2014 Alexander G Anderson. All rights reserved.
//

#ifndef __rough_eden__HashMapHistogram__
#define __rough_eden__HashMapHistogram__

#include <iostream>
#include <unordered_map>
#include <fstream>
#include <vector>
#include <algorithm>

using std::endl;
using std::cout;

typedef std::unordered_map<int, int> Counter;

/*
 Hash Map Histogram
 
 Sample usage:
 Insert a collection of integers
 
 The data structure stores the integers and the number of times
    that each integer has been inserted.
 
 Useful for storing the number of mutants produced for large simulations
 */

class HashMapHistogram {
public:
    Counter c;
    
    HashMapHistogram(); // constructor
    void increment(int i); // increments the value associated with i by one
    void print();
    void clear(); // empties the histogram
    void save(std::ofstream & outfile); // saves in format:
    //  value, count associated with value
};


#endif /* defined(__rough_eden__HashMapHistogram__) */
