#ifndef UIELEMENT_H
#define UIELEMENT_H

#include "Signal.h"  // Include the Signal class for signal support
#include <string>
#include <ncurses.h>

struct Position {
    int x;
    int y;
};

class UIElement {
public:
    virtual ~UIElement() = default;
    virtual void draw(bool selected = false) const = 0;
    virtual void setPosition(Position pos) = 0;
    virtual void setText(const std::string& txt) {}
    virtual bool handleMouseClick(Position mouse_pos) = 0;
    virtual void activate() = 0;
    virtual void navigate(int dRow, int dCol = 0) {}
    virtual bool handleMouseEvent(const MEVENT& event) {
        return false; // Default implementation
    }
};


#endif // UIELEMENT_H
