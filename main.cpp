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

// Globals!
Map map;
Entity player(0, 0, '@');
// All the portals on the current map
std::list<Portal> portals;
// There's always a point on the map where you arrive
Entity arrivalPoint(0, 0, 'X', TCODColor::blue);

// Pointers to all the entities on the current map
std::list<Entity*> things;

/**
 * Generate a level and set up all the globals for the player arriving there.
 */
void generateLevel(TCODRandom* rng) {

    things.clear();

    // Make a map
    map.generate(rng);
    
    // Make an arrival point
    auto arrivalPos = map.findEmpty(rng);
    arrivalPoint.setX(arrivalPos.first);
    arrivalPoint.setY(arrivalPos.second);
    things.push_back(&arrivalPoint);
    
    // Place the player on top of it
    player.setX(arrivalPos.first);
    player.setY(arrivalPos.second);
    things.push_back(&player);
    
    // Add outgoing portals
    portals.clear();
    int numPortalsOut = rng->getInt(1, 3);
    for(int i = 0; i < numPortalsOut; i++) {
        // Make some portals
        auto portalPos = map.findEmpty(rng);
        portals.push_back(Portal(portalPos.first, portalPos.second));
    }
    for(auto& portal : portals) {
        // Put them in the entity list
        things.push_back(&portal);
    }

}

int main(int argc, char** argv) {
    
    
    TCODConsole::initRoot(80,40,"Amalgam",false);

    // Grab an RNG
    TCODRandom* rng = TCODRandom::getInstance();

    // Generate a first level.
    generateLevel(rng);
    
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
        
        // Is the player on a portal?
        for(auto& portal : portals) {
            if(portal.getX() == player.getX() && portal.getY() == player.getY()) {
                // We're on a portal!
                
                // Activate it and go to a new map
                generateLevel(rng);
                
                // Stop looping because everything is different
                break;
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
