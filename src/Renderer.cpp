/*
    Implementation of the Renderer class
*/

#include "Renderer.h"
#include "Configs.h"

#define BUFFER_SCALE 4

// initializing our member variables
Renderer::Renderer() : pixel_buffer(WINDOW_HEIGHT * WINDOW_WIDTH * BUFFER_SCALE) {
    for (int i = 0; i < 2; i++) {
        line.emplace_back(sf::Vector2f{0,0}, sf::Color::Black);
    }

    sprite.setSize({WINDOW_WIDTH, WINDOW_HEIGHT}); // force to convert to correct type
    texture.create(WINDOW_WIDTH, WINDOW_HEIGHT);
}

void Renderer::renderLine(sf::RenderTarget& target,
                          const sf::Vector2f& begin,
                          const sf::Vector2f& end,
                          sf::Color color_) {
    // first vertex in the line
    line[0].position = begin;
    line[0].color = color_;

    // second vertex in the line
    line[1].position = begin;
    line[1].color = color_;

    target.draw(line.data(), 2, sf::PrimitiveType::Lines);
}

void Renderer::render(sf::RenderTarget& target) {
    sprite.setTexture(&texture);
    texture.update(pixel_buffer.data());
    target.draw(sprite);
}

void Renderer::setPixels(int x_pos, int y_pos, sf::Color color_) {
    if (x_pos < 0 || x_pos >= WINDOW_WIDTH ||
        y_pos < 0 || y_pos >= WINDOW_HEIGHT) {
        return;
    }
    sf::Uint8* ptr = &pixel_buffer.at((x_pos + (y_pos * WINDOW_WIDTH)) * 4);
    ptr[3] = 255;
    ptr[0] = color_.r;
    ptr[1] = color_.g;
    ptr[2] = color_.b;
}

void Renderer::resetPixels() {
    std::memset(pixel_buffer.data(), 0, pixel_buffer.size());
}