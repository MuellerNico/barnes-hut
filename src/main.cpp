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
    std::vector<Particle> particles = ioHandler.read_snapshot("input/jpl_horizons/0.bin"); // Global vector of particles (system state)
    std::vector<Particle> particles_end_state = ioHandler.read_snapshot("input/jpl_horizons/25.bin");

    double t = 0; // time [yr]
    double T = 25; // final time [yr]
    int num_steps = (T - t) / dt + 1; // number of time steps

    std::cout << "num_steps=" << num_steps << std::endl;
    std::cout << "dt=" << dt << std::endl << std::endl;

    for (int i = 0; i < num_steps; ++i) {
        Node* root = step(particles, dt);
        t += dt;
        if (i % (num_steps / 25) == 0) {
            int f = i / (num_steps / 25);
            // std::cout << "Progress: " << f << "%, t=" << t << " yr" << std::endl;
            // ioHandler.write_snapshot(f, particles, t); // write snapshot to file
            // ioHandler.write_tree(root, f); // write tree to file
            std::string filename = "output/snapshots/" + std::to_string(f) + ".bin";
            ioHandler.write_snapshot(filename, particles); 
        }
        delete root; // free memory
    }
    
    std::cout << "Used " << com_approx << " center of mass approximations" << std::endl;

    return 0;
}
