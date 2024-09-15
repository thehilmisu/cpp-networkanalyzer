#ifndef TEXTBOX_H
#define TEXTBOX_H

#include <string>
#include <ncurses.h>
#include "UIElement.h"

class TextBox : public UIElement {
public:
    TextBox(Position pos, int width)
        : position(pos), width(width), cursorPosition(0), isActive(false) {
        content = "";
    }

    void draw(bool selected = false) override {
        attron(COLOR_PAIR(GREEN_ON_WHITE));
        if (selected || isActive) {
            attron(A_REVERSE);
        }
        mvprintw(position.y, position.x, "%-*s", width, content.c_str());
        attroff(A_REVERSE);
        attroff(COLOR_PAIR(GREEN_ON_WHITE));
        refresh();

        // If active, move the cursor to the appropriate position
        if (isActive) {
            move(position.y, position.x + cursorPosition);
            curs_set(1); // Make cursor visible
        } else {
            curs_set(0); // Hide cursor
        }
    }

    void setPosition(Position pos) override {
        position = pos;
    }

    bool handleMouseClick(Position mouse_pos) override {
        if (mouse_pos.y == position.y && mouse_pos.x >= position.x && mouse_pos.x < position.x + width) {
            isActive = true;
            cursorPosition = std::min(static_cast<int>(content.length()), mouse_pos.x - position.x);
            draw(true);
            return true;
        } else {
            if (isActive) {
                isActive = false;
                draw();
            }
            return false;
        }
    }

    void activate() override {
        isActive = true;
        draw(true);
    }

    void navigate(int dRow, int dCol = 0) override {
        // For TextBox, navigation can move the cursor within the text
        if (!isActive) return;
        cursorPosition += dCol;
        if (cursorPosition < 0) cursorPosition = 0;
        if (cursorPosition > content.length()) cursorPosition = content.length();
        draw(true);
    }

    bool handleKeyPress(int ch) {
        if (!isActive) return false;

        switch (ch) {
            case KEY_LEFT:
                navigate(0, -1);
                break;
            case KEY_RIGHT:
                navigate(0, 1);
                break;
            case KEY_BACKSPACE:
            case 127: // DEL key
                if (cursorPosition > 0) {
                    content.erase(cursorPosition - 1, 1);
                    cursorPosition--;
                    draw(true);
                }
                break;
            case KEY_DC: // Delete key
                if (cursorPosition < content.length()) {
                    content.erase(cursorPosition, 1);
                    draw(true);
                }
                break;
            case '\n': // Enter key
                isActive = false;
                draw();
                break;
            default:
                if (isprint(ch) && content.length() < width) {
                    content.insert(cursorPosition, 1, static_cast<char>(ch));
                    cursorPosition++;
                    draw(true);
                }
                break;
        }
        return true;
    }

    std::string getText() const {
        return content;
    }

    void setText(const std::string& text) {
        content = text.substr(0, width);
        cursorPosition = content.length();
        draw(isActive);
    }

    bool handleMouseEvent(const MEVENT& event) override {
        int mouseX = event.x;
        int mouseY = event.y;

        if (event.bstate & BUTTON1_CLICKED) {
            return handleMouseClick({mouseX, mouseY});
        }
        return false;
    }


private:
    Position position;
    int width;
    std::string content;
    int cursorPosition;
    bool isActive;
};

#endif // TEXTBOX_H
