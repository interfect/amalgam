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
#include "time.hpp"

using namespace amalgam;

// Constants!
// What name sets are valid for naming scapes?
std::vector<std::string> scapeNameSets = {
    "Mingos town",
    "dwarf surname",
    "demon female"
};

// Globals!
Map map;
Entity player(0, 0, '@');
// All the portals on the current map
std::list<Portal> portals;
// There's always a point on the map where you arrive
Entity arrivalPoint(0, 0, 'X', TCODColor::cyan);

// Pointers to all the entities on the current map
std::list<Entity*> things;

// Name of the scape where you are
std::string scapeName;

// Name of the amalgam node where you are
std::string nodeName;

// Name of the location of the node
std::string nodeLocation;

// Subjective time offset
// Start 325 years in the future.
std::chrono::hours timeOffset(24 * 365 * 325);

/**
 * Generate a level and set up all the globals for the player arriving there.
 *
 * The name generator must be not yet set up, or reset (so we can set its RNG).
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
    
    // Name the scape
    
    // Set up the name generator
    // TODO: This might be slow. Can we just set the rng somehow?
    TCODNamegen::parse("names.txt", rng);
    
    // Pick a random valid name set for scapes
    auto nameSetIndex = rng->getInt(0, scapeNameSets.size() - 1);
    std::string nameSetName = scapeNameSets[nameSetIndex];
    
    // Make a name from the set, and copy the name to the string's storage.
    scapeName = TCODNamegen::generate(&nameSetName[0]);
    
    if(rng->getDouble(0, 1) < 0.33 || nodeName.empty()) {
        // A portion of links lead out of the current node to another node.
        // We also need a new node if we're just starting.
        
        // Make a new node name
        std::string setName("Amalgam node name");
        nodeName = TCODNamegen::generate(&setName[0]);
        
        setName = "Amalgam node location";
        nodeLocation = TCODNamegen::generate(&setName[0]);
        
        // How long does it take to get there?
        // Say we go 4-10 LY at a hop.
        // Go in increments of a day.
        auto travelTime = std::chrono::hours(24 * rng->getInt(365 * 4, 365 * 10));
        timeOffset += travelTime;
    }
    
    // Clear the name generator so we can run again.
    TCODNamegen::destroy();
}

int main(int argc, char** argv) {
    
    
    TCODConsole::initRoot(80,40,"Amalgam",false);

    // Grab an RNG
    TCODRandom* rng = TCODRandom::getInstance();
    
    // Generate a first level.
    generateLevel(rng);
    
    // Set time start
    auto gameStart = std::chrono::system_clock::now();
    
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
        
        // Put some text in the sidebar.
        sidebar.clear();
        
        // Set up control characters
        sidebar.getConsole()->setColorControl(TCOD_COLCTRL_1, TCODColor::red, TCODColor::black);
        sidebar.getConsole()->setColorControl(TCOD_COLCTRL_2, TCODColor::green, TCODColor::black);
        sidebar.getConsole()->setColorControl(TCOD_COLCTRL_3, TCODColor::lightSky, TCODColor::black);
        sidebar.getConsole()->setColorControl(TCOD_COLCTRL_4, TCODColor::amber, TCODColor::black);
        sidebar.getConsole()->setColorControl(TCOD_COLCTRL_5, TCODColor::purple, TCODColor::black);
        
        // Name of the scape we are on.
        sidebar.getConsole()->printRectEx(2, 2, sidebar.getWidth() - 2 - 1, 2, TCOD_BKGND_NONE, TCOD_LEFT,
            "Scape: %c%s%c", TCOD_COLCTRL_1, scapeName.c_str(), TCOD_COLCTRL_STOP); 
        
        // Name of the node running the scape
        sidebar.getConsole()->printRectEx(2, 5, sidebar.getWidth() - 2 - 1, 4, TCOD_BKGND_NONE, TCOD_LEFT,
            "Amalgam Node:\n%c%s%c", TCOD_COLCTRL_3, nodeName.c_str(), TCOD_COLCTRL_STOP);
        
        // Location of the node in space
        sidebar.getConsole()->printRectEx(2, 10, sidebar.getWidth() - 2 - 1, 2, TCOD_BKGND_NONE, TCOD_LEFT,
            "Location:\n%c%s%c", TCOD_COLCTRL_4, nodeLocation.c_str(), TCOD_COLCTRL_STOP);
        
        // Subjective time
        auto realElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - gameStart);
        // Convert to float seconds
        double subjectiveSeconds = realElapsed.count() / 1000.0;
        sidebar.getConsole()->printRectEx(2, 14, sidebar.getWidth() - 2 - 1, 2, TCOD_BKGND_NONE, TCOD_LEFT,
            "Subjective Time:\n%c%.2f%c tau", TCOD_COLCTRL_2, subjectiveSeconds, TCOD_COLCTRL_STOP); 
        
        
        // Time that it is
        auto realDuration = std::chrono::system_clock::now() - gameStart;
        // We say 1 subjective second is 1 real-time millisecond
        auto inGameDuration = realDuration / 1000;
        // Take when we started, jump to the future, and proceed slowly from there.
        auto inGameNow = gameStart + inGameDuration + timeOffset;
        
        std::stringstream gameRealTime;
        printTime(gameRealTime, inGameNow);
        sidebar.getConsole()->printRectEx(2, 17, sidebar.getWidth() - 2 - 1, 2, TCOD_BKGND_NONE, TCOD_LEFT,
            "Real Time:\n%c%s%c", TCOD_COLCTRL_5, gameRealTime.str().c_str(), TCOD_COLCTRL_STOP); 
        
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
