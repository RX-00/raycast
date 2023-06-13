#!/bin/bash

cd ~/Documents/raycast
cd build

# cleaning
cmake --build . --target clean

# rebuild
cmake ..
cmake --build .

# run the program
./raycast