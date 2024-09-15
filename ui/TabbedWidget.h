#ifndef TABBEDWIDGET_H
#define TABBEDWIDGET_H

#include <ncurses.h>
#include <vector>
#include <string>
#include <map>
#include <stdexcept>
#include "UIElement.h"

class TabbedWidget : public UIElement {
public:
    TabbedWidget(Position pos, const std::vector<std::string>& tabLabels)
        : position(pos), tabs(tabLabels), selectedTab(0) {
        if (tabs.empty()) {
            throw std::runtime_error("Tab labels cannot be empty.");
        }
    }

    void draw(bool selected = false) override {
        drawTabs(selected);
        drawCurrentTabWidgets();
        refresh(); // Refresh the screen after drawing
    }

    void setPosition(Position pos) override {
        position = pos;
    }

    bool handleMouseClick(Position mouse_pos) override {
        for (std::size_t i = 0; i < tabs.size(); ++i) {
            int startX = position.x + i * (tabWidth + tabPadding);
            if (mouse_pos.y == position.y && mouse_pos.x >= startX && mouse_pos.x < startX + tabWidth) {
                selectedTab = i;
                draw(true);
                return true;
            }
        }
        // Forward mouse click to the active tab's widgets
        return handleTabWidgetsMouseClick(mouse_pos);
    }

    void activate() override {
        // Handle activation of the selected tab (if needed)
    }

    void navigate(int direction) {
        selectedTab = (selectedTab + direction + tabs.size()) % tabs.size();
        draw(true);
    }

    void addWidgetToTab(int tabIndex, UIElement* widget) {
        if (tabIndex < 0 || tabIndex >= tabs.size()) {
            throw std::out_of_range("Tab index out of range.");
        }
        tabWidgets[tabIndex].push_back(widget);
    }

    int getSelectedTab() const {
        return selectedTab;
    }

private:
    Position position;  // Position in the terminal window
    std::vector<std::string> tabs;
    std::size_t selectedTab;
    int tabWidth = 10; // Fixed width for each tab
    int tabPadding = 2; // Padding between tabs
    std::map<int, std::vector<UIElement*>> tabWidgets; // Widgets for each tab

    void drawTabs(bool selected) const {
        for (std::size_t i = 0; i < tabs.size(); ++i) {
            int startX = position.x + i * (tabWidth + tabPadding);
            if (i == selectedTab) {
                attron(A_REVERSE | COLOR_PAIR(2)); // Highlight the selected tab
            }

            mvprintw(position.y, startX, "%-*s", tabWidth, tabs[i].c_str());

            if (i == selectedTab) {
                attroff(A_REVERSE | COLOR_PAIR(2));
            }
        }
    }

    void drawCurrentTabWidgets() const {
        auto it = tabWidgets.find(selectedTab);
        if (it != tabWidgets.end()) {
            for (auto widget : it->second) {
                widget->draw();
            }
        }
    }

    bool handleTabWidgetsMouseClick(Position mouse_pos) {
        auto it = tabWidgets.find(selectedTab);
        if (it != tabWidgets.end()) {
            for (auto widget : it->second) {
                if (widget->handleMouseClick(mouse_pos)) {
                    return true;
                }
            }
        }
        return false;
    }
};

#endif // TABBEDWIDGET_H
