/* 
    Class for keyboard input for the user to interact with the
    game demo
*/

// This #pragma once is needed so that during compilation time we don't get
// errors of redefinitions of the const variables
#pragma once

#include <array>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>


class KeyInputs {
  private:
    std::array<bool, sf::Keyboard::KeyCount> key_state_map;

  public:
    // constructor
    KeyInputs();

    // check key events and update state (every frame)
    void update(sf::Event e);
    
    // check if key pressed
    bool isKeyPress(sf::Keyboard::Key key);

    // reset keys back to unpressed state
    void resetKeyUnpress();
};