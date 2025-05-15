# barnes-hut
C++ implementation of the Barnes-Hut algorithm for N-body problems. Project work for 402-0812-00L Computational Statistical Physics FS2025, ETH Zürich.

## How to run
To compile and run the C++ software use the shell script (or the makefile and run the executable separately)
    $ sh run.sh

## Output
Three forms of output files are written to `/output`: 
- `data.csv` contains the position, velocity, mass and radius for each body at different points in time used for debug and/or to visualize the system
- `tree.csv` contains the octree, storing every node with its position, size, center of mass and mass to inspect, debug and visualize the tree at different points in time
- binary snapshots (`.bin`): more efficient format to store particle data for evaluation that does not require human readability (verification, performance study, etc)

## Special requirements
astropy
astroquery