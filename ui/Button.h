#ifndef BUTTON_H
#define BUTTON_H

#include "UIElement.h"
#include <functional>
#include <ncurses.h>

class Button : public UIElement
{
public:
    Button(
        const std::string &label, std::function<void()> onClick, 
        int color_pair, const Position &pos);
    virtual ~Button() = default;
    void draw(bool selected = false) const override;
    void activate() override;
    bool handleMouseClick(Position mouse_pos) override;
    void setPosition(Position pos) override;

private:
    std::string label;
    std::function<void()> onClick;
    int color_pair;
    Position m_pos;
};

#endif
