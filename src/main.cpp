// Standard System Directory Libraries
#include <sstream>
#include <string>
#include <iostream>
#include <cstdlib>
#include <cassert>
#include <cmath>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

// User System Directory Libraries
#include "HelperFxns.h"
#include "Renderer.h"
#include "Configs.h"
#include "KeyInputs.h"
#include "Player.h"
#include "Map.h"

void raycast() {
    // Setup the main window
    sf::RenderWindow window({WINDOW_WIDTH, WINDOW_HEIGHT}, "Raycast Demo");
    window.setFramerateLimit(60);
    window.setKeyRepeatEnabled(false);

    // Setup the minimap
    sf::RenderTexture mini_mapTexture;
    mini_mapTexture.create(MINI_MAP_TILE_SIZE * MAP_SIZE, 
                           MINI_MAP_TILE_SIZE * MAP_SIZE);
    sf::RectangleShape mini_mapSprite;
    mini_mapSprite.setSize({(float)MINI_MAP_TILE_SIZE * MAP_SIZE,
                            (float)MINI_MAP_TILE_SIZE * MAP_SIZE});
    sf::RectangleShape mini_mapTile;
    mini_mapTile.setSize({MINI_MAP_TILE_SIZE, MINI_MAP_TILE_SIZE});
    mini_mapTile.setFillColor(sf::Color::Black);
    mini_mapTile.setOutlineColor(sf::Color::White);
    mini_mapTile.setOutlineThickness(1.1);

    // Setup Renderer, map, and player
    Renderer drawRendererBuffer;
    Map map;
    Player player;

    // Main loop with keyboard input
    KeyInputs playerInput;
    while (window.isOpen()) {
        sf::Event e; // keyboard event
        while (window.pollEvent(e)) {
            playerInput.update(e);
            switch (e.type) {
            case sf::Event::Closed:
                window.close();
                break;

            // If player edits the mini map
            case sf::Event::MouseButtonReleased: {
                auto mouse_click = e.mouseButton;
                int x_pos = mouse_click.x;
                int y_pos = mouse_click.y;
                if (x_pos > 0 && x_pos < MAP_SIZE * MINI_MAP_TILE_SIZE &&
                    y_pos > 0 && y_pos < MAP_SIZE * MINI_MAP_TILE_SIZE) {
                    x_pos /= MINI_MAP_TILE_SIZE;
                    y_pos /= MINI_MAP_TILE_SIZE;
                    map.setTile(x_pos, y_pos, !(map.getTile(x_pos, y_pos)));
                }
                break;
            }
            
            default:
                break;
            }
        }
        // get player input
        player.execInput(playerInput);

        // update the window by clearing the previous frame
        window.clear();
        drawRendererBuffer.resetPixels();
        mini_mapTexture.clear(sf::Color::Transparent);

        /*
        
            RAYCASTING ALGORITHM
        
        */

        // calculate ray angle
        float ray_angle = player.getRayAngle();
        bool isLeft;

        // scan over the window width view of the player
        for (int i = 0; i < WINDOW_WIDTH; i++) {
            sf::Vector2f horiz_intersect;
            sf::Vector2f verti_intersect;
            sf::Vector2f init_horiz_intersect;
            sf::Vector2f init_verti_intersect;
            sf::Vector2f horiz_dist;
            sf::Vector2f verti_dist;
            sf::Vector2f horiz_next;
            sf::Vector2f verti_next;
            int horiz_gridX;
            int horiz_gridY;
            int verti_gridX;
            int verti_GridY;

            /*
                Horizonal scan
            */
            if (ray_angle < 180) {
                init_horiz_intersect.y = std::floor(player.getY() / TILE_SIZE) * TILE_SIZE + TILE_SIZE;
                horiz_dist.y = TILE_SIZE;
                horiz_dist.x = TILE_SIZE / std::tan(conv_deg_to_rad(ray_angle));
            } else {
                init_horiz_intersect.y = std::floor(player.getY() / TILE_SIZE) * TILE_SIZE - 1;
                horiz_dist.y = -TILE_SIZE;
                horiz_dist.x = TILE_SIZE / -std::tan(conv_deg_to_rad(ray_angle));
            }
            init_horiz_intersect.x = (init_horiz_intersect.y - player.getY()) /
                                     std::tan(conv_deg_to_rad(ray_angle)) +
                                     player.getX();
            horiz_gridX = std::floor(init_horiz_intersect.x / TILE_SIZE);
            horiz_gridY = std::floor(init_horiz_intersect.y / TILE_SIZE);
            horiz_next = init_horiz_intersect;
            while ((horiz_gridX >= 0 && horiz_gridX < MAP_SIZE) && 
                   (map.getTile(horiz_gridX, horiz_gridY) == 0)) {
                horiz_next += horiz_dist;
                horiz_gridX = std::floor(horiz_next.x / TILE_SIZE);
                horiz_gridY = std::floor(horiz_next.y / TILE_SIZE);
            }
            horiz_intersect = horiz_next;

            
            /*
                Vertical scan
            */

        }

        drawRendererBuffer.render(window);

        window.display();
    }
}

int main(int argc, char* argv[]) {
    std::cout << "\n\nInitializing raycast demo...\n" << std::endl;
    
    raycast();

    return 0;
}