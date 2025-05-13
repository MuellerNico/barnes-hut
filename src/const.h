#ifndef CONST_H
#define CONST_H

#include <cmath>

constexpr double AU = 1.496e11; // L: 1 AU in meters
constexpr double M_sun = 1.989e30; // M: M_sun mass of sun in kg
constexpr double year = 3.154e7; // T: 1 year in seconds
constexpr double G = 4 * M_PI * M_PI; // gravitational constant in AU^3 / (M_sun * year^2)

constexpr int numParticles = 50; 
constexpr double theta = 0.5; // threshold. using center of mass approximation if s/d < theta. If zero, degenerates to brute force
constexpr double dt = 0.01; // time step

constexpr double eps = 1e-8; // small value to avoid division by zero
constexpr double eps2 = eps * eps; // for force calculation

#endif // CONST_H