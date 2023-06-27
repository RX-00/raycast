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
            {
                sf::Vector2f initialIntersect;
                initialIntersect.y = std::floor(player.getY() / TILE_SIZE) * TILE_SIZE +
                                     (ray_angle < 180 ? TILE_SIZE : -1);
                initialIntersect.x =
                    (initialIntersect.y - player.getY()) / std::tan(conv_deg_to_rad(ray_angle)) + player.getX();

                // Find distances to the next intersection
                sf::Vector2f distance;
                distance.y = ray_angle < 180 ? TILE_SIZE : -TILE_SIZE;
                distance.x = TILE_SIZE / (ray_angle < 180 ? std::tan(conv_deg_to_rad(ray_angle))
                                                         : -std::tan(conv_deg_to_rad(ray_angle)));

                int gridX = std::floor(initialIntersect.x / TILE_SIZE);
                int gridY = std::floor(initialIntersect.y / TILE_SIZE);
                sf::Vector2f next = initialIntersect;
                while ((gridX >= 0 && gridX < MAP_SIZE) &&
                       map.getTile(gridX, gridY) == 0) {
                    next += distance;
                    gridX = std::floor(next.x / TILE_SIZE);
                    gridY = std::floor(next.y / TILE_SIZE);
                }
                horiz_intersect = next;
            }

            bool left = ray_angle > 90 && ray_angle < 270;
            
            /*
                Vertical line scan
            */
            {
                
                sf::Vector2f initialIntersect;
                initialIntersect.x = std::floor(player.getX() / TILE_SIZE) * TILE_SIZE +
                                     (left ? -1 : TILE_SIZE);
                initialIntersect.y =
                    (initialIntersect.x - player.getX()) * std::tan(conv_deg_to_rad(ray_angle)) + player.getY();

                sf::Vector2f distance;
                distance.x = left ? -TILE_SIZE : TILE_SIZE;
                distance.y = TILE_SIZE * (left ? -std::tan(conv_deg_to_rad(ray_angle))
                                               : std::tan(conv_deg_to_rad(ray_angle)));

                sf::Vector2f next = initialIntersect;
                int gridX = std::floor(next.x / TILE_SIZE);
                int gridY = std::floor(next.y / TILE_SIZE);

                while ((gridX >= 0 && gridX < MAP_SIZE) &&
                       map.getTile(gridX, gridY) == 0) {
                    next += distance;
                    gridX = std::floor(next.x / TILE_SIZE);
                    gridY = std::floor(next.y / TILE_SIZE);
                }
                verti_intersect = next;
            }

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
                wall_color = {255, 153, 51}; // pink/purple
            } else {
                dist = sh_verti_dist;
                wall_color = {255, 204, 102}; // darker pink/purple
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
                drawRendererBuffer.setPixels(i, j, {135, 206, 235}); // sky
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


int main() {
    sf::RenderWindow window({WINDOW_WIDTH, WINDOW_HEIGHT}, "Raycast Test");
    window.setFramerateLimit(60);
    window.setKeyRepeatEnabled(false);

    // For the minimap
    sf::RenderTexture minimapTexture;
    minimapTexture.create(MINI_MAP_TILE_SIZE * MAP_SIZE, MINI_MAP_TILE_SIZE * MAP_SIZE);
    sf::RectangleShape minimapSprite;
    minimapSprite.setSize(
        {(float)MINI_MAP_TILE_SIZE * MAP_SIZE, (float)MINI_MAP_TILE_SIZE * MAP_SIZE});

    Renderer drawBuffer;
    Map map;
    Player player;

    sf::RectangleShape minimapTile;
    minimapTile.setSize({MINI_MAP_TILE_SIZE, MINI_MAP_TILE_SIZE});
    minimapTile.setFillColor(sf::Color::White);
    minimapTile.setOutlineColor(sf::Color::Black);
    minimapTile.setOutlineThickness(1);

    
    KeyInputs keyboard;
    while (window.isOpen()) {
        sf::Event e;
        while (window.pollEvent(e)) {
            keyboard.update(e);
            switch (e.type) {
                case sf::Event::Closed:
                    window.close();
                    break;

                // Minimap editing
                case sf::Event::MouseButtonReleased: {
                    auto mouseEvent = e.mouseButton;
                    int x = mouseEvent.x;
                    int y = mouseEvent.y;
                    if (x > 0 && x < MAP_SIZE * MINI_MAP_TILE_SIZE && y > 0 &&
                        y < MAP_SIZE * MINI_MAP_TILE_SIZE) {
                        x /= MINI_MAP_TILE_SIZE;
                        y /= MINI_MAP_TILE_SIZE;
                        // toggle the tile
                        map.setTile(x, y, !map.getTile(x, y));
                    }
                    break;
                }

                default:
                    break;
            }
        }
        // Input
        player.execInput(keyboard);

        // Update

        // Clear
        window.clear();
        drawBuffer.resetPixels();
        minimapTexture.clear(sf::Color::Transparent);

        // ================= Raycasting starts here ========================
        // Get the starting angle of the ray, that is half the FOV to the "left" of the
        // player's looking angle
        float rayAngle = wrap_around_deg(player.getAngle() - FOV / 2);
        for (int i = 0; i < WINDOW_WIDTH; i++) {

            // These need to be stored for later so they can be compared
            sf::Vector2f horizonatalIntersect;
            sf::Vector2f verticalIntersect;
            // =============== Horizontal line =========================
            // Y Intersection -> Divide the player's Y position by the size of the tiles,
            //  +64 if the ray is looking "down"
            // X Intersection -> Use tan and trig where:
            //  Opp = (Y Intersection - Player Y position)
            //  Theta = Ray's angle
            //  tan(Theta) = Opp / X Intersection so X Intersection = Opp / tan(Theta)
            //bool down = rayAngle < 180;
            {
                sf::Vector2f initialIntersect;
                initialIntersect.y = std::floor(player.getY() / TILE_SIZE) * TILE_SIZE +
                                     (rayAngle < 180 ? TILE_SIZE : -1);
                initialIntersect.x =
                    (initialIntersect.y - player.getY()) / std::tan(conv_deg_to_rad(rayAngle)) + player.getX();

                // Find distances to the next intersection
                sf::Vector2f distance;
                distance.y = rayAngle < 180 ? TILE_SIZE : -TILE_SIZE;
                distance.x = TILE_SIZE / (rayAngle < 180 ? std::tan(conv_deg_to_rad(rayAngle))
                                                         : -std::tan(conv_deg_to_rad(rayAngle)));

                int gridX = std::floor(initialIntersect.x / TILE_SIZE);
                int gridY = std::floor(initialIntersect.y / TILE_SIZE);
                sf::Vector2f next = initialIntersect;
                while ((gridX >= 0 && gridX < MAP_SIZE) &&
                       map.getTile(gridX, gridY) == 0) {
                    next += distance;
                    gridX = std::floor(next.x / TILE_SIZE);
                    gridY = std::floor(next.y / TILE_SIZE);
                }
                horizonatalIntersect = next;
            }

            bool left = rayAngle > 90 && rayAngle < 270;
            // =============== Vertical line =========================
            {
                
                sf::Vector2f initialIntersect;
                initialIntersect.x = std::floor(player.getX() / TILE_SIZE) * TILE_SIZE +
                                     (left ? -1 : TILE_SIZE);
                initialIntersect.y =
                    (initialIntersect.x - player.getX()) * std::tan(conv_deg_to_rad(rayAngle)) + player.getY();

                sf::Vector2f distance;
                distance.x = left ? -TILE_SIZE : TILE_SIZE;
                distance.y = TILE_SIZE * (left ? -std::tan(conv_deg_to_rad(rayAngle))
                                               : std::tan(conv_deg_to_rad(rayAngle)));

                sf::Vector2f next = initialIntersect;
                int gridX = std::floor(next.x / TILE_SIZE);
                int gridY = std::floor(next.y / TILE_SIZE);

                while ((gridX >= 0 && gridX < MAP_SIZE) &&
                       map.getTile(gridX, gridY) == 0) {
                    next += distance;
                    gridX = std::floor(next.x / TILE_SIZE);
                    gridY = std::floor(next.y / TILE_SIZE);
                }
                verticalIntersect = next;
            }

            // Find the shortest distance (And draw a ray on the minimap)
            float hDist = calc_dist({player.getX(), player.getY()}, horizonatalIntersect);
            float vDist = calc_dist({player.getX(), player.getY()}, verticalIntersect) - (left ? 1 : 0);
            float dist = 0;
            sf::Color colour;
            if (hDist < vDist) {
                dist = hDist;
                colour = {255, 153, 51}; // orange
            }
            else {
                dist = vDist;
                colour = {255, 204, 102}; // darker orange
            }

            // Fix the fisheye effect (not quite right...)
            dist = std::cos(conv_deg_to_rad(FOV / 2)) * dist;

            // Draw the walls
            float height =
                TILE_SIZE / dist * (WINDOW_WIDTH / 2 / std::tan(conv_deg_to_rad(FOV / 2)));
            int start = (WINDOW_HEIGHT / 2) - height / 2;
            // Draw the ceiling, then the wall, then the floor
            // This is done by drawing vertical lines in a loop
            for (int y = 0; y < start; y++) {
                drawBuffer.setPixels(i, y, {135, 206, 235});
            }
            for (int y = start; y < start + height; y++) {
                drawBuffer.setPixels(i, y, {colour.r, colour.g, colour.b});
            }
            for (int y = start + height; y < WINDOW_HEIGHT; y++) {
                drawBuffer.setPixels(i, y, {0, 153, 51});
            }

            // Find the next ray angle
            rayAngle = wrap_around_deg(rayAngle + (float)FOV / (float)WINDOW_WIDTH);

            // Draw rays for the mini map
            if (hDist < vDist) {
                drawBuffer.renderLine(
                    minimapTexture, {player.getX() / MINI_MAP_SCALE, player.getY() / MINI_MAP_SCALE},
                    horizonatalIntersect / (float)MINI_MAP_SCALE, {0, 0, 255, 50});
            }
            else {
                drawBuffer.renderLine(
                    minimapTexture, {player.getX() / MINI_MAP_SCALE, player.getY() / MINI_MAP_SCALE},
                    verticalIntersect / (float)MINI_MAP_SCALE, {255, 0, 0, 50});
            }
            
            
        }

        // Actually render the walls
        drawBuffer.render(window);

        // Render the minimap
        for (int y = 0; y < MAP_SIZE; y++) {
            for (int x = 0; x < MAP_SIZE; x++) {
                switch (map.getTile(x, y)) {
                    case 1:
                        minimapTile.setFillColor({255, 255, 255, 200});
                        break;

                    default:
                        minimapTile.setFillColor({127, 127, 127, 200});
                        break;
                }
                minimapTile.setPosition(x * MINI_MAP_TILE_SIZE, y * MINI_MAP_TILE_SIZE);
                window.draw(minimapTile);
            }
        }
        player.draw(minimapTexture);
        drawBuffer.renderLine(minimapTexture,
                            {player.getX() / MINI_MAP_SCALE, player.getY() / MINI_MAP_SCALE},
                            {player.getX() / MINI_MAP_SCALE + player.getXspd() * 25,
                             player.getY() / MINI_MAP_SCALE + player.getYspd() * 25},
                            sf::Color::Yellow);

        minimapTexture.display();
        minimapSprite.setTexture(&minimapTexture.getTexture());
        window.draw(minimapSprite);

        window.display();
    }
    return 0;
}


int main2(int argc, char* argv[]) {
    std::cout << "\n\nInitializing raycast demo...\n" << std::endl;

    sf::Color sky_color = sf::Color::Cyan;
    sf::Color gnd_color = {153, 76, 0}; // brown
    
    //raycast(sky_color, gnd_color);

    return 0;
}