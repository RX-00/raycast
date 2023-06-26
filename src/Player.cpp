/*
    Implementation of Player class
*/

#include "Player.h"


Player::Player() {
    playerModel.setRadius(5.0f);
    x_spd = std::cos(angle);
    y_spd = std::sin(angle);
}

void Player::execInput(const KeyInputs& keys) {
    float angle_move = conv_deg_to_rad(angle);

    if (keys.isKeyPress(sf::Keyboard::W)) { // W move forward
        x_pos += x_spd * PLAYER_SPEED;
        y_pos += y_spd * PLAYER_SPEED;
    }

    if (keys.isKeyPress(sf::Keyboard::A)) { // A turn left
        angle -= PLAYER_SPEED;
        angle = wrap_around_deg(angle);
        x_spd = std::cos(angle_move);
        y_spd = std::sin(angle_move);
    }

    if (keys.isKeyPress(sf::Keyboard::S)) { // S move backward
        x_pos -= x_spd * PLAYER_SPEED;
        y_pos -= y_spd * PLAYER_SPEED;
    }

    if (keys.isKeyPress(sf::Keyboard::D)) { // D turn right
        angle += PLAYER_SPEED;
        angle = wrap_around_deg(angle);
        x_spd = std::cos(angle_move);
        y_spd = std::sin(angle_move);
    }

    if (keys.isKeyPress(sf::Keyboard::Q)) { // quit game
        std::cout << "Goodbye!" << std::endl;
        exit(0);
    }
}

void Player::draw(sf::RenderTexture& window) {
    playerModel.setPosition(x_pos / MINI_MAP_SCALE - 5,
                            y_pos / MINI_MAP_SCALE - 5);
    window.draw(playerModel);
}

float Player::getAngle() {
    return angle;
}

float Player::getRayAngle() {
    return wrap_around_deg(getAngle() - FOV / 2);
}

float Player::getX() {
    return x_pos;
}

float Player::getY() {
    return y_pos;
}