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
    
    ux = 0;
    uy = 0;
    uz = 1;
    
    path = 0;
    alive = true;
    step = 0;
}
    
void Photon::move(double L) {
    x += L * ux;
    y += L * uy;
    z += L * uz;
    path += L;
}

void Photon::scatter() {
    double mu = 2 * rand01() - 1;
    double phi = 2 * M_PI * rand01();

    double sin_theta = sqrt(1 - mu * mu);

    double uxx = ux, uyy = uy, uzz = uz;

    if (fabs(uzz) > 0.999) {
        ux = sin_theta * cos(phi);
        uy = sin_theta * sin(phi);
        uz = mu * (uzz > 0 ? 1 : -1);
    } else {
        double denom = sqrt(1 - uzz * uzz);

        ux = sin_theta * (uxx * uzz * cos(phi) - uyy * sin(phi)) / denom + uxx * mu;
        uy = sin_theta * (uyy * uzz * cos(phi) + uxx * sin(phi)) / denom + uyy * mu;
        uz = -sin_theta * cos(phi) * denom + uzz * mu;
    }
}
    


// -- Medium --

// -- SimpleMedium --

SimpleMedium::SimpleMedium(double g, double zlim, double p) {
        P_absorb = p;
        Z_limit = zlim;
        ground = g;
}

SimpleMedium::SimpleMedium(double g, double zlim, double s, double scatter_prob) {
    ground = g;
    Z_limit = zlim;
    sigma = s;
    q = scatter_prob;
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

double SimpleMedium::get_sigma(double z) {
    return sigma;
}

double SimpleMedium::get_q(double z) {
    return q;
}



// -- Layer --

Layer::Layer(double z1, double z2, double p) {
    z_min = z1;
    z_max = z2;
    P_absorb = p;
}

Layer::Layer(double z1, double z2, double s, double scatter_prob) {
    z_min = z1;
    z_max = z2;
    sigma = s;
    q = scatter_prob;
}

bool Layer::contains(double z) {
    return (z >= z_min && z < z_max);
}



// -- MultiLayerMedium --

MultiLayerMedium::MultiLayerMedium(double g, double t) {
    ground = g;
    top = t;
}

void MultiLayerMedium::add_layer(double z1, double z2, double s, double q) {
    layers.emplace_back(z1, z2, s, q);
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

double MultiLayerMedium::get_sigma(double z) {
    for (auto &layer : layers) {
        if (layer.contains(z))
            return layer.sigma;
    }
    return 0.0;
}

double MultiLayerMedium::get_q(double z) {
    for (auto &layer : layers) {
        if (layer.contains(z))
            return layer.q;
    }
    return 0.0;
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
            double sigma = medium.get_sigma(photon.z);
            double L = -log(rand01()) / sigma;
            photon.move(L);
            
            if (medium.is_escaped(photon.z)) {
                escaped++;
                break;
            }
            
            if (medium.is_grounded(photon.z)) {
                grounded++;
                break;
            }
            
            double q = medium.get_q(photon.z);

            if (rand01() > q) {
                photon.alive = false; // поглощение
            } else {
                photon.scatter(); // поворот
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
