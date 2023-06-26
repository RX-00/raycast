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
#include "Renderer.h"
#include "Configs.h"
#include "KeyInputs.h"
#include "Player.h"
#include "Map.h"


void raycast(sf::Color sky_color, sf::Color gnd_color) {
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
            int verti_gridY;

            /*
                Horizonal line scan
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
                Vertical line scan
            */
            if (ray_angle < 270 && ray_angle > 90) {
                init_verti_intersect.x = std::floor(player.getY() / TILE_SIZE) * TILE_SIZE - 1;
                verti_dist.x = -TILE_SIZE;
                verti_dist.y = TILE_SIZE / -std::tan(conv_deg_to_rad(ray_angle));
            } else {
                init_verti_intersect.x = std::floor(player.getY() / TILE_SIZE) * TILE_SIZE + TILE_SIZE;
                verti_dist.x = TILE_SIZE;
                verti_dist.y = TILE_SIZE / std::tan(conv_deg_to_rad(ray_angle));
            }
            init_verti_intersect.y = (init_verti_intersect.x - player.getX()) /
                                     std::tan(conv_deg_to_rad(ray_angle)) +
                                     player.getY();
            verti_gridX = std::floor(init_verti_intersect.x / TILE_SIZE);
            verti_gridY = std::floor(init_verti_intersect.y / TILE_SIZE);
            verti_next = init_verti_intersect;
            while ((verti_gridX >= 0 && verti_gridX < MAP_SIZE) && 
                   (map.getTile(verti_gridX, verti_gridY) == 0)) {
                verti_next += verti_dist;
                verti_gridX = std::floor(verti_next.x / TILE_SIZE);
                verti_gridY = std::floor(verti_next.y / TILE_SIZE);
            }
            verti_intersect = verti_next;

            // getting the shortest casted ray line
            float sh_horiz_dist = calc_dist({player.getX(), player.getY()}, horiz_intersect);
            float sh_verti_dist;
            if (ray_angle < 270 && ray_angle > 90) {
                sh_verti_dist = calc_dist({player.getX(), player.getY()}, verti_intersect) - 1;
            } else {
                sh_verti_dist = calc_dist({player.getX(), player.getY()}, verti_intersect) - 0;
            }
            float dist = 0;
            // wall color depending on your view
            sf::Color wall_color;
            if (sh_horiz_dist < sh_verti_dist) {
                dist = sh_horiz_dist;
                wall_color = {255, 0, 255}; // pink/purple
            } else {
                dist = sh_verti_dist;
                wall_color = {205, 0, 205}; // darker pink/purple
            }

            // fisheye effect compensation
            dist = std::cos(conv_deg_to_rad(FOV / 2)) * dist;

            /*
                Draw everything!
            */
            float height = TILE_SIZE / dist *
                           (WINDOW_WIDTH / 2 / std::tan(conv_deg_to_rad(FOV / 2)));
            int start = (WINDOW_HEIGHT / 2) - height / 2;
            // draw sky, wall, floor in that order by drawing vertical lines
            // again and again in a loop
            for (int j = 0; j < start; j++) {
                drawRendererBuffer.setPixels(i, j, {sky_color.r, sky_color.g, sky_color.b}); // sky
            }
            for (int j = start; j < start + height; j++) {
                drawRendererBuffer.setPixels(i, j, {wall_color.r, wall_color.g, wall_color.b}); // wall
            }
            for (int j = start + height; j < WINDOW_HEIGHT; j++) {
                drawRendererBuffer.setPixels(i, j, {gnd_color.r, gnd_color.g, gnd_color.b}); // ground (floor)
            }

            /*
                Cast next ray and draw stuff on mini map
            */
            ray_angle = wrap_around_deg(ray_angle + (float)FOV / (float)WINDOW_WIDTH);

            // TODO: can probably get rid of this if else
            if (sh_horiz_dist < sh_verti_dist) {
                drawRendererBuffer.renderLine(mini_mapTexture,
                                              {player.getX() / MINI_MAP_SCALE, player.getY() / MINI_MAP_SCALE},
                                              horiz_intersect / (float)MINI_MAP_SCALE,
                                              {0, 0, 255, 50});
            } else {
                drawRendererBuffer.renderLine(mini_mapTexture,
                                              {player.getX() / MINI_MAP_SCALE, player.getY() / MINI_MAP_SCALE},
                                              horiz_intersect / (float)MINI_MAP_SCALE,
                                              {0, 0, 255, 50});
            }
        }
        
        /*
            Render everything!
        */
        drawRendererBuffer.render(window);

        window.display();
    }
}

int main(int argc, char* argv[]) {
    std::cout << "\n\nInitializing raycast demo...\n" << std::endl;

    sf::Color sky_color = sf::Color::Cyan;
    sf::Color gnd_color = {153, 76, 0}; // brown
    
    raycast(sky_color, gnd_color);

    return 0;
}