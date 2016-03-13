#include "map.hpp"

#include <set>
#include <list>
#include <vector>

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
    
    int numRooms = random->getInt(20, 30);
    
    for(int i = 0; i < numRooms; i++) {
        // Make some rooms
        make_rectangle(random->getInt(0, 20), random->getInt(0, 20), random->getInt(3, 10), random->getInt(3, 10));
    }
    
    // Flood fill from a single point and delete everything else.
    std::set<std::pair<int, int>> reachable;
    
    // We need to remember what we already queued
    std::set<std::pair<int, int>> tested;
    // Start at a random place and flood fill from there.
    std::list<std::pair<int, int>> queue = {findEmpty(random)};
    
    while(queue.size() > 0) {
        // Grab a random place
        auto toVisit = queue.front();
        queue.pop_front();
        
        if(getTile(toVisit.first, toVisit.second).isPassable()) {
            // We can go here!
            
            // This tile is reachable
            reachable.insert(toVisit);
            
            std::vector<std::pair<int, int>> offsets = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
            for(auto offset : offsets) {
                auto nextToVisit = std::make_pair(toVisit.first + offset.first, toVisit.second + offset.second);
                
                if(!tested.count(nextToVisit)) {
                
                    // Try this potentially reachable spot next.
                    queue.push_back(nextToVisit);
                    // Don't queue it again
                    tested.insert(nextToVisit);   
                }
                
            }
        }
    }
    
    for(auto& kv : map) {
        if(!reachable.count(kv.first) && kv.second.isPassable()) {
            // This is a passable place that isn't reachable.
            
            // Make it impassable as a hack. The player will never know because
            // they can never get there, but it won't be chosen when we ask for
            // a passable spot.
            kv.second = Tile(kv.second.getCharacter(), false);
        }
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
