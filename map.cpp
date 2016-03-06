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

}
