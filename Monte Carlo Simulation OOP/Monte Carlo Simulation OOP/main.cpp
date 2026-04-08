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



double rand01() {
    return (double)rand() / RAND_MAX;
}



class Photon {
public:
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
    Medium(double pabs, double zlim, double g) {
        P_Absorb = pabs;
        Z_Limit = zlim;
        ground = g;
    }
    
    bool isAbsorbed() {
        return rand01() < P_Absorb;
    }
    
private:
    double P_Absorb;
    double Z_Limit;
    double ground;
};



class Simulation {
public:
    Simulation(int n, double lmax) {
        N = n;
        L_Max = lmax;
    }
    
    void run() {
        ofstream file("/Users/antonymiroshnichenko/Library/Mobile\ Documents/com\~apple\~CloudDocs/Desktop/workSpace/Курс\ 3/Курсовая/Monte-Carlo-Simulation/Monte\ Carlo\ Simulation/Prog/Monte\ Carlo\ Simulation/data1.txt");
        
        for (double P = 0; P <= 1; P += 0.01) {
            Medium medium(P, 30, 0);
            
            int escaped = 0;
            int grounded = 0;
            double total_path = 0;
            
            for (int i = 0; i < N; i++) {
                Photon photon;
            
                while (photon.alive) {
                    double L = rand01() * L_Max;
                    photon.move(L);
                    
                    if (photon.z > medium.Z_Limit) {
                        escaped++;
                        break;
                    }
                    
                    if (photon.z < medium.ground) {
                        grounded++;
                        break;
                    }
                    
                    if (medium.isAbsorbed()) {
                        photon.alive = false;
                    }
                }
                
                total_path += photon.path;
            }
            
            double avg_path = total_path / N;
            
            file << P << " " << (double)escaped / N << " " << (double)grounded / N << " " << avg_path << std::endl;
        }
        
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
