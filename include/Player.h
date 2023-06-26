/*
    Player class to represent the user's game character

    has the state variables of the player, and member methods of
    executing input from keyboard and drawing the player character on the
    mini map 
*/


#pragma once

#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "KeyInputs.h"
#include "Configs.h"

class Player {
    private:
        float x_pos;
        float y_pos;
        float angle; // orientation of the player
        float x_spd; // player velocity magnitude in x direction
        float y_spd; // player velocity magnitude in y direction
        sf::CircleShape playerModel; // character representation

    public:
        Player(); // constructor
        void execInput(const KeyInputs& keys); // do input from keyboard: WASD movement
        void draw(sf::RenderTexture& window);
        float getAngle();
        float getRayAngle();
        float getX();
        float getY();
};