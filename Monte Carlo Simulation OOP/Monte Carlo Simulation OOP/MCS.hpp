//
//  MCS.hpp
//  Monte Carlo Simulation OOP
//
//  Created by Antony Miroshnichenko on 18.04.2026.
//

#ifndef MCS_hpp
#define MCS_hpp

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <vector>

double rand01();


class Photon {
public:
    friend class Simulation;
    
    Photon();
    
    void move(double L);
    void scatter();
    
private:
    double x, y, z;
    double ux, uy, uz; // направление
    double path;
    bool alive;
    int step;
};



class Medium {
public:
    virtual bool is_absorbed(double z) = 0;
    virtual bool is_escaped(double z) = 0;
    virtual bool is_grounded(double z) = 0;
    virtual double get_sigma(double z) = 0;
    virtual double get_q(double z) = 0;
    
    virtual ~Medium() {}
};



class SimpleMedium : public Medium {
public:
    SimpleMedium(double g, double zlim, double p);
    SimpleMedium(double g, double zlim, double s, double scatter_prob);

    bool is_absorbed(double z) override;
    bool is_escaped(double z) override;
    bool is_grounded(double z) override;
    
    double get_sigma(double z) override;
    double get_q(double z) override;
    
private:
    double P_absorb;
    double Z_limit;
    double ground;
    double sigma;
    double q;
};



class Layer {
public:
    friend class MultiLayerMedium;
    
    Layer(double z1, double z2, double p);
    Layer(double z1, double z2, double s, double scatter_prob);

    bool contains(double z);
    
private:
    double z_min, z_max;
    double P_absorb;
    double sigma, q;
};



class MultiLayerMedium : public Medium {
public:
    MultiLayerMedium(double g, double t);

    void add_layer(double z1, double z2, double s, double q);

    double get_absorption(double z);

    bool is_absorbed(double z) override;
    bool is_escaped(double z) override;
    bool is_grounded(double z) override;
    
    double get_sigma(double z) override;
    double get_q(double z) override;
    
private:
    std::vector<Layer> layers;
    double ground, top;
    double sigma, q;
};



class Simulation {
public:
    Simulation(int n, double lmax);
    
    void run(Medium &medium);
    
private:
    int N;
    double L_Max;
};

#endif
