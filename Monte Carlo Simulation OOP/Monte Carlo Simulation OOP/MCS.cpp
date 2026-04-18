//
//  MCS.cpp
//  Monte Carlo Simulation OOP
//
//  Created by Antony Miroshnichenko on 18.04.2026.
//

#include "MCS.hpp"

double rand01() {
    return (double)rand() / RAND_MAX;
}



// -- Photon --

Photon::Photon() {
    x = y = z = 0;
    path = 0;
    alive = true;
    step = 0;
}
    
void Photon::move(double L) {
    double theta = 2 * M_PI * rand01();
    double phi;
    
    if (step == 0)
        phi = acos(rand01());
    else
        phi = acos(2 * rand01() - 1);
    
    double dx = L * sin(phi) * cos(theta);
    double dy = L * sin(phi) * sin(theta);
    double dz = L * cos(phi);
    
    x += dx;
    y += dy;
    z += dz;
    
    path += L;
    step++;
}
    


// -- Medium --

// -- SimpleMedium --

SimpleMedium::SimpleMedium(double g, double zlim, double p) {
        P_absorb = p;
        Z_limit = zlim;
        ground = g;
}

bool SimpleMedium::is_absorbed(double z) {
    return rand01() < P_absorb;
}

bool SimpleMedium::is_escaped(double z) {
    return z > Z_limit;
}

bool SimpleMedium::is_grounded(double z) {
    return z < ground;
}



// -- Layer --

Layer::Layer(double z1, double z2, double p) {
    z_min = z1;
    z_max = z2;
    P_absorb = p;
}

bool Layer::contains(double z) {
    return (z >= z_min && z < z_max);
}



// -- MultiLayerMedium --

MultiLayerMedium::MultiLayerMedium(double g, double t) {
    ground = g;
    top = t;
}

void MultiLayerMedium::add_layer(double z1, double z2, double p) {
    layers.emplace_back(z1, z2, p);
}

double MultiLayerMedium::get_absorption(double z) {
    for (auto &layer : layers) {
        if (layer.contains(z)) {
            return layer.P_absorb;
        }
    }
    return 0.0;
}

bool MultiLayerMedium::is_absorbed(double z) {
    return rand01() < get_absorption(z);
}

bool MultiLayerMedium::is_escaped(double z) {
    return z > top;
}

bool MultiLayerMedium::is_grounded(double z) {
    return z < ground;
}



// -- Simulation --

Simulation::Simulation(int n, double lmax) {
    N = n;
       L_Max = lmax;
}
    
void Simulation::run(Medium &medium) {
    std::ofstream file("/Users/antonymiroshnichenko/Library/Mobile\ Documents/com\~apple\~CloudDocs/Desktop/workSpace/Курс\ 3/Курсовая/Monte-Carlo-Simulation/Monte\ Carlo\ Simulation\ OOP/Monte\ Carlo\ Simulation\ OOP/data1.txt");
    
    int escaped = 0;
    int grounded = 0;
    double total_path = 0;
    
    for (int i = 0; i < N; i++) {
        Photon photon;
        
        while (photon.alive) {
            double L = rand01() * L_Max;
            photon.move(L);
            
            if (medium.is_escaped(photon.z)) {
                escaped++;
                break;
            }
            
            if (medium.is_grounded(photon.z)) {
                grounded++;
                break;
            }
            
            if (medium.is_absorbed(photon.z)) {
                photon.alive = false;
            }
        }
        
        total_path += photon.path;
    }
    
    double avg_path = total_path / N;
    
    file << (double)escaped / N << " "
            << (double)grounded / N << " "
    << avg_path << std::endl;
    
    file.close();
}
