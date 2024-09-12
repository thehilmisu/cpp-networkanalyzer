#ifndef TABLEWIDGET_H
#define TABLEWIDGET_H

#include <ncurses.h>
#include <vector>
#include <string>
#include <stdexcept>
#include "UIElement.h"

class TableWidget : public UIElement {
public:
    TableWidget(Position pos, std::size_t cols)
        : position(pos), cols(cols), cellWidth(20), padding(2) {
        selectedRow = 0;
        selectedCol = 0;
    }

    void setHeaders(const std::vector<std::string> &headerLabels) {
        headers = headerLabels;
        if (headers.size() != cols) {
            throw std::runtime_error("Header labels count does not match the number of columns.");
        }
    }

    void addRow(const std::vector<std::string> &rowData) {
        if (rowData.size() != cols) {
            throw std::runtime_error("Row data does not match the number of columns.");
        }
        content.push_back(rowData);
    }

    void removeRow(int rowIndex) {
        if (rowIndex >= 0 && rowIndex < content.size()) {
            content.erase(content.begin() + rowIndex);
            if (selectedRow >= content.size()) {
                selectedRow = content.size() - 1;
            }
        }
    }

    void setItem(int row, int col, const std::string &text) {
        if (row < content.size() && col < cols) {
            content[row][col] = text;
        }
    }

    void draw(bool selected = false) const override {
        drawHeaders();
        drawCells(selected);
        refresh(); // Refresh the screen after drawing
    }

    void setPosition(Position pos) override {
        position = pos;
    }

    bool handleMouseClick(Position mouse_pos) override {
        int row = (mouse_pos.y - position.y - 2) / 2;  // Adjust for header row, line, and dashed lines
        int col = (mouse_pos.x - position.x) / cellWidth;
        if (row >= 0 && row < content.size() && col >= 0 && col < cols) {
            selectedRow = row;
            selectedCol = col;
            draw(true);
            return true;
        }
        return false;
    }

    void activate() override {
        editItem("Activated");
    }

    void navigate(int dRow, int dCol) {
        if (!content.empty()) {
            selectedRow = (selectedRow + dRow + content.size()) % content.size();
            selectedCol = (selectedCol + dCol + cols) % cols;
            draw(true);
        }
    }

    void editItem(const std::string &text) {
        if (!content.empty()) {
            content[selectedRow][selectedCol] = text;
            draw(true);
        }
    }

private:
    Position position;  // Position in the terminal window
    std::size_t cols;
    int cellWidth;
    int padding;
    int selectedRow, selectedCol;
    std::vector<std::vector<std::string>> content;
    std::vector<std::string> headers;

    void drawHeaders() const {
        attron(A_REVERSE | COLOR_PAIR(GREEN_ON_BLACK)); // Bold and different color for headers
        for (int j = 0; j < cols; ++j) {
            int x = position.x + j * cellWidth + padding;
            mvprintw(position.y, x, "%-*s", cellWidth - 2 * padding, headers[j].c_str());
        }
        attroff(A_REVERSE | COLOR_PAIR(GREEN_ON_BLACK));

        // Draw a line under the headers
        mvhline(position.y + 1, position.x, ACS_HLINE, cols * cellWidth);
    }

    void drawCells(bool selected) const {
        for (std::size_t i = 0; i < content.size(); ++i) {
            drawCell(i * 2 + 2, i, selected); // Shift by 2 per row to accommodate headers and dashed lines
            drawDashedLine(i * 2 + 3);        // Draw dashed line after each row
        }
    }

    void drawCell(int row, int index, bool selected) const {
        for (int col = 0; col < cols; ++col) {
            int x = position.x + col * cellWidth + padding;
            int y = position.y + row;

            if (selected && index == selectedRow) {
                attron(A_REVERSE | COLOR_PAIR(GREEN_ON_BLACK)); // Highlight selected row
                mvprintw(y, x, "%-*s", cellWidth - 2 * padding, content[index][col].c_str());
                attroff(A_REVERSE | COLOR_PAIR(GREEN_ON_BLACK));
            } else {
                attron(COLOR_PAIR(GREEN_ON_BLACK)); 
                mvprintw(y, x, "%-*s", cellWidth - 2 * padding, content[index][col].c_str());
                attroff(COLOR_PAIR(GREEN_ON_BLACK)); 
            }
        }
    }

    void drawDashedLine(int row) const {
        int y = position.y + row; // Position the line below the current row
        for (int x = position.x; x < position.x + cols * cellWidth; ++x) {
            if (x % 2 == 0) { // Creating a dashed effect
                mvaddch(y, x, '-');
            }
        }
    }
};

#endif // TABLEWIDGET_H
