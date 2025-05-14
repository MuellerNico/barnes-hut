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
    std::vector<Particle> particles_end_state;
    particles = ioHandler.read_snapshot("input/snapshots/2460676.5.bin"); // read snapshot from file
    // only take the first 5 particles
    particles.resize(5);
    ioHandler.print(particles); 
    // particles_end_state = ioHandler.read_snapshot("input/snapshots/2460686.5.bin"); // read snapshot from file
    std::cout << "G=" << G << std::endl;
    std::cout << "Please ensure that input data uses correct units (AU, yr, M_sun)" << std::endl;

    double t = 0; // time
    for (int f = 0; f < 365; ++f) {
        std::cout << "f=" << f << std::endl;
        Node* root = step(particles, dt);
        ioHandler.write_snapshot(f, particles, t);
        ioHandler.write_tree(root, f);
        t += dt;
        delete root; // free memory
    }
    
    return 0;
}
