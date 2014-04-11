//
//  random_queue.cpp
//  rough_eden
//
//  Created by Alexander G Anderson on 12/10/13.
//  Copyright (c) 2013 Alexander G Anderson. All rights reserved.
//

#include "random_queue.h"

RandomQueue::RandomQueue() {
    std::unordered_map<loc, int, LocHasher> loc_map;
    vector<loc> b;
}

bool RandomQueue::isEmpty() {
    return b.size() == 0;
}

int RandomQueue::insert(loc l) {
    if (loc_map.find(l) == loc_map.end()) {
        int n = (int) b.size();
        b.push_back(l);
        loc_map[l] = n;
        return 0;
    }
    else {
        return 1;
    }
}

loc RandomQueue::pop() {
    if (isEmpty())
        cout << "Tried to remove an element from an empty queue" << endl;
    int n = (int) b.size();
    //get random item
    int k = rand() % n;
    loc ret = b[k];
    if (k != n - 1) {
    //move last item in array to that index
        loc last_loc = b[n - 1];
        b[k] = last_loc;
    //update map
        loc_map[last_loc] = k;
    }
    loc_map.erase(ret);
    //b[n - 1] = make_pair(-1, -1);
    b.pop_back();
    
    return ret;
}

int RandomQueue::remove(loc l) {
    //check if l is in hashmap
    if (loc_map.find(l) == loc_map.end()) {
        //cout << "Element " << l << " doesn't exist" << endl;
        return 1;
    }
    else {
        int n = (int) b.size();
        int k = loc_map[l];
        if (k != n -1) {
            loc last_loc = b[n - 1];
            b[k] = last_loc;
            loc_map[last_loc] = k;
        }
        loc_map.erase(l);
        //b[n - 1] = make_pair(-1,-1);
        b.pop_back();
        return 0; //successful removal
    }
    
}

void RandomQueue::print() {
    cout << "The locations in the boundary are:" << endl;
    for (vector<loc>::iterator i = b.begin(); i != b.end(); ++i) {
        cout << "(" << i->first << ", " << i->second << "), ";
    }
    cout << endl << "The values in the hashmap are:" << endl;
    for (auto it = loc_map.begin(); it != loc_map.end(); ++it)
        cout << "(" << (it->first).first << ", " << (it->first).second <<"), : " << (it->second) << ", ";
    cout << endl << "The number of elements is " << b.size() << endl;
    
    
}

void RandomQueue::clear() {
    // leaves L the same
    loc_map.clear();
    b.clear();
}

int RandomQueue::getSize() {
    return (int) b.size();
}