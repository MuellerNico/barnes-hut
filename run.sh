#!/bin/bash

echo "Compiling..."
make clean
make 
echo "Running..."
./a.out
echo "Done."