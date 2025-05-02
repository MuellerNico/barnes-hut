#include "tree.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <queue>

constexpr int numParticles = 10; 
constexpr double theta = 0.5; // threshold. using center of mass approximation if s/d < theta. If zero, degenerates to brute force
constexpr double dt = 0.01; // time step
constexpr double G = 1.0; // gravitational constant ToDo: correctly scale masses and distances

void write_snapshot(const std::vector<Particle>& particles, int f) {
    std::ofstream file("out/frame_" + std::to_string(f) + ".csv");
    file << "t,particle_id,pos_x,pos_y,vel_x,vel_y,mass,radius" << std::endl;
    for (const Particle& p : particles) {
        file << f << "," << p.id << "," << p.position.x << "," << p.position.y << ","
             << p.velocity.x << "," << p.velocity.y << "," << p.mass << "," << p.radius << std::endl;
    }
    file.close();
}

void write_tree(Node* root, int f) {
    std::ofstream file("out/tree_" + std::to_string(f) + ".csv");
    file << "pos_x,pos_y,size_x,size_y,center_of_mass_x,center_of_mass_y,mass" << std::endl;
    std::queue<Node*> queue;
    queue.push(root);
    while (!queue.empty()) {
        Node* node = queue.front();
        queue.pop();
        // only write external nodes
        file << node->position.x << "," << node->position.y << ","
                << node->size.x << "," << node->size.y << ","
                << node->center_of_mass.x << "," << node->center_of_mass.y << ","
                << node->mass << std::endl;    
        // enqueue children
        for (Node* child : node->children) {
            queue.push(child);
        }
    }
    file.close();
}
    
// compute force acting on particle p1 from a body (particle or node)
Vec2 force(Vec2 p1, double m1, Vec2 p2, double m2) {
    Vec2 dist = p2 - p1;
    double r = dist.length();
    if (r == 0) return Vec2(0, 0); // avoid division by zero
    return dist.normalized() * (G * m1 * m2 / (r * r)); // F = G * m1 * m2 / r^2
}

// force acting on particle p from a node/quad
Vec2 compute_force(Particle* p, Node* node) {
    Vec2 f = {0, 0};
    if (node->particles.size() > 0) { // external node, calculate force from particles (should maybe be only 1?)
        for (Particle* other : node->particles) {
            if (other != p) { // avoid self-interaction
                f += force(p->position, p->mass, other->position, other->mass);
            }
        }
    } else {
        double d = (node->center_of_mass - p->position).length();
        double s = node->size.x;
        if (s / d < theta) { // use center of mass approximation
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

void step(std::vector<Particle>& particles, double dt, int f) {
    Node* root = new Node(Vec2(0, 0), Vec2(100, 100)); // new tree every step
    // insert particles into quadtree
    for (Particle& p : particles) {   
        root->insert(&p);
    }  
    write_tree(root, f);  
    // update particles
    for (Particle& p : particles) {    
        Vec2 force = compute_force(&p, root);
        p.velocity += force / p.mass * dt;
        p.position += p.velocity * dt;
    }
    // cleanup
    delete root;    
}

int main() {

    std::vector<Particle> particles; // Global vector of particles (system state)
    for (int i = 0; i < numParticles; ++i) {
        Particle p;
        p.position = Vec2(rand() % 100, rand() % 100);
        p.velocity = Vec2(rand() % 10, rand() % 10);
        p.mass = 1.0;
        p.radius = 1.0;
        p.id = i;
        particles.push_back(p);
    }

    // std::cout << "Test" << std::endl;

    for (int f = 0; f < 100; ++f) {
        step(particles, dt, f);
        write_snapshot(particles, f);
    }

    return 0;
}
