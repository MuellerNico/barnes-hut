#include "node.h"
#include "ioHandler.h"
#include "integrator.h"
#include "const.h"

// TODOs
// - validate model 
// - benchmark tree vs naive
// - delete if collided

int main() {
    srand(0); // seed random number generator
    IOHandler ioHandler; // IO handler for writing output
    std::vector<Particle> particles; // Global vector of particles (system state)

    particles = ioHandler.read_snapshot("input/snapshots/2460676.5.bin");
    
    std::cout << "Particles: " << particles.size() << std::endl;
    for (const Particle& p : particles) {
        std::cout << p.position.x << " " << p.position.y << " " << p.position.z << std::endl;
    }
    return 0;
    
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
