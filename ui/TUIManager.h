#ifndef TUIMANAGER_UI
#define TUIMANAGER_UI

#include <memory>
#include <vector>
#include "UIElement.h"
#include <ncurses.h>

enum ColorPair {
    RED_ON_BLACK = 1,
    GREEN_ON_BLACK,
    YELLOW_ON_BLACK,
    BLUE_ON_BLACK,
    MAGENTA_ON_BLACK,
    CYAN_ON_BLACK,
    WHITE_ON_BLACK,
    BLACK_ON_WHITE,
    RED_ON_WHITE,
    GREEN_ON_WHITE,
    BLUE_ON_WHITE,
    WHITE_ON_GREEN,
};

class TUIManager {
public:
    template<typename T>
    void placeElement(std::unique_ptr<T> element, Position pos) {
        element->setPosition(pos);
        element->draw();
        elements.push_back(std::move(element));
    }

    template<typename T>
    void placeElement(std::unique_ptr<T> element) {
        element->draw();
        elements.push_back(std::move(element));
    }

    void drawUI();
    void handleInput();
    void initWindow();
    void endWindow();

private:
    std::vector<std::unique_ptr<UIElement>> elements;
    std::size_t selectedElement = 0;
    void initializeColorPairs();
    void navigate(int direction);
    void handleMouse();
};

#endif