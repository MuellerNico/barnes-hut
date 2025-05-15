#include <sys/time.h>
#include "node.h"
#include "ioHandler.h"
#include "integrator.h"
#include "const.h"

double walltime(){
    struct timeval time;
    if (gettimeofday(&time, NULL)) {
        std::cout << "Unexpected error in walltime" << std::endl;
        return 0;
    }
    return (double)time.tv_sec + (double)time.tv_usec * .000001;
}

int main() {
    IOHandler ioHandler;
    std::vector<Particle> particles = ioHandler.read_snapshot("input/gaia_stars_49992.bin");
    std::vector<int> num_particles = {100, 500, 1000, 2000, 3000, 5000, 7000, 10000, 15000, 20000, 25000, 30000, 35000, 40000, 45000, 49992}; // sample sizes
    int num_steps = 10;

    std::ofstream benchmark;
    benchmark.open("output/benchmark.csv", std::ios::app); 
    if (!benchmark.is_open()) {
        std::cerr << "Error opening file" << std::endl;
    }
        
    for (int n : num_particles) {
        std::cout << "Benchmarking with N=" << n << std::endl;
        particles.resize(n);
        
        step_leapfrog(particles, dt); // warm up

        for (int i = 0; i < num_steps; ++i) {

            double start_time = walltime(); 
            step_leapfrog(particles, dt);
            double end_time = walltime(); 

            std::string mode = "octree8"; // mode
            #ifdef USE_NAIVE
            mode = "naive";
            #endif
            benchmark << n << "," << mode << "," << end_time - start_time << std::endl;
            benchmark.flush(); 
        }
    }
    benchmark.close();
    return 0;
}
