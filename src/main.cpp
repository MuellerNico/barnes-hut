#include "node.h"
#include "ioHandler.h"
#include "integrator.h"
#include "const.h"
#include <sys/time.h>

int main() {
    IOHandler ioHandler; // IO handler for writing output
    Integrator integrator;
    std::vector<Particle> particles = ioHandler.read_snapshot("input/jpl_horizons/0.bin"); 
    // std::vector<Particle> particles = ioHandler.read_snapshot("input/gaia_stars_49992.bin");
    // particles.resize(5000); // limit count (make sure to not accidentally expand the vector)

    double t = 0; // time [yr]
    double T = 25; // final time [yr]
    int num_steps = (T - t) / dt; // number of time steps
    
    for (int i = 0; i < num_steps; ++i) {
        integrator.step_leapfrog(particles, dt);
        t += dt;
        if (i % (num_steps / 100) == 0) {
            int f = i / (num_steps / 100); // frame number
            std::cout << "Progress: " << f << "%" << std::endl;
            ioHandler.write_frame(f, particles, t);
            ioHandler.write_snapshot("output/snapshots/" + std::to_string(f) + ".bin", particles);
        }
    }

    return 0;
}
