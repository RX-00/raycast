/*
    Implementation of the Map class
*/

#include "Map.h"


Map::Map() {
    std::ifstream inFile("content/map.txt");
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