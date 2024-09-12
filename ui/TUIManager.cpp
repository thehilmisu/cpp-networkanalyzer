#include "TUIManager.h"


void TUIManager::drawUI() {
    for (std::size_t i = 0; i < elements.size(); ++i) {
        elements[i]->draw(i == selectedElement);
    }
    refresh();
}

void TUIManager::handleInput()
{
    int ch;
    while ((ch = getch()) != 'q')
    {
        switch (ch)
        {
        case KEY_UP:
            navigate(-1);
            break;
        case KEY_DOWN:
            navigate(1);
            break;
        case '\n': // Enter key
            elements[selectedElement]->activate();
            break;
        case KEY_MOUSE:
            handleMouse();
            break;
        }
        drawUI();
    }
}

void TUIManager::initializeColorPairs() {
    
    // Define the color pairs
    init_pair(RED_ON_BLACK, COLOR_RED, COLOR_BLACK);
    init_pair(GREEN_ON_BLACK, COLOR_GREEN, COLOR_BLACK);
    init_pair(YELLOW_ON_BLACK, COLOR_YELLOW, COLOR_BLACK);
    init_pair(BLUE_ON_BLACK, COLOR_BLUE, COLOR_BLACK);
    init_pair(MAGENTA_ON_BLACK, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(CYAN_ON_BLACK, COLOR_CYAN, COLOR_BLACK);
    init_pair(WHITE_ON_BLACK, COLOR_WHITE, COLOR_BLACK);
    init_pair(BLACK_ON_WHITE, COLOR_BLACK, COLOR_WHITE);
    init_pair(RED_ON_WHITE, COLOR_RED, COLOR_WHITE);
    init_pair(GREEN_ON_WHITE, COLOR_GREEN, COLOR_WHITE);
    init_pair(BLUE_ON_WHITE, COLOR_BLUE, COLOR_WHITE);
    init_pair(WHITE_ON_GREEN, COLOR_WHITE, COLOR_GREEN);
}

void TUIManager::initWindow()
{
    initscr();
    start_color();
    initializeColorPairs();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
}

void TUIManager::endWindow()
{
    endwin();
}

void TUIManager::navigate(int direction)
{
    selectedElement = (selectedElement + direction + elements.size()) % elements.size();
}

void TUIManager::handleMouse()
{
    MEVENT event;
    if (getmouse(&event) == OK)
    {
        for (std::size_t i = 0; i < elements.size(); ++i)
        {
            if (elements[i]->handleMouseClick({event.x, event.y}))
            {
                selectedElement = i;
                break;
            }
        }
    }
}
