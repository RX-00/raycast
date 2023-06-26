/*
    Collection of helper functions that do small conversions and calculations
*/

#include <string>
#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "Configs.h"


// Convert degrees to radians
static float conv_deg_to_rad(float degrees) {
    return degrees * PI / 180.0f;
}

// Determine the angle in degrees, catching cases of wrap around
// i.e. when the angle is not within [0, 360]
static float wrap_around_deg(float angle) {
    if (angle < 0)
        return angle + 360.0f;
    else if (angle > 360)
        return angle - 360.0f;
    else
        return angle;
}

// Calculate the distance between two 2D points
// uses the general distance formulat of sqrt(x^2 + y^2)
static float calc_dist(const sf::Vector2f& vec_a, sf::Vector2f& vec_b) {
    float delta_x = vec_a.x - vec_b.x;
    float delta_y = vec_a.y - vec_b.y;
    return std::sqrt(std::pow(delta_x, 2) + std::pow(delta_y, 2));
}