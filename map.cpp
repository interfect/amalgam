#include "map.hpp"

namespace amalgam {

Tile::Tile(int character, bool passable) : character(character), passable(passable) {
    // Nothing to do!
}

bool Tile::isPassable() const {
    return passable;
}

int Tile::getCharacter() const {
    return character;
}

void Map::setTile(int x, int y, const Tile& tile) {
    // Save the tile
    map[std::make_pair(x, y)] = tile;
}

const Tile Map::getTile(int x, int y) const {
    if(map.count(std::make_pair(x, y))) {
        // We have a tile there.
        return map.at(std::make_pair(x, y));
    } else {
        // This is off the edge of the map.
        return Tile();
    }
}

void Map::generate(TCODRandom* random) {
    // Generate a random map
    
    // Throw out the old map
    map.clear();
    
    int numRooms = random->getInt(10, 20);
    
    for(int i = 0; i < numRooms; i++) {
        // Make some rooms
        make_rectangle(random->getInt(0, 20), random->getInt(0, 20), random->getInt(3, 10), random->getInt(3, 10));
    }
    
}

void Map::make_rectangle(int x, int y, int width, int height) {
    for(int i = x; i < x + width; i++) {
        for(int j = y; j < y + height; j++) {
            if(i == x || i == x + width - 1 || j == y || j == y + height - 1) {
                // We're on the border. Set a border tile if there's not ile set already
                if(!map.count(std::make_pair(i, j))) {
                    setTile(i, j, Tile('#', false));
                }
            } else {
                // We need to carve out a passable place
                setTile(i, j, Tile('.', true));
            }
        }
    }
}

std::pair<int, int> Map::findEmpty(TCODRandom* random) {
    while(true) {
        // Find an actual tile that's passable
        int tileNumber = random->getInt(0, map.size());
        
        auto it = map.begin();
        
        for(int i = 0; i < tileNumber; i++) {
            it++;
        }
        
        if((*it).second.isPassable()) {
            // We found one
            return (*it).first;
        }
        // Otherwise keep looping until we find one.
        
    }
}

}
