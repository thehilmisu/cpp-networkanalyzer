#include "Button.h"

Button::Button(
      const std::string &label, std::function<void()> onClick, int color_pair, const Position &pos)
    : label(label), onClick(onClick), color_pair(color_pair), m_pos(pos){}

void Button::draw(bool selected) 
{
    if (selected)
    {
        attron(A_REVERSE);
    }
    attron(COLOR_PAIR(color_pair));
    mvprintw(m_pos.y, m_pos.x, "[ %s ]", label.c_str());
    attroff(COLOR_PAIR(color_pair));
    if (selected)
    {
        attroff(A_REVERSE);
    }
}

void Button::activate()
{
    if (onClick)
    {
        onClick();
    }
}

bool Button::handleMouseClick(Position mouse_pos)
{
    int length = label.length() + 4; // 4 for "[ ]" around the label
    if (mouse_pos.y == m_pos.y && mouse_pos.x >= m_pos.x && mouse_pos.x < m_pos.x + length)
    {
        activate();
        return true;
    }
    return false;
}

void Button::setPosition(Position pos)
{
    m_pos = pos;
}
