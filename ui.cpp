#include "ui.hpp"

namespace amalgam {

Window::Window(int x, int y, int width, int height, bool border): 
    x(x), y(y), width(width), height(height), border(border), buffer(new TCODConsole(width, height)) {
    
    if(border) {
        // Draw the border
        getConsole()->printFrame(0, 0, width, height, true);
    }
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

int Window::getWidth() const {
    return width;
}

int Window::getHeight() const {
    return height;
}

}
