#define _USE_MATH_DEFINES
#include <cmath>
#include <ctime>
#include <iostream>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <vector>
#include <list>
#include <thread>
#include <utility>

#include <libtcod/libtcod.hpp>

#include "ui.hpp"
#include "view.hpp"
#include "map.hpp"
#include "game.hpp"

using namespace amalgam;

int main(int argc, char** argv) {
    
    
    TCODConsole::initRoot(80,40,"Amalgam",false);

    // Make a map
    Map map;
    
    std::list<Entity> things;
    
    // Make a player
    things.emplace_back(10, 10, '@');
    Entity& player = *things.begin();

    // Make a sidebar
    auto sidebar = Window(50, 0, 30, 40, true);
    // Make a main window
    auto mapView = MapView(map, 0, 0, 50, 40, true);
    
    // Decide where the map should be drawn from
    int originX = 0;
    int originY = 0;

    double maxFrameTime = 0;
    
    while (!TCODConsole::isWindowClosed()) {
    
        // We need to measure how long it takes
        auto loop_start = std::chrono::steady_clock::now();
        
        // Collect key presses
        TCOD_key_t pressed;
        TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS,&pressed,NULL);
        
        if(pressed.vk == TCODK_ESCAPE) {
            // Exit the game
            break;
        }
        
        if(pressed.vk == TCODK_UP) {
            // Try moving up
            if(map.getTile(player.getX(), player.getY() - 1).isPassable()) {
                player.setY(player.getY() - 1);
            }
        }
        
        if(pressed.vk == TCODK_DOWN) {
            // Try moving down
            if(map.getTile(player.getX(), player.getY() + 1).isPassable()) {
                player.setY(player.getY() + 1);
            }
        }
        
        if(pressed.vk == TCODK_LEFT) {
            // Try moving left
            if(map.getTile(player.getX() - 1, player.getY()).isPassable()) {
                player.setX(player.getX() - 1);
            }
        }
        
        if(pressed.vk == TCODK_RIGHT) {
            // Try moving right
            if(map.getTile(player.getX() + 1, player.getY()).isPassable()) {
                player.setX(player.getX() + 1);
            }
        }
    
        // Decide where the map should draw from
        // We subtract off 2 for the border.
        originX = player.getX() - (mapView.getWidth() - 2) / 2;
        originY = player.getY() - (mapView.getHeight() - 2) / 2;
    
        // Now clear the screen and draw everything
        TCODConsole::root->clear();
        
        mapView.update(originX, originY, things);
        
        mapView.draw(TCODConsole::root);
        sidebar.draw(TCODConsole::root);
        
        // Add a frame time counter
        // First get the frame time in ms
        double frameTime = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(
            std::chrono::steady_clock::now() - loop_start).count();
        maxFrameTime = std::max(frameTime, maxFrameTime);
        // Draw it
        TCODConsole::root->printEx(79, 0, TCOD_BKGND_NONE, TCOD_RIGHT, "Frame: %.2f ms/%.2f ms max",
            frameTime, maxFrameTime);
        
        TCODConsole::flush();
        
        // How long did the game loop take?
        auto elapsed = std::chrono::steady_clock::now() - loop_start;
        
        // Sleep for a total frame time of 20 ms
        std::this_thread::sleep_for(std::chrono::milliseconds(10) - elapsed);
    }

    return 0;
}
