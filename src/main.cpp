#include "tree.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <queue>

constexpr int numParticles = 10; 
constexpr double theta = 0.5; // threshold. using center of mass approximation if s/d < theta. If zero, degenerates to brute force

void write_snapshot(std::vector<Particle*> particles, int f) {
    std::ofstream file("out/frame_" + std::to_string(f) + ".csv");
    file << "t,particle_id,pos_x,pos_y,vel_x,vel_y,mass,radius" << std::endl;
    for (Particle* p : particles) {
        file << f << "," << p->id << "," << p->position.x << "," << p->position.y << ","
             << p->velocity.x << "," << p->velocity.y << ","
             << p->mass << "," << p->radius << std::endl;
    }
    file.close();
}

Vec2 compute_force(Particle* p, Node* node) {
    Vec2 force = {0, 0};
    if (node->particles.size() > 0) { // external node, calculate force from particles (should maybe be only 1?)
        for (Particle* other : node->particles) {
            if (other != p) { // avoid self-interaction
                Vec2 direction = (other->position - p->position).normalized();
                double d = (other->position - p->position).length();
                force += direction * (other->mass / d / d); // TODO: formula
            }
        }
    } else {
        double d = (node->center_of_mass - p->position).length();
        double s = node->size.x;
        if (s / d < theta) { // use center of mass approximation
            Vec2 direction = (node->center_of_mass - p->position).normalized();
            force += direction * (node->mass / d / d); // TODO: formula
        } else {
            for (Node* child : node->children) { // recursively traverse children
                force += compute_force(p, child);
            }
        }
    }
    return force;
}

void step(std::vector<Particle*>& particles, double dt) {
    Node* root = new Node(Vec2(0, 0), Vec2(100, 100)); // new tree every step
    for (Particle* p : particles) {    // insert particles into quadtree
        root->insert(p);
    }    
    for (Particle* p : particles) {    // update particles
        Vec2 force = compute_force(p, root);
        p->velocity += force / p->mass * dt;
        p->position += p->velocity * dt;
    }
    // cleanup
    delete root;    
}

int main() {

    std::vector<Particle*> particles; // could omit pointers here
    for (int i = 0; i < numParticles; ++i) {
        Particle* p = new Particle();
        p->position = Vec2(rand() % 100, rand() % 100);
        p->velocity = Vec2(0, 0);
        p->mass = 1.0;
        p->radius = 1.0;
        p->id = i;
        particles.push_back(p);
    }

    std::cout << "Test" << std::endl;

    return 0;
}
