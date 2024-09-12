#include "Label.h"

#include <iostream>

Label::Label(const std::string &text, int color_pair, const Position &pos)
    : text(text), color_pair(color_pair), m_pos(pos) {}

void Label::draw(bool selected) const
{
    if (selected)
    {
        attron(A_REVERSE);
    }
    attron(COLOR_PAIR(color_pair));
    mvprintw(m_pos.y, m_pos.x, "%s", text.c_str());
    attroff(COLOR_PAIR(color_pair));
    if (selected)
    {
        attroff(A_REVERSE);
    }
}

void Label::setPosition(Position pos)
{
    m_pos = pos;
}

bool Label::handleMouseClick(Position mouse_pos)
{
    //std::cout << mouse_pos.x << mouse_pos.y << std::endl;
    return false;
}

void Label::activate()
{
}