//
//  main.cpp
//  Monte Carlo Simulation OOP
//
//  Created by Antony Miroshnichenko on 08.04.2026.
//

#include <iostream>

#include "MCS.hpp"



int main() {
    srand(time(0));

    Simulation sim(10000, 1.0);

    int choice;
    std::cout << "Choose medium:\n" << "1 - Simple, 2 - Multi-layer\n";
    std::cin >> choice;
    
    if (choice == 1) {
        SimpleMedium medium(0, 30, 0.02);
        sim.run(medium);
    }
    
    else if (choice == 2) {
        MultiLayerMedium medium(0, 30);

        medium.add_layer(0, 5, 0.06);
        medium.add_layer(5, 10, 0.05);
        medium.add_layer(10, 15, 0.04);
        medium.add_layer(10, 20, 0.03);
        medium.add_layer(20, 25, 0.02);
        medium.add_layer(25, 30, 0.01);
        
        sim.run(medium);
    }

    std::cout << "Done!" << std::endl;
    return 0;
}
