//
//  HashMapHistogram.cpp
//  rough_eden
//
//  Created by Alexander G Anderson on 3/16/14.
//  Copyright (c) 2014 Alexander G Anderson. All rights reserved.
//

#include "HashMapHistogram.h"



HashMapHistogram::HashMapHistogram() {
    Counter c;
}

// Increment the value associated with i by 1
void HashMapHistogram::increment(int i) {
    if(c.find(i) == c.end())
        c[i] = 1;
    else
        c[i] += 1;
}

void HashMapHistogram::clear() {
    c.clear();
}

void HashMapHistogram::print() {
    for (auto it = c.begin(); it != c.end(); ++it) {
        cout << it->first << " " << it->second << endl;
    }
}

void HashMapHistogram::save(std::ofstream & outfile) {
    std::vector<int> v;
    for (auto it = c.begin(); it != c.end(); ++it)
        v.push_back(it->first);
    std::sort(v.begin(), v.end());
    for (auto it = v.begin(); it != v.end(); ++it) {
        outfile << (*it) << " " << c[*it] << endl;
    }
    
}