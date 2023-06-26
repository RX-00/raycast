/*
    Class Renderer to render the (pseudo) 3D view of the world based
    on the raycasting method
*/

#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <cstring>

class Renderer {
  private:
    std::vector<sf::Uint8> pixel_buffer;
    std::vector<sf::Vertex> line; // line made from vertices
    sf::Texture texture;
    sf::RectangleShape sprite;

  public:
    Renderer();
    void renderLine(sf::RenderTarget& target,
                    const sf::Vector2f& begin,
                    const sf::Vector2f& end,
                    sf::Color color_);
    void render(sf::RenderTarget& target);
    void setPixels(int x_pos, int y_pos, sf::Color color_);
    void resetPixels(); // clear all the pixels on screen
};