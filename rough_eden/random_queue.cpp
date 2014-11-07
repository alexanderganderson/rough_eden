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

// Insert a location, l, into the RandomQueue
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

// Return a random location in the queue
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
    b.pop_back();
    
    return ret;
}

// Removes a location from the queue
//  returns 0 if successful, 1 if unsuccessful
int RandomQueue::remove(loc l) {
    //check if l is in hashmap
    if (loc_map.find(l) == loc_map.end()) {
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
        b.pop_back();
        return 0; //successful removal
    }
    
}

// Full output of the internal state of the queue
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

// Clears the queue
void RandomQueue::clear() {
    loc_map.clear();
    b.clear();
}

// Return the number of elements currently stored in the queue
int RandomQueue::getSize() {
    return (int) b.size();
}