#include "node.h"
#include "ioHandler.h"
#include "integrator.h"
#include "const.h"
#include <sys/time.h>

int main() {
    IOHandler ioHandler; // IO handler for writing output

    std::vector<Particle> particles = ioHandler.read_snapshot("input/gaia_stars_49992.bin"); // Global vector of particles (system state)
    
    // particles.resize(20000); // resize particles to numParticles
    // std::vector<Particle> particles = ioHandler.read_snapshot("input/jpl_horizons/0.bin"); // Global vector of particles (system state)
    // std::vector<Particle> particles_end_state = ioHandler.read_snapshot("input/jpl_horizons/25.bin");

    double t = 0; // time [yr]
    double T = 1; // final time [yr]
    int num_steps = 5; // (T - t) / dt + 1; // number of time steps

    
    for (int i = 0; i < num_steps; ++i) {
        step_leapfrog(particles, dt);
        t += dt;

        // if (i % (int(1.0 * num_steps / 100)) == 0) {
        //     int f = i / int(1.0 * num_steps / 100);
        //     std::cout << "Progress: " << f << "%, t=" << t << " yr" << std::endl;
        //     // ioHandler.write_frame(f, particles, t); // write snapshot to file
        //     // ioHandler.write_tree(root, f); // write tree to file
        //     // std::string filename = "output/snapshots/" + std::to_string(f) + ".bin";
        //     // ioHandler.write_snapshot(filename, particles); 
        //     // ioHandler.print(particles); // print particles to console
        // }

    }


    std::cout << "\nParticles: " << particles.size() << std::endl;
    std::cout << "num_steps=" << num_steps << std::endl;
    std::cout << "dt=" << dt << std::endl;
    #ifdef USE_NAIVE
    std::cout << "Using naive N^2 force calculation" << std::endl;
    #else
    std::cout << "Using octree for force calculation" << std::endl;
    #endif

    return 0;
}
