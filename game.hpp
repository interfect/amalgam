#ifndef AMALGAM_GAME_HPP
#define AMALGAM_GAME_HPP

#include "ui.hpp"

#include <libtcod/libtcod.hpp>

namespace amalgam {

class Entity {
protected:
    // Where is it:?
    int x;
    int y;
    
    // What character does it have?
    int character;
    
public:
    Entity(int x, int y, int character);
    
    // What character represents this thing?
    int getCharacter() const;
    
    // Where is it?
    int getX() const;
    int getY() const;
    
    // How do we move it?
    void setX(int newX);
    void setY(int newY);
    
};

}

#endif
