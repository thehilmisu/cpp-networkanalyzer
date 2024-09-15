#include "TUIManager.h"
#include "Textbox.h"

void TUIManager::drawUI() {
    for (size_t i = 0; i < elements.size(); ++i) {
        elements[i]->draw(i == selectedElement);
    }
    refresh();
}

void TUIManager::handleInput(std::atomic<bool>& exitFlag) {
    nodelay(stdscr, TRUE);
    int ch = getch();

    if (ch != ERR) {
        if (ch == KEY_MOUSE) {
            MEVENT event;
            if (getmouse(&event) == OK) {
                // Loop through all elements to see if they handle the mouse event
                for (size_t i = 0; i < elements.size(); ++i) {
                    if (elements[i]->handleMouseEvent(event)) {
                        selectedElement = i;
                        drawUI();
                        break;
                    }
                }
            }
        } else {
            // Pass key presses to the selected element
            auto activeElement = elements[selectedElement];
            // Check if the element has a method to handle key presses
            if (auto textBox = dynamic_cast<TextBox*>(activeElement.get())) {
                textBox->handleKeyPress(ch);
            } else {
                switch (ch) {
                    case KEY_UP:
                        navigate(-1);
                        break;
                    case KEY_DOWN:
                        navigate(1);
                        break;
                    case '\n':
                        activeElement->activate();
                        break;
                    case 'q':
                        exitFlag = true;
                        break;
                    // Handle other keys
                    default:
                        break;
                }
            }
            drawUI();
        }
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
        for (size_t i = 0; i < elements.size(); ++i)
        {
            if (elements[i]->handleMouseClick({event.x, event.y}))
            {
                selectedElement = i;
                break;
            }
        }
    }
}
