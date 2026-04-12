//
//  main.cpp
//  Monte Carlo Simulation OOP
//
//  Created by Antony Miroshnichenko on 08.04.2026.
//

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <vector>



double rand01() {
    return (double)rand() / RAND_MAX;
}



class Photon {
public:
    friend class Simulation;
    
    Photon() {
        x = y = z = 0;
        path = 0;
        alive = true;
        step = 0;
    }
    
    void move(double L) {
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
    
private:
    double x, y, z;
    double path;
    bool alive;
    int step;
};



class Medium {
public:
    virtual bool is_absorbed(double z) = 0;
    virtual bool is_escaped(double z) = 0;
    virtual bool is_grounded(double z) = 0;

    virtual ~Medium() {}
};



class SimpleMedium : public Medium {
public:

    SimpleMedium(double p, double zlim, double g) {
        P_absorb = p;
        Z_limit = zlim;
        ground = g;
    }

    bool is_absorbed(double z) override {
        return rand01() < P_absorb;
    }

    bool is_escaped(double z) override {
        return z > Z_limit;
    }

    bool is_grounded(double z) override {
        return z < ground;
    }
    
private:
    double P_absorb;
    double Z_limit;
    double ground;
};



class Layer {
public:
    friend class MultiLayerMedium;
    
    Layer(double z1, double z2, double p) {
        z_min = z1;
        z_max = z2;
        P_absorb = p;
    }

    bool contains(double z) {
        return (z >= z_min && z < z_max);
    }
    
private:
    double z_min, z_max;
    double P_absorb;
};



class MultiLayerMedium : public Medium {
public:
    std::vector<Layer> layers;
    double ground, top;

    MultiLayerMedium(double g, double t) {
        ground = g;
        top = t;
    }

    void add_layer(double z1, double z2, double p) {
        layers.emplace_back(z1, z2, p);
    }

    double get_absorption(double z) {
        for (auto &layer : layers) {
            if (layer.contains(z)) {
                return layer.P_absorb;
            }
        }
        return 0.0;
    }

    bool is_absorbed(double z) override {
        return rand01() < get_absorption(z);
    }

    bool is_escaped(double z) override {
        return z > top;
    }

    bool is_grounded(double z) override {
        return z < ground;
    }
};



class Simulation {
public:
    Simulation(int n, double lmax) {
        N = n;
        L_Max = lmax;
    }
    
    void run(Medium &medium) {
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
    
private:
    int N;
    double L_Max;
};



int main() {
    srand(time(0));
    
    Simulation simulation(10000, 1.0);
    simulation.run();
    
    return 0;
}
