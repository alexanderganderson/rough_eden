//
//  loc.h
//  rough_eden
//
//  Created by Alexander G Anderson on 1/22/14.
//  Copyright (c) 2014 Alexander G Anderson. All rights reserved.
//

#ifndef rough_eden_loc_h
#define rough_eden_loc_h

/*
 Each cell has a location that is a pair of integers
 */

typedef std::pair<int,int> loc;


/* The random queue uses a hash table. The following is a hash function
 that takes in a location and outputs a number (i.e. a hash).
 */

struct LocHasher {
    std::size_t operator()(const loc& k) const {
        using std::size_t;
        using std::hash;
        using std::string;
        
        return (hash<int>()(k.first) ^ (hash<int>()(k.second) << 1));
    }
};




#endif
