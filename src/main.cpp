#include "tree.h"
#include "ioHandler.h"

// TODOs
// - correct scaling of masses, G etc
// - validate model 
// - benchmark tree vs naive
// - delete if collided

constexpr int numParticles = 50; 
constexpr double theta = 0.5; // threshold. using center of mass approximation if s/d < theta. If zero, degenerates to brute force
constexpr double dt = 0.01; // time step
constexpr double G = 1.0; // gravitational constant
    
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
    if (node->particles.size() > 0) { // external node, calculate force from particles (usually 1 body per leaf)
        for (Particle* other : node->particles) {
            if (other != p) { // avoid self-interaction
                f += force(p->position, p->mass, other->position, other->mass);
                // maybe handle collisions
            }
        }
    } else {
        // check if far enough away
        double d = (node->center_of_mass - p->position).length();
        double s = node->size.x;
        if (s / d < theta) { 
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
Vec2 compute_force_naive(Particle* p, const std::vector<Particle>& particles) {
    Vec2 f = {0, 0};
    for (const Particle& other : particles) {
        if (&other != p) { // avoid self-interaction
            f += force(p->position, p->mass, other.position, other.mass);
        }
    }
    return f;
}

// explicit euler time step (mby est other schemes)
Node* step(std::vector<Particle>& particles, double dt) {
    Node* root = new Node(Vec2(0, 0), Vec2(100, 100)); // new tree every step
    // insert particles into quadtree
    for (Particle& p : particles) {   
        root->insert(&p);
    }  
    // update particles
    for (Particle& p : particles) {    
        Vec2 force = compute_force(&p, root);
        p.velocity += force / p.mass * dt;
        p.position += p.velocity * dt;
    }
    return root; // careful memory leak (should delete here, find solution for tree output)
}

int main() {
    std::vector<Particle> particles; // Global vector of particles (system state)
    for (int i = 0; i < numParticles; ++i) {
        Particle p;
        p.position = Vec2(rand() % 100, rand() % 100);
        p.velocity = Vec2(rand() % 10, rand() % 10) - Vec2(5, 5);
        p.mass = rand() % 5 + 1;
        p.radius = p.mass;
        p.id = i;
        particles.push_back(p);
    }

    // std::vector<Particle> particles = ioHandler::read_input("input/solar_system.csv");
   
    for (int f = 0; f < 100; ++f) {
        Node* root = step(particles, dt);
        ioHandler::write_snapshot(particles, f);
        ioHandler::write_tree(root, f);
        delete root; // free memory
    }
    
    return 0;
}
