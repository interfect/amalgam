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


int main(int argc, char** argv) {
    
    
    TCODConsole::initRoot(80,40,"Amalgam",false);

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
    
        // Now clear the screen and draw everything
        TCODConsole::root->clear();
        
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
