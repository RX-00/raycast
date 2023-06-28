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

    // mini map window
    sf::RenderWindow mini_window({MINI_MAP_TILE_SIZE * MAP_SIZE, MINI_MAP_TILE_SIZE * MAP_SIZE},
                                 "Mini map");
    mini_window.setFramerateLimit(60);
    mini_window.setKeyRepeatEnabled(false);

    // Setup the minimap
    sf::RenderTexture mini_mapTexture;
    mini_mapTexture.create(MINI_MAP_TILE_SIZE * MAP_SIZE, 
                           MINI_MAP_TILE_SIZE * MAP_SIZE);
    sf::RectangleShape mini_mapSprite;
    mini_mapSprite.setSize({(float)MINI_MAP_TILE_SIZE * MAP_SIZE,
                            (float)MINI_MAP_TILE_SIZE * MAP_SIZE});
    sf::RectangleShape mini_mapTile;
    mini_mapTile.setSize({MINI_MAP_TILE_SIZE, MINI_MAP_TILE_SIZE});
    mini_mapTile.setFillColor(sf::Color::Blue);
    mini_mapTile.setOutlineColor(sf::Color::Black);
    mini_mapTile.setOutlineThickness(1.1);

    // Setup Renderer, map, and player
    Renderer drawRendererBuffer;
    Map map;
    Player player;

    // variables needed for the raycast loop
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


    // Main loop with keyboard input
    KeyInputs playerInput;
    while (window.isOpen() || mini_window.isOpen()) {
        sf::Event e; // keyboard event
        while (window.pollEvent(e) || mini_window.pollEvent(e)) {
            playerInput.update(e);
            switch (e.type) {
            case sf::Event::Closed:
                std::cout << "Goodbye!" << std::endl;
                window.close();
                mini_window.close();
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
        mini_window.clear();
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
            /*
                Horizonal line scan
            */
            init_horiz_intersect.y = std::floor(player.getY() / TILE_SIZE) * TILE_SIZE +
                                     (ray_angle < 180 ? TILE_SIZE : -1);
            init_horiz_intersect.x = (init_horiz_intersect.y - player.getY()) /
                                      std::tan(conv_deg_to_rad(ray_angle)) + player.getX();

            // Find distances to the next intersection
            horiz_dist.y = ray_angle < 180 ? TILE_SIZE : -TILE_SIZE;
            horiz_dist.x = TILE_SIZE / (ray_angle < 180 ? std::tan(conv_deg_to_rad(ray_angle))
                                                       : -std::tan(conv_deg_to_rad(ray_angle)));

            horiz_gridX = std::floor(init_horiz_intersect.x / TILE_SIZE);
            horiz_gridY = std::floor(init_horiz_intersect.y / TILE_SIZE);
            horiz_next = init_horiz_intersect;
            while ((horiz_gridX >= 0 && horiz_gridX < MAP_SIZE) &&
                    map.getTile(horiz_gridX, horiz_gridY) == 0) {
                horiz_next += horiz_dist;
                horiz_gridX = std::floor(horiz_next.x / TILE_SIZE);
                horiz_gridY = std::floor(horiz_next.y / TILE_SIZE);
            }
            horiz_intersect = horiz_next;
            
            /*
                Vertical line scan
            */
            bool left = ray_angle > 90 && ray_angle < 270;
            init_verti_intersect.x = std::floor(player.getX() / TILE_SIZE) * TILE_SIZE +
                                     (left ? -1 : TILE_SIZE);
            init_verti_intersect.y = (init_verti_intersect.x - player.getX()) *
                                      std::tan(conv_deg_to_rad(ray_angle)) + player.getY();

            verti_dist.x = left ? -TILE_SIZE : TILE_SIZE;
            verti_dist.y = TILE_SIZE * (left ? -std::tan(conv_deg_to_rad(ray_angle))
                                               : std::tan(conv_deg_to_rad(ray_angle)));

            verti_next = init_verti_intersect;
            verti_gridX = std::floor(verti_next.x / TILE_SIZE);
            verti_gridY = std::floor(verti_next.y / TILE_SIZE);

            while ((verti_gridX >= 0 && verti_gridX < MAP_SIZE) &&
                map.getTile(verti_gridX, verti_gridY) == 0) {
                verti_next += verti_dist;
                verti_gridX = std::floor(verti_next.x / TILE_SIZE);
                verti_gridY = std::floor(verti_next.y / TILE_SIZE);
            }
            verti_intersect = verti_next;    

            // getting the shortest casted ray line
            float sh_horiz_dist = calc_dist({player.getX(), player.getY()}, horiz_intersect);
            float sh_verti_dist;
            if (left) {
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
                wall_color = {100, 0, 100}; // darker pink/purple
            }

            // fisheye effect compensation
            dist = std::cos(conv_deg_to_rad(FOV / 2.5)) * dist;

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
                                              {255, 0, 255});
            } else {
                drawRendererBuffer.renderLine(mini_mapTexture,
                                              {player.getX() / MINI_MAP_SCALE, player.getY() / MINI_MAP_SCALE},
                                              verti_intersect / (float)MINI_MAP_SCALE,
                                              {210, 0, 210});
            }
        }
        
        /*
            Render everything!
        */
        drawRendererBuffer.render(window);
        drawRendererBuffer.render(mini_window);

        // mini map render
        for (int y = 0; y < MAP_SIZE; y++) {
            for (int x = 0; x < MAP_SIZE; x++) {
                switch (map.getTile(x,y))
                {
                case 1:
                    mini_mapTile.setFillColor(sf::Color::White);
                    break;
                
                default:
                    mini_mapTile.setFillColor(sf::Color::Cyan);
                    break;
                }
                mini_mapTile.setPosition(x * MINI_MAP_TILE_SIZE + 0,
                                         y * MINI_MAP_TILE_SIZE + 0);
                //window.draw(mini_mapTile);
                mini_window.draw(mini_mapTile);
            }
        }

        player.draw(mini_mapTexture);
        // draw player orientation line
        drawRendererBuffer.renderLine(mini_mapTexture,
                                      {player.getX() / MINI_MAP_SCALE, player.getY() / MINI_MAP_SCALE},
                                      {player.getX() / MINI_MAP_SCALE + player.getXspd() * 25,
                                       player.getY() / MINI_MAP_SCALE + player.getYspd() * 25},
                                      sf::Color::Red);

        mini_mapTexture.display();
        mini_mapSprite.setTexture(&mini_mapTexture.getTexture());
        //window.draw(mini_mapSprite);
        mini_window.draw(mini_mapSprite);
        window.display();
        mini_window.display();
    }
}


int main(int argc, char* argv[]) {
    std::cout << "\n\nInitializing raycast demo...\n" << std::endl;

    sf::Color sky_color = {25, 200, 50}; // light blue 90 90 200
    sf::Color gnd_color = {90, 90, 200}; // dark green 25, 200, 50
    
    raycast(sky_color, gnd_color);

    return 0;
}