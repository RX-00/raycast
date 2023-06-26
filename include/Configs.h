/*
    Constants and configurations for the raycast program.

    This includes parameters such as the window size, minimap size, tile size,
    speed of the player, FOV of the player, etc.
*/

// This #pragma once is needed so that during compilation time we don't get
// errors of redefinitions of the const variables
#pragma once

// Player and game constants
const double PI = 3.141592653589793;
const float PLAYER_SPEED = 2.5;
const int FOV = 60; // seems to be a common practice to set FOV to 60 as default

// Display and map constants
const int WINDOW_WIDTH = 2048;  // 2K resolution | aspect ratio 1.90
const int WINDOW_HEIGHT = 1080;
const int MAP_SIZE = 20;
const int TILE_SIZE = 64;
const int MINI_MAP_SCALE = 4;
const int MINI_MAP_TILE_SIZE = TILE_SIZE / MINI_MAP_SCALE;