#ifndef DROPDOWNWIDGET_H
#define DROPDOWNWIDGET_H

#include <ncurses.h>
#include <vector>
#include <string>
#include <stdexcept>
#include "UIElement.h"

enum class DropdownState {
    Closed,
    Open,
    OptionSelected,
};

class DropdownWidget : public UIElement {
public:
    DropdownWidget(Position pos, const std::vector<std::string>& options)
        : position(pos), options(options), state(DropdownState::Closed), selectedIndex(0), color_pair(BLUE_ON_WHITE), color_pair_selected(BLACK_ON_WHITE) {
        if (options.empty()) {
            throw std::runtime_error("Dropdown options cannot be empty.");
        }
        cellWidth = calculateMaxOptionWidth() + 2; // +2 for padding
    }

    void draw(bool selected = false) const override {
        if (selected) {
            attron(A_REVERSE);
        }

        // Display the selected option
        attron(COLOR_PAIR(color_pair_selected));
        mvprintw(position.y, position.x, " %-*s v ", cellWidth, options[selectedIndex].c_str());
        attroff(COLOR_PAIR(color_pair_selected));

        if (selected) {
            attroff(A_REVERSE);
        }

        // If dropdown is open, display all options below the selected one
        if (state == DropdownState::Open) {
            for (std::size_t i = 0; i < options.size(); ++i) {
                //if (i == selectedIndex) continue; // Skip the already displayed selected item
                attron(COLOR_PAIR(color_pair));
                mvprintw(position.y + i + 1, position.x, "%-*s", cellWidth, options[i].c_str());
                attroff(COLOR_PAIR(color_pair));
            }
        }

        refresh();
    }

    void setPosition(Position pos) override {
        position = pos;
    }

    bool handleMouseClick(Position mouse_pos) override {
        switch (state) {
            case DropdownState::Closed:
                handleClosedState(mouse_pos);
                break;

            case DropdownState::Open:
                handleOpenState(mouse_pos);
                break;

            case DropdownState::OptionSelected:
                handleOptionSelectedState(mouse_pos);
                break;
        }

        return false;
    }

    void activate() override {
        state = (state == DropdownState::Closed) ? DropdownState::Open : DropdownState::Closed;
        draw(true);
    }

    void navigate(int direction) {
        if (state != DropdownState::Open) return;

        selectedIndex = (selectedIndex + direction + options.size()) % options.size();
        draw(true);
    }

    std::string getSelectedOption() const {
        return options[selectedIndex];
    }

private:
    Position position;
    std::vector<std::string> options;
    DropdownState state;
    std::size_t selectedIndex;
    int cellWidth; 
    int color_pair;
    int color_pair_selected;


    int calculateMaxOptionWidth() const {
        int maxWidth = 0;
        for (const auto& option : options) {
            if (option.length() > maxWidth) {
                maxWidth = option.length();
            }
        }
        return maxWidth;
    }

    void handleClosedState(Position mouse_pos) {
        if (mouse_pos.y == position.y && mouse_pos.x >= position.x && mouse_pos.x < position.x + cellWidth) {
            state = DropdownState::Open;
            draw(true);
        }
    }

    void handleOpenState(Position mouse_pos) {
        int optionIndex = mouse_pos.y - position.y - 1;
        if (optionIndex >= 0 && optionIndex < options.size() && mouse_pos.x >= position.x && mouse_pos.x < position.x + cellWidth) {
            selectedIndex = optionIndex;
            state = DropdownState::OptionSelected;
            clearDropdownOptions(); // Clear the dropdown area before closing
            state = DropdownState::Closed;
            draw(true);
        } else {
            clearDropdownOptions(); // Clear the dropdown area before closing
            state = DropdownState::Closed;
            draw(true);
        }
    }

    void handleOptionSelectedState(Position mouse_pos) {
        clearDropdownOptions(); // Clear the dropdown area before closing
        state = DropdownState::Closed;
        draw(true);
    }

    void clearDropdownOptions() const {
        for (std::size_t i = 0; i < options.size(); ++i) {
            mvprintw(position.y + i + 1, position.x, "%-*s", cellWidth, ""); // Clear each line
        }
    }
};

#endif // DROPDOWNWIDGET_H
