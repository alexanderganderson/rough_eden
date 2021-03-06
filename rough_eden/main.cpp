//
//  main.cpp
//  rough_eden
//
//  Created by Alexander G Anderson on 12/10/13.
//  Copyright (c) 2013 Alexander G Anderson. All rights reserved.
//

#include <iostream>
#include <random>
#include "Simulation.h"
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
    const int N_REPLICATES = 1;
    const int N_PARAM = 1;
    const int L_X = 200;
    const int L_Y = 100;
    //const int R = 200;
    const int digraph_factor = 0; // digraph allocates space for this * L_X * L_Y cells, usually use 0 for nothing, 1 for circular growth, 10 for linear growth
    
    //double s_set[N_PARAM];
    //s_set[0] = 0.01; s_set[1] = 1.0;
    //double s_set[] = {0.005, 0.01, 0.015, 0.02, 0.03, 0.04, 0.05, 0.06, 0.07, 0.10, 0.20};
    double s_set[] = {-0.075};
    //double s_set[] = {0.20, 0.10, 0.0};
    //double p_set[] = {0.0000};
    double m0_set[] = {0};
    
    string datapath = "/Users/alex/Dropbox/Berkeley/Hallatschek/simulation_data/";
    
    ofstream sim_data_outfile(datapath + "sim_data.txt");

    Simulation sim(L_X, L_Y, sim_data_outfile, digraph_factor, s_set[0]);
    
    
    int result;
    vector<int> winners;
    vector<int> mut_nums;
    

    
    for (int i = 0; i < N_PARAM; i++) {
        sim.set_s(s_set[i]);
        sim.set_p(0.0);
        sim.set_m0(m0_set[i]);
        //sim.set_p(p_set[i]);
        for (int j = 0; j < N_REPLICATES; j++) {
            if (j % 10 == 0)
                cout << "Simulation " << (j + N_REPLICATES * i) << " is running." << endl;
            sim.clear();
            
            //sim.initialize_circular_sectorpic();
            //sim.initialize_diffusive_boundary();
            //sim.initialize_circular();
            sim.initialize_linear();
            //sim.initialize_sector(R);
            
            result = sim.run();
            winners.push_back(result);
            mut_nums.push_back(sim.get_mut_num());
            sim_data_outfile << endl;
        }
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
    
    for (int i = 0; i != N_REPLICATES * N_PARAM; ++i) {
        if (winners[i] != -1) {
            w_c[i / N_REPLICATES][winners[i]] += 1;
        }
    }
    
    string summary_file_name = "summary.txt";
    std::ofstream summary_outfile(datapath + summary_file_name);
    
    //outfile << w_c[0] << " " << w_c[1] << endl;
    
    for (int i = 0; i != N_REPLICATES * N_PARAM; ++i) {
        summary_outfile << winners[i] << "," << mut_nums[i] << endl;
    }
    summary_outfile.close();
    
    
    for (int i = 0; i != N_PARAM; ++i) {
        cout << "The win frequencies for type (0,1,n/a) are (" << w_c[i][0] << ", " << w_c[i][1] << ", " << N_REPLICATES-w_c[i][0]-w_c[i][1] << ") for s = " << s_set[i] << endl;
    }
    
    for (int i = 0; i != N_PARAM; ++i) {
        cout << N_REPLICATES-w_c[i][0]-w_c[i][1] << "," << s_set[i] << "," << m0_set[i] << endl;
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


