#ifndef LABEL_H
#define LABEL_H

#include "UIElement.h"
#include <ncurses.h>

class Label : public UIElement
{
public:
    Label(const std::string &text, int color_pair, const Position &pos);
    virtual ~Label() = default;
    void draw(bool selected = false) override;
    void setPosition(Position pos) override;
    void setText(const std::string &text) override;
    bool handleMouseClick(Position mouse_pos) override;
    void activate() override;

private:
    std::string text;
    int color_pair;
    Position m_pos;
};

#endif