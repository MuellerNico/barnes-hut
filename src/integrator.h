#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#include <iostream>
#include "node.h"
#include "particle.h"
#include "const.h"

struct Integrator {
    Vec3 force(Vec3 p1, double m1, Vec3 p2, double m2);
    Vec3 compute_force(Particle* p, Node* node);
    Vec3 compute_force_naive(Particle* p, const std::vector<Particle>& particles);
    Node* build_tree(std::vector<Particle>& particles);
    void step_euler(std::vector<Particle>& particles, double dt);
    void step_leapfrog(std::vector<Particle>& particles, double dt);
};

// compute force acting on particle p1 from a body (particle or node)
Vec3 Integrator::force(Vec3 p1, double m1, Vec3 p2, double m2) {
    Vec3 dist = p2 - p1;
    double r2 = dist.length2();
    return dist.normalized() * (G * m1 * m2 / (r2 + eps2)); // F = G * m1 * m2 / r^2
}

// force acting on particle p from a node/quad
Vec3 Integrator::compute_force(Particle* p, Node* node) {
    Vec3 f = {0, 0, 0};
    // external node, calculate force from particles (usually 1 body per leaf)
    if (node->is_leaf()) {
        for (Particle* other : node->particles) {
            if (other != p) { // avoid self-interaction
                f += force(p->position, p->mass, other->position, other->mass);
            }
        }
    } else { // is internal node
        // check if far enough away
        double d = (node->center_of_mass - p->position).length();
        double s = node->size.x;
        if (s / (d + eps) < theta) { 
            // use center of mass approximation
            f += force(p->position, p->mass, node->center_of_mass, node->mass);    
        } else {
            // recursively visit children
            for (Node* child : node->children) { 
                f += compute_force(p, child);
            }
        }
    }
    return f;
}

// O(N^2) naive implementation for benchmarking
Vec3 Integrator::compute_force_naive(Particle* p, const std::vector<Particle>& particles) {
    Vec3 f = {0, 0, 0};
    for (const Particle& other : particles) {
        if (&other != p) { // avoid self-interaction
            f += force(p->position, p->mass, other.position, other.mass);
        }
    }
    return f;
}

// build octree from particles
Node* Integrator::build_tree(std::vector<Particle>& particles) {
    // find bounding box
    Vec3 min = particles[0].position;   
    Vec3 max = particles[0].position;
    for (const Particle& p : particles) {
        min.x = std::min(min.x, p.position.x);
        min.y = std::min(min.y, p.position.y);
        min.z = std::min(min.z, p.position.z);
        max.x = std::max(max.x, p.position.x);
        max.y = std::max(max.y, p.position.y);
        max.z = std::max(max.z, p.position.z);
    }
    // padding
    double diag = (max - min).length();
    min += Vec3(-eps*diag, -eps*diag, -eps*diag);
    max += Vec3(eps*diag, eps*diag, eps*diag);
   
    Node* root = new Node(min, max - min); // create root node
    // insert particles into octree
    for (Particle& p : particles) {  
        root->insert(&p);
    }  
    return root;
}   

// explicit euler time step
void Integrator::step_euler(std::vector<Particle>& particles, double dt) {
    #ifndef USE_NAIVE
    Node* root = build_tree(particles);
    #endif
    // update particles (could be parallelized)
    for (Particle& p : particles) {    
        #ifdef USE_NAIVE
        Vec3 force = compute_force_naive(&p, particles);
        #else
        Vec3 force = compute_force(&p, root);
        #endif
        p.velocity += force / p.mass * dt;
        p.position += p.velocity * dt;
    }
}

// leapfrog time step
void Integrator::step_leapfrog(std::vector<Particle>& particles, double dt) {
    #ifndef USE_NAIVE
    Node* root = build_tree(particles);
    #endif
    std::vector<Vec3> accelerations(particles.size());
    // compute accelerations
    for (size_t i = 0; i < particles.size(); ++i) {
        Vec3 f;
        #ifdef USE_NAIVE
        f = compute_force_naive(&particles[i], particles);
        #else
        f = compute_force(&particles[i], root);
        #endif
        accelerations[i] = f / particles[i].mass;
    }
    // half-step velocities
    for (size_t i = 0; i < particles.size(); ++i) {
        particles[i].velocity += 0.5 * dt * accelerations[i];
    }
    // full step position update
    for (Particle& p : particles) {
        p.position += dt * p.velocity;
    }
    // rebuild tree
    #ifndef USE_NAIVE
    delete root; // free memory
    root = build_tree(particles);
    #endif
    // compute accelerations again
    for (size_t i = 0; i < particles.size(); ++i) {
        #ifdef USE_NAIVE
        Vec3 f = compute_force_naive(&particles[i], particles);
        #else
        Vec3 f = compute_force(&particles[i], root);
        #endif
        accelerations[i] = f / particles[i].mass;
    }
    // half-step velocities
    for (size_t i = 0; i < particles.size(); ++i) {
        particles[i].velocity += 0.5 * dt * accelerations[i];
    }
    #ifndef USE_NAIVE
    delete root;
    #endif
}

#endif // INTEGRATOR_H