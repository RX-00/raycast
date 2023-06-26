/*
    Implementation of the KeyInputs class
*/

#include "KeyInputs.h"


KeyInputs::KeyInputs() {
    std::fill(key_state_map.begin(), key_state_map.end(), false);
}

void KeyInputs::update(sf::Event e) {
    switch (e.type) {
    case sf::Event::KeyReleased:
        key_state_map.at(e.key.code) = false;
        break;
    
    case sf::Event::KeyPressed:
        key_state_map.at(e.key.code) = true;
        break;
    
    default:
        break;
    }
}

bool KeyInputs::isKeyPress(sf::Keyboard::Key key) {
    return key_state_map.at(key);
}

void KeyInputs::resetKeyUnpress(){
    std::fill(key_state_map.begin(), key_state_map.end(), false);
}