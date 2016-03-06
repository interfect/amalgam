#include "game.hpp"

namespace amalgam {

Entity::Entity(int x, int y, int character) : x(x), y(y), character(character) {
    // Nothing to do
}

int Entity::getCharacter() const {
    return character;
}

int Entity::getX() const {
    return x;
}

int Entity::getY() const {
    return y;
}

void Entity::setX(int newX) {
    x = newX;
}

void Entity::setY(int newY) {
    y = newY;
}
    
}
