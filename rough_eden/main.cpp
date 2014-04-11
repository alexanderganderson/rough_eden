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
#include "DiGraph.h"
#include "HashMapHistogram.h"

using std::ofstream;
using std::string;

int main1() {
    // Parameters
    const int N_SIM = 1;
    const int N_PARAM = 1;
    const int L_X = 1000;
    const int L_Y = 1000;
    int R = 2000;
    const int digraph_factor = 0; // digraph allocates space for this * L_X * L_Y cells, usually use 0 for nothing, 1 for circular growth, 10 for linear growth
    
    //double s_set[N_PARAM];
    //s_set[0] = 0.01; s_set[1] = 1.0;
    //double s_set[] = {0.005, 0.01, 0.015, 0.02, 0.03, 0.04, 0.05, 0.06, 0.07, 0.10, 0.20};
    
    double s_set[] = {0.03};
    
    
    string datapath = "/Users/alex/Dropbox/Berkeley/Hallatschek/simulation_data/";
    
    ofstream sim_data_outfile(datapath + "sim_data.txt");

    simulation sim(L_X, L_Y, s_set[0], sim_data_outfile, digraph_factor);
    
    
    int result;
    vector<int> winners;
    vector<int> mut_nums;
    

    
    for (int i = 0; i < N_SIM * N_PARAM; i++) {
        if (i % N_SIM == 0)
            sim.set_s(s_set[i / N_SIM]);
        
        if (i % 10 == 0)
            cout << "Simulation " << i << " is running." << endl;
        sim.clear();
        
        sim.initialize_circular_sectorpic();
        //sim.initialize_diffusive_boundary();
        //sim.initialize_circular();
        //sim.initialize_linear();
        
        //sim.initialize_sector(R);
        result = sim.run();
        winners.push_back(result);
        mut_nums.push_back(sim.get_mut_num());
        
    }

    sim_data_outfile.close();
    ofstream s(datapath + "all.txt");
    ofstream se(datapath + "est.txt");
    ofstream sd(datapath + "die.txt");
    sim.h.save(s);
    sim.he.save(se);
    sim.hd.save(sd);
    s.close();
    se.close();
    sd.close();
    
    
    
    
    
    // Collect the simulation information
    int w_c[N_PARAM][2] = {};
    
    for (int i = 0; i != N_SIM * N_PARAM; ++i) {
        if (winners[i] != -1) {
            w_c[i / N_SIM][winners[i]] += 1;
        }
    }
    
    string summary_file_name = "summary.txt";
    std::ofstream summary_outfile(datapath + summary_file_name);
    
    //outfile << w_c[0] << " " << w_c[1] << endl;
    
    for (int i = 0; i != N_SIM * N_PARAM; ++i) {
        summary_outfile << winners[i] << "," << mut_nums[i] << endl;
    }
    summary_outfile.close();
    
    
    for (int i = 0; i != N_PARAM; ++i) {
        cout << "The win frequencies for type (0,1) are (" << w_c[i][0] << ", " << w_c[i][1] << ") for s = " << s_set[i] << endl;
    }
    return 0;
}

int main() {
    clock_t c1,c2;
    c1 = clock();
    main1();
    c2 = clock();
    float diff = ((float)c2 - (float)c1) / CLOCKS_PER_SEC;
    cout << "The program ran for " << diff << " s." << endl;
    return 0;
}

