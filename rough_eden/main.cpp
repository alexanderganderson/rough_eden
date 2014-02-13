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


int main() {
    int N_SIM = 1;
    int L_X = 7;
    int L_Y = 12;
    double s = 0.1;
    simulation sim(L_X, L_Y, s);
    int mut_tot;
    std::string datapath = "/Users/alex/Dropbox/Berkeley/Hallatschek/simulation_data/";
    std::string filename = "output.txt";
    std::ofstream outfile(datapath + filename);
    
    for (int i = 0; i < N_SIM; i++) {
        if (i % 1000 ==0)
            std::cout << "Simulation " << i << " is running." << std::endl;
        sim.clear();
        sim.initialize();
        mut_tot = sim.run();
        if (mut_tot != -1)
            outfile << mut_tot << std::endl;
    }
    
    outfile.close();
    std::cout << "Simulation Finished" << std::endl;
    return 0;
}

int main1() {
    
    int a = 20;
    
    std::string s = std::to_string(a);
    
    cout << a;
    
    return 0;
}