#ifndef TUIMANAGER_UI
#define TUIMANAGER_UI

#include <memory>
#include <vector>
#include <atomic>
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
    void placeElement(std::shared_ptr<T> element) {
        element->draw();
        elements.push_back(element);
    }

    void drawUI();
    void handleInput(std::atomic<bool>& exitFlag);
    void initWindow();
    void endWindow();

private:
    std::vector<std::shared_ptr<UIElement>> elements;
    size_t selectedElement = 0;
    void initializeColorPairs();
    void navigate(int direction);
    void handleMouse();
};

#endif