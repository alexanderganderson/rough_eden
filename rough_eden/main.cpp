//
//  main.cpp
//  rough_eden
//
//  Created by Alexander G Anderson on 12/10/13.
//  Copyright (c) 2013 Alexander G Anderson. All rights reserved.
//

#include <iostream>
#include <random>
#include "simulation.h"
#include <stdio.h>
#include <fstream>
#include "random_queue.h"
#include "WeightedRandomQueue.h"
#include <string>
//#include


int main1() {
    int N_SIM = 10000;
    int L_X = 100;
    int L_Y = 50;
    double s = 0.0;
    int result;
    std::string datapath = "/Users/alex/Dropbox/Berkeley/Hallatschek/simulation_data/";
    std::string filename = "output.txt";
    std::ofstream outfile(datapath + filename);
    
    simulation sim(L_X, L_Y, s);

    
    vector<int> winners;
    vector<int> mut_nums;
    
    for (int i = 0; i < N_SIM; i++) {
        if (i % 1000 == 0)
            cout << "Simulation " << i << " is running." << endl;
        sim.clear();
        sim.initialize();
        result = sim.run();
        winners.push_back(result);
        mut_nums.push_back(sim.get_mut_num());
    }

    int w_c[2] = {};
    
    for (vector<int>::iterator i = winners.begin(); i != winners.end(); ++i) {
        if (*i != -1) {
            w_c[*i] += 1;
        }
    }
    
    
    //outfile << w_c[0] << " " << w_c[1] << endl;
    
    for (int i = 0; i != mut_nums.size(); ++i) {
        if (winners[i] == 0) {
            outfile << mut_nums[i] << endl;
        }
    }
    outfile.close();
    
    cout << "The win frequencies for type (0,1) are (" << w_c[0] << ", " << w_c[1] << ")" << endl;
    
    cout << "Simulation Finished" << endl;
    return 0;
}

int main() {
    clock_t c1,c2;
    c1 = clock();
    main1();
    c2 = clock();
    float diff = ((float)c2 - (float)c1) / CLOCKS_PER_SEC;
    cout << "The program ran for " << diff << " s." << endl;
    
}