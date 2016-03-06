#ifndef AMALGAM_MAP_HPP
#define AMALGAM_MAP_HPP

#include <map>
#include <vector>

#include <libtcod/libtcod.hpp>

namespace amalgam {

/**
 * Represents a tile in the map.
 */
class Tile {
protected:
    bool passable;
    int character;
    
public:
    Tile(int character = ' ', bool passable = true);
    
    /**
     * Is this tile passable?
     */
    bool isPassable() const;
    
    /**
     * How should this tile be drawn?
     */
    int getCharacter() const;
};

/**
 * Represents the map of a level.
 */
class Map {
protected:
    // Holds all the tiles in the map
    std::map<std::pair<int, int>, Tile> map;
    
public:
    /**
     * Set a tile in the map
     */
    void setTile(int x, int y, const Tile& tile);
    
    /**
     * Get a tile in the map
     */
    const Tile getTile(int x, int y) const;
    
    
    
};

}

#endif
