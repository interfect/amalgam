#include "view.hpp"
#include <list>

namespace amalgam {

MapView::MapView(const Map& map, int x, int y, int width, int height, bool border) : Window(x, y, width, height, border), map(map) {
    // Nothing to do!
}

void MapView::update(int startX, int startY, std::list<Entity>& things) {
    getConsole()->clear();
    if(border) {
        getConsole()->printFrame(0, 0, width, height, true);
    }
    for(int i = 0; i < width - (border ? 2 : 0); i++) {
        for(int j = 0; j < height - (border ? 2 : 0); j++) {
            // What do we draw here?
            auto tile = map.getTile(startX + i, startY + j);
            // Put it on the console
            getConsole()->putChar(i + border, j + border, tile.getCharacter());
        }
    }
    for(auto& thing : things) {
        // Can we see the thing?
        if(thing.getX() >= startX + border && thing.getX() < startX + width - (border ? 2 : 0) &&
            thing.getY() >= startY + border && thing.getY() < startY + height - (border ? 2 : 0)) {
        
            // Put the entity if it's in bounds
            getConsole()->putChar(border + thing.getX() - startX, border + thing.getY() - startY, thing.getCharacter());    
            
        }
    }
}

}
