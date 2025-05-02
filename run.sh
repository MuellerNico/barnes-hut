#!/bin/bash

echo "Compiling..."
make clean
make 
echo "Running..."
./a.out
echo "Plotting..."
python3 plot.py
echo "Done."