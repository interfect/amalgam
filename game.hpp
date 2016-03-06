#ifndef AMALGAM_GAME_HPP
#define AMALGAM_GAME_HPP

#include "ui.hpp"

#include <string>

#include <libtcod/libtcod.hpp>

namespace amalgam {

/**
 * Represents a thing that can sit in the world somewhere.
 */
class Entity {
protected:
    // Where is it:?
    int x;
    int y;
    
    // What character does it have?
    int character;
    
    // What color is it?
    TCODColor foreground;
    
public:
    Entity(int x, int y, int character, TCODColor foreground = TCODColor::white);
    
    // What character represents this thing?
    virtual int getCharacter() const;
    virtual TCODColor getColor() const;
    
    // Where is it?
    int getX() const;
    int getY() const;
    
    // How do we move it?
    void setX(int newX);
    void setY(int newY);
    
};

/**
 * Represents a portal to another scape.
 */ 
class Portal : public Entity {
protected:
    const static std::string charOptions;
public:
    Portal(int x, int y);
    
    // The character will change every frame.
    virtual int getCharacter() const;
};

}

#endif
