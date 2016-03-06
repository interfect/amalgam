#include "ui.hpp"

namespace amalgam {

Window::Window(int x, int y, int width, int height): 
    x(x), y(y), width(width), height(height), buffer(new TCODConsole(width, height)) {
    // Nothing to do
}

void Window::draw(TCODConsole* drawTo) const {
    // Blit the whole window's console to the right place on the thing we're
    // drawing on.
    TCODConsole::blit(getConsole(), 0, 0, width, height, drawTo, x, y);
}
    
TCODConsole* Window::getConsole() {
    return buffer.get();
}

const TCODConsole* Window::getConsole() const {
    return buffer.get();
}

}
