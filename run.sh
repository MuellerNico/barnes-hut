#!/bin/bash

echo "Compiling..."
make clean
make 
echo "Running the simulation..."
./a.out
echo "Plotting..."
python3 plot.py
echo "Done."