# barnes-hut
C++ implementation of the Barnes-Hut algorithm for N-body simulations. Project work for 402-0812-00L Computational Statistical Physics FS2025, ETH Zürich.

## Usage
To compile the C++ software use the provided makefile

    $ make
    $ ./a.out

The default setup integrates the solar system over 25 years. 

To test the benchmarking program, change the `SRC` inside the makefile from `main.cpp` to `benchmark.cpp`.  

To enable naive $O(N^2)$ force computation  uncomment `#define USE_NAIVE` in `const.h`. When using data from the Gaia Archive make sure to change the gravitational constant there as well to account for the different length scales (parsecs instead of AU).

## Directory Structure
`root`: README, makefile, shell run script, python plotting scripts  
`/input`: Simulation input data (JPL Horizons & Gaia Archive data, python query scripts)  
`/output`: Simulation output data (benchmarking data, binary snapshots, CSV frames for animation, tree metadata)  
`/results`: Plots and animations  
`/src`: C++ code  

## Output
Three types of output files can be written by the `IOHandler` to `/output`: 
- `data.csv` contains the position, velocity, mass and radius for each body at different points in time used for debug and/or to visualize the system
- `tree.csv` contains the octree metadata, storing every node with its position, size, center of mass and mass to inspect, debug and visualize the tree at different points in time
- binary snapshots (`.bin`): more efficient format to store particle data for evaluation that does not require human readability (verification, performance study, etc)

## Python requirements
pandas
matplotlib
numpy
astropy
astroquery