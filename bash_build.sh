#!/bin/bash

cd ~/Documents/raycast
cp content/map.txt build/map.txt
cd build

# cleaning
cmake --build . --target clean

# rebuild
cmake ..
cmake --build .

# run the program
./raycast