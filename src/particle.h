#ifndef PARTICLE_H
#define PARTICLE_H

#include "vec2.h"

struct Particle {
    Vec2 position;
    Vec2 velocity;
    double mass;
    double radius;
    int id;
};

#endif // PARTICLE_H