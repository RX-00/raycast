/*
    Map class. The map is represented by a vector of integers, the default 
    of which is stored in a map.txt file.

    Each element in the map vector is known as a tile.

    The member methods are to get & set the current tile in the map.
*/

#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include "Configs.h"


class Map {
  private:
    std::vector<int> map; // constructed from a text file where 0 -> no block, !0 -> block
  
  public:
    Map();
    int getTile(int x_pos, int y_pos) const;
    void setTile(int x_pos, int y_pos, int tile);
};