#include "game.hpp"

namespace amalgam {

Entity::Entity(int x, int y, int character, TCODColor foreground) : x(x), y(y), character(character), foreground(foreground) {
    // Nothing to do
}

int Entity::getCharacter() const {
    return character;
}

TCODColor Entity::getColor() const {
    return foreground;
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

Portal::Portal(int x, int y) : Entity(x, y, 'P', TCODColor::green) {
    // Nothing to do
}

int Portal::getCharacter() const {
    // Pick a random character.
    return charOptions[TCODRandom::getInstance()->getInt(0, charOptions.size())];
}

const std::string Portal::charOptions = "Aagshjdasflw71984(*^@29*90#A.,:[];";
    
}
