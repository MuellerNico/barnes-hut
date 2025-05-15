#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#include <iostream>
#include "node.h"
#include "particle.h"
#include "const.h"

// compute force acting on particle p1 from a body (particle or node)
Vec3 force(Vec3 p1, double m1, Vec3 p2, double m2) {
    Vec3 dist = p2 - p1;
    double r2 = dist.length2();
    return dist.normalized() * (G * m1 * m2 / (r2 + eps2)); // F = G * m1 * m2 / r^2
}

// force acting on particle p from a node/quad
Vec3 compute_force(Particle* p, Node* node) {
    Vec3 f = {0, 0, 0};
    // external node, calculate force from particles (usually 1 body per leaf)
    if (node->is_leaf()) {
        for (Particle* other : node->particles) {
            if (other != p) { // avoid self-interaction
                f += force(p->position, p->mass, other->position, other->mass);
                if ((p->position - other->position).length() < p->radius + other->radius) {
                    std::cerr << "Collision." << std::endl;
                }
            }
        }
    } else { // internal node
        // check if far enough away
        double d = (node->center_of_mass - p->position).length();
        double s = node->size.x;
        if (s / (d + eps) < theta) { 
            // use center of mass approximation
            f += force(p->position, p->mass, node->center_of_mass, node->mass);    
        } else {
            // recursively traverse children
            for (Node* child : node->children) { 
                f += compute_force(p, child);
            }
        }
    }
    return f;
}

// O(N^2) naive implementation for benchmarking
Vec3 compute_force_naive(Particle* p, const std::vector<Particle>& particles) {
    Vec3 f = {0, 0, 0};
    for (const Particle& other : particles) {
        if (&other != p) { // avoid self-interaction
            f += force(p->position, p->mass, other.position, other.mass);
        }
    }
    return f;
}

// build octree from particles
Node* build_tree(std::vector<Particle>& particles) {
    // find bounding box
    Vec3 min = {0, 0, 0}; // note: zero works here because sun is at origin
    Vec3 max = {0, 0, 0};
    for (const Particle& p : particles) {
        min.x = std::min(min.x, p.position.x);
        min.y = std::min(min.y, p.position.y);
        min.z = std::min(min.z, p.position.z);
        max.x = std::max(max.x, p.position.x);
        max.y = std::max(max.y, p.position.y);
        max.z = std::max(max.z, p.position.z);
    }
    min += Vec3(-eps, -eps, -eps); // add some padding
    max += Vec3(eps, eps, eps);
   
    Node* root = new Node(min, max - min); // create root node
    // insert particles into octree
    for (Particle& p : particles) {   
        root->insert(&p);
    }  
    return root;
}

// explicit euler time step
Node* step_euler(std::vector<Particle>& particles, double dt) {
    Node* root = build_tree(particles); // build tree
    // update particles TODO: parallel?
    for (Particle& p : particles) {    
        #ifdef USE_NAIVE
        Vec3 force = compute_force_naive(&p, particles);
        #else
        Vec3 force = compute_force(&p, root);
        #endif
        p.velocity += force / p.mass * dt;
        p.position += p.velocity * dt;
    }
    return root;
}

Node* step_leapfrog(std::vector<Particle>& particles, double dt) {
    Node* root;
    #ifndef USE_NAIVE
    root = build_tree(particles); // build tree
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
    delete root; // free memory
    root = build_tree(particles);
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
    return root; // mby use smart pointer
}

#endif // INTEGRATOR_H