#include "node.h"
#include "ioHandler.h"

// TODOs
// - validate model 
// - benchmark tree vs naive
// - delete if collided

// units
constexpr double AU = 1.496e11; // L: 1 AU in meters
constexpr double M_sun = 1.989e30; // M: M_sun mass of sun in kg
constexpr double year = 3.154e7; // T: 1 year in seconds
constexpr double G = 4 * M_PI * M_PI; // gravitational constant in AU^3 / (M_sun * year^2)

constexpr int numParticles = 50; 
constexpr double theta = 0.5; // threshold. using center of mass approximation if s/d < theta. If zero, degenerates to brute force
constexpr double dt = 0.01; // time step
constexpr double eps = 1e-8; // small value to avoid division by zero
constexpr double eps2 = eps * eps; // for force calculation

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
                // TODO: handle collisions
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

// explicit euler time step (mby est other schemes)
Node* step(std::vector<Particle>& particles, double dt) {
    Node* root = new Node(Vec3(0, 0, 0), Vec3(100, 100, 100)); // new tree every step
    // insert particles into quadtree
    for (Particle& p : particles) {   
        root->insert(&p);
    }  
    // update particles TODO: parallel?
    for (Particle& p : particles) {    
        Vec3 force = compute_force(&p, root);
        p.velocity += force / p.mass * dt;
        p.position += p.velocity * dt;
    }
    return root; // careful memory leak (should delete here, find solution for tree output)
}

int main() {
    srand(0); // seed random number generator
    IOHandler ioHandler; // IO handler for writing output
    std::vector<Particle> particles; // Global vector of particles (system state)
    particles.reserve(numParticles); // reserve space for particles
    
    std::cout << "G=" << G << std::endl;

    // init
    for (int i = 0; i < numParticles; ++i) {
        Particle p;
        p.position = Vec3(rand() % 100, rand() % 100, rand() % 100);
        Vec3 rand_dir = Vec3(rand(), rand(), rand()).normalized() * 2 - Vec3(1, 1, 1); // random direction
        double speed = rand() % 10 + 20; // random speed
        p.velocity = rand_dir * speed; // random direction
        p.mass = rand() % 5 + 1;
        p.radius = p.mass;
        particles.push_back(p);
    }

    double t = 0; // time
    for (int f = 0; f < 100; ++f) {
        Node* root = step(particles, dt);
        ioHandler.write_snapshot(f, particles, t);
        ioHandler.write_tree(root, f);
        t += dt;
        delete root; // free memory
    }
    
    return 0;
}
