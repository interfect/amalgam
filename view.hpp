#ifndef AMALGAM_VIEW_HPP
#define AMALGAM_VIEW_HPP

#include "ui.hpp"
#include "map.hpp"
#include "game.hpp"

#include <list>

#include <libtcod/libtcod.hpp>

namespace amalgam {

class MapView : public Window {

protected:
    // Keep a reference to the map we're drawing
    const Map& map;
 
public:
    /**
     * Make a new MapView at the given location with the given width and height,
     * drawing the given map.
     */
    MapView(const Map& map, int x, int y, int width, int height, bool border = true);
    
    /**
     * Update the internal buffer, drawing the map from the given upper left
     * coordinate.
     */
    void update(int startX, int startY, std::list<Entity>& things);
    
};

}

#endif
