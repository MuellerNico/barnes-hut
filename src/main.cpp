#include "node.h"
#include "ioHandler.h"
#include "integrator.h"
#include "const.h"
#include <sys/time.h>

int main() {
    IOHandler ioHandler; // IO handler for writing output
    Integrator integrator;
    std::vector<Particle> particles = ioHandler.read_snapshot("input/gaia_stars_49992.bin");
    // std::vector<Particle> particles = ioHandler.read_snapshot("input/jpl_horizons/0.bin"); 
    // particles.resize(5000); // limit count

    double t = 0; // time [yr]
    double T = 1; // final time [yr]
    int num_steps = 5; // (T - t) / dt + 1; // number of time steps
    
    for (int i = 0; i < num_steps; ++i) {
        integrator.step_leapfrog(particles, dt);
        t += dt;
        ioHandler.write_frame(i, particles, t); // write frame to csv
        std::cout << "Frame " << i << " written to file" << std::endl;
    }

    return 0;
}
