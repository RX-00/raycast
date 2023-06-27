/*
    Implementation of the Renderer class
*/

#include "Renderer.h"
#include "Configs.h"

#define BUFFER_SCALE 4

// initializing our member variables
Renderer::Renderer() : pixel_buffer(WINDOW_HEIGHT * WINDOW_WIDTH * BUFFER_SCALE) {
    line.emplace_back(sf::Vector2f{0,0}, sf::Color::Red);
    line.emplace_back(sf::Vector2f{0,0}, sf::Color::Red);
    texture.create(WINDOW_WIDTH, WINDOW_HEIGHT);
    sprite.setSize({WINDOW_WIDTH, WINDOW_HEIGHT}); // force to convert to correct type
}

void Renderer::renderLine(sf::RenderTarget& target,
                          const sf::Vector2f& begin,
                          const sf::Vector2f& end,
                          sf::Color color) {
    line[0].position = begin;
    line[1].position = end;
    
    line[0].color = color;
    line[1].color = color;

    target.draw(line.data(), 2, sf::PrimitiveType::LinesStrip);
}

void Renderer::render(sf::RenderTarget& target) {
    sprite.setTexture(&texture);
    texture.update(pixel_buffer.data());
    target.draw(sprite);
}

void Renderer::setPixels(int x_pos, int y_pos, sf::Color color) {
    if (x_pos < 0 || x_pos >= WINDOW_WIDTH ||
        y_pos < 0 || y_pos >= WINDOW_HEIGHT) {
        return;
    }
    sf::Uint8* ptr = &pixel_buffer.at((y_pos * WINDOW_WIDTH + x_pos) * 4);
    ptr[0] = color.r;
    ptr[1] = color.g;
    ptr[2] = color.b;
    ptr[3] = 255;
}

void Renderer::resetPixels() {
    std::memset(pixel_buffer.data(), 0, pixel_buffer.size());
}