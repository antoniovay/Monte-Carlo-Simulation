//
//  main.cpp
//  Monte Carlo Simulation
//
//  Created by Antony Miroshnichenko on 18.10.2025.
//

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>

using namespace std;

double rand01() {
    return (double)rand() / RAND_MAX;
}

void random_direction(double &dx, double &dy, double &dz, double L, bool first) {
    double theta = 2 * M_PI * rand01();
    double phi;

    if (!first) {
        phi = acos(rand01()); // z >= 0
    } else {
        phi = acos(2 * rand01() - 1);
    }

    dx = L * sin(phi) * cos(theta);
    dy = L * sin(phi) * sin(theta);
    dz = L * cos(phi);
}
/*                  -- Для константной P_Absorb --
int main() {
    srand(time(0));

    int N = 10000;            // общее число фотонов
    double L_Max = 1.0;       // максимальная длина шага
    double P_Absorb = 0.02;   // вероятность поглощения
    double Z_Limit = 30;      // граница среды
    double ground = 0;        // уровень земли

    double total_path = 0.0;
    int escaped = 0;
    int grounded = 0;

    for (int i = 0; i < N; i++) {
        double x = 0, y = 0, z = 0;
        double path = 0.0;

        bool alive = true;
        int step = 0;

        while (alive) {
            double dx, dy, dz;
            double L = rand01() * L_Max;
            random_direction(dx, dy, dz, L, step == 0);

            x += dx;
            y += dy;
            z += dz;

            path += L;
            step++;

            // Проверка выхода из среды
            if (z > Z_Limit) {
                escaped++;
                break;
            }
            
            // Проверка на поглощение землёй
            if (z < ground) {
                grounded++;
                break;
            }

            // Поглощение (только внутри среды)
            if (rand01() < P_Absorb) {
                alive = false;
            }
        }

        total_path += path;
    }

    double avg_path = total_path / N;

    cout << "Total photons:              " << N << endl; // общее число фотонов
    cout << "Escaped photons (z > 30):   " << escaped << " (" << (double)escaped * 100 / N << "%)" << endl; // число вылетевших из среды фотонов
    cout << "Grounded photons (z < 0):   " << grounded << " (" << (double)grounded * 100 / N << "%)" << endl; // число поглощённых землёй фотонов
    cout << "Average path length:        " << avg_path << endl << endl; // средняя длина пути фотона

    return 0;
}
*/

/*
                    -- Для проверки P_Absorb от 0 до 1 --
 */
int main() {
    srand(time(0));

    int N = 10000;            // общее число фотонов
    double L_Max = 1.0;       // максимальная длина шага
    //double P_Absorb = 0.02;   // вероятность поглощения
    double Z_Limit = 30;      // граница среды
    double ground = 0;        // уровень земли

    for (double P_Absorb = 0; P_Absorb <= 1; P_Absorb += 0.001) {
        double total_path = 0.0;
        int escaped = 0;
        int grounded = 0;
        
        for (int i = 0; i < N; i++) {
            double x = 0, y = 0, z = 0;
            double path = 0.0;
            
            bool alive = true;
            int step = 0;
            
            while (alive) {
                double dx, dy, dz;
                double L = rand01() * L_Max;
                random_direction(dx, dy, dz, L, step == 0);
                
                x += dx;
                y += dy;
                z += dz;
                
                path += L;
                step++;
                
                // Проверка выхода из среды
                if (z > Z_Limit) {
                    escaped++;
                    break;
                }
                
                // Проверка на поглощение землёй
                if (z < ground) {
                    grounded++;
                    break;
                }
                
                // Поглощение (только внутри среды)
                if (rand01() < P_Absorb) {
                    alive = false;
                }
            }
            
            total_path += path;
        }
        
        double avg_path = total_path / N;
        
        cout << "Total photons:              " << N << endl; // общее число фотонов
        cout << "Escaped photons (z > 30):   " << escaped << " (" << (double)escaped * 100 / N << "%)" << endl; // число вылетевших из среды фотонов
        cout << "Grounded photons (z < 0):   " << grounded << " (" << (double)grounded * 100 / N << "%)" << endl; // число поглощённых землёй фотонов
        cout << "Average path length:        " << avg_path << endl << endl; // средняя длина пути фотона
    }
    return 0;
}
