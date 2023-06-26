#include <iostream>
#include <SFML/Graphics.hpp>
#include "KeyInputs.h"

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
        void execInput(const KeyInputs& keys); // do input from keyboard
};