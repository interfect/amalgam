#ifndef AMALGAM_UI_HPP
#define AMALGAM_UI_HPP

#include <memory>
#include <sstream>

#include <libtcod/libtcod.hpp>

namespace amalgam {

class Window {
protected:
    // Where is the window in its parent?
    int x;
    int y;
    int width;
    int height;
    
    // Do we have a border?
    bool border;
    
private:
    // We need our own console. But we will change how it works so people
    // shouldn't touch it directly.
    std::unique_ptr<TCODConsole> buffer;
 
public:
    /**
     * Make a new Window at the given location with the given width and height.
     */
    Window(int x, int y, int width, int height, bool border = false);
    
    /**
     * Draw the console to the root window at the right place.
     */
    void draw(TCODConsole* drawTo) const;
    
    /**
     * Get a pointer to the underlying libtcod console to draw on
     */
    TCODConsole* getConsole();
    const TCODConsole* getConsole() const;
    
    int getWidth() const;
    int getHeight() const;
    
};

}

#endif
