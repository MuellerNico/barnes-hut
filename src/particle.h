#ifndef PARTICLE_H
#define PARTICLE_H

#include "vec3.h"

struct Particle {
    Vec3 position;
    Vec3 velocity;
    double mass;
    double radius;
    int id;
};

#endif // PARTICLE_H