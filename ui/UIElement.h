#ifndef UIELEMENT_H
#define UIELEMENT_H

struct Position {
    int x;
    int y;
};

class UIElement
{
public:
    virtual ~UIElement() = default;
    virtual void draw(bool selected = false) const = 0;
    virtual void setPosition(Position pos) = 0;
    virtual bool handleMouseClick(Position mouse_pos) = 0;
    virtual void activate() = 0;
};

#endif // UIElement