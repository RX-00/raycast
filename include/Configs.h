/*
    Constants and configurations for the raycast program.

    This includes parameters such as the window size, minimap size, tile size,
    speed of the player, FOV of the player, etc.
*/

// This #pragma once is needed so that during compilation time we don't get
// errors of redefinitions of the const variables
#pragma once

// Player and game constants
const float PI = 3.1415927;
const float PLAYER_SPEED = 2.0;
const int FOV = 60; // seems to be a common practice to set FOV to 60 as default

// Display and map constants
const int WINDOW_WIDTH = 1280;  // 720p resolution
const int WINDOW_HEIGHT = 720;
const int MAP_SIZE = 20;
const int TILE_SIZE = 64;
const int MINI_MAP_SCALE = 4;
const int MINI_MAP_TILE_SIZE = TILE_SIZE / MINI_MAP_SCALE;