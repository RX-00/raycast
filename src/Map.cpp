/*
    Implementation of the Map class
*/

#include "Map.h"
#include <cassert>


Map::Map() {
    std::ifstream inFile("map.txt"); // NOTE: unfortunately needs to be in build/
    if (inFile.fail()) {
        std::cerr << "ERR: Could not find map" << std::endl;
    }
    std::string line;
    map.reserve(MAP_SIZE * MAP_SIZE);
    while (std::getline(inFile, line)) {
        for (auto ch : line) {
            map.push_back(ch - '0');
        }
    }
    assert(map.size() == (MAP_SIZE * MAP_SIZE));
}

int Map::getTile(int x_pos, int y_pos) const {
    if (x_pos < 0 || x_pos > MAP_SIZE || 
        y_pos < 0 || y_pos > MAP_SIZE) { 
        return 0;
    }
    return map.at(y_pos * MAP_SIZE + x_pos);
}

void Map::setTile(int x_pos, int y_pos, int tile) {
    if (x_pos < 0 || x_pos > MAP_SIZE || 
        y_pos < 0 || y_pos > MAP_SIZE) {
        return;
    }
    map.at(y_pos * MAP_SIZE + x_pos) = tile;
}