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


class HashMapHistogram {
public:
    Counter c;
    
    HashMapHistogram(); // constructor
    void increment(int i); // increments the value with i by one
    void print();
    void clear(); // empties the histogram
    void save(std::ofstream & outfile); // saves
};


#endif /* defined(__rough_eden__HashMapHistogram__) */
