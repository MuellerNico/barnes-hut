#include "node.h"
#include "ioHandler.h"
#include "integrator.h"
#include "const.h"


int main() {
    srand(0); // seed random number generator
    IOHandler ioHandler; // IO handler for writing output

    std::vector<Particle> particles = ioHandler.read_snapshot("input/gaia_stars.bin"); // Global vector of particles (system state)
    std::cout << "Particles: " << particles.size() << std::endl;
    // limit particles
    particles.resize(100);

    // std::vector<Particle> particles = ioHandler.read_snapshot("input/jpl_horizons/0.bin"); // Global vector of particles (system state)
    // std::vector<Particle> particles_end_state = ioHandler.read_snapshot("input/jpl_horizons/25.bin");

    double t = 0; // time [yr]
    double T = 1; // final time [yr]
    int num_steps = (T - t) / dt + 1; // number of time steps

    std::cout << "num_steps=" << num_steps << std::endl;
    std::cout << "dt=" << dt << std::endl << std::endl;

    for (int i = 0; i < num_steps; ++i) {
        Node* root = step_leapfrog(particles, dt);
        t += dt;

        if (i % (num_steps / 100) == 0) {
            int f = i / (num_steps / 100);
            std::cout << "Progress: " << f << "%, t=" << t << " yr" << std::endl;
            ioHandler.write_frame(f, particles, t); // write snapshot to file
            ioHandler.write_tree(root, f); // write tree to file
            std::string filename = "output/snapshots/" + std::to_string(f) + ".bin";
            ioHandler.write_snapshot(filename, particles); 
        }
        delete root; // free memory
    }
    
    return 0;
}
