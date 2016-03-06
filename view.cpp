#include "view.hpp"

namespace amalgam {

MapView::MapView(const Map& map, int x, int y, int width, int height, bool border) : Window(x, y, width, height, border), map(map) {
    // Nothing to do!
}

void MapView::update(int startX, int startY) {
    getConsole()->clear();
    for(int i = 0; i < width - (border ? 2 : 0); i++) {
        for(int j = 0; j < height - (border ? 2 : 0); j++) {
            // What do we draw here?
            auto& tile = map.getTile(startX + i, startY + j);
            // Put it on the console
            getConsole()->putChar(i + border, j + border, tile.getCharacter());
        }
    }
}

}
