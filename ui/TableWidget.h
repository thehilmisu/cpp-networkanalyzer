#ifndef TABLEWIDGET_H
#define TABLEWIDGET_H

#include <ncurses.h>
#include <vector>
#include <string>
#include <stdexcept>
#include "UIElement.h"

class TableWidget : public UIElement {
public:
    TableWidget(Position pos, int cols, int visibleRows = 10)
        : position(pos), cols(cols), padding(2), visibleRows(visibleRows), scrollOffset(0) {
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

    void removeAllRows(int rowIndex) {
        content.clear();
    }

    void setItem(int row, int col, const std::string &text) {
        if (row < content.size() && col < cols) {
            content[row][col] = text;
        }
    }

    void draw(bool selected = false) override {
        // Get terminal size
        int termRows, termCols;
        getmaxyx(stdscr, termRows, termCols);

        // Calculate total available width for the table
        int scrollbarWidth = (content.size() > visibleRows) ? 1 : 0; // Space for scrollbar
        int totalTableWidth = termCols - position.x - scrollbarWidth - 1; // Subtract position and margin

        // Calculate cell width
        cellWidth = totalTableWidth / cols;
        if (cellWidth < 5) {
            cellWidth = 5; // Minimum cell width
        }

        drawHeaders();
        drawCells(selected);
        refresh(); // Refresh the screen after drawing
    }

    void setPosition(Position pos) override {
        position = pos;
    }

    bool handleMouseClick(Position mouse_pos) override {
        int relativeY = mouse_pos.y - position.y - 2; // Adjust for headers and line
        int row = relativeY / 2 + scrollOffset;       // Calculate actual row index
        int col = (mouse_pos.x - position.x) / cellWidth;

        if (relativeY >= 0 && relativeY < visibleRows * 2 && row >= 0 && row < content.size() && col >= 0 && col < cols) {
            selectedRow = row;
            selectedCol = col;
            draw(true);
            return true;
        }
        return false;
    }

    bool handleMouseEvent(const MEVENT& event) override {
        int mouseX = event.x;
        int mouseY = event.y;

        int widgetLeft = position.x;
        int widgetRight = position.x + cols * cellWidth;
        int widgetTop = position.y;
        int widgetBottom = position.y + visibleRows * 2 + 2;

        // Handle mouse wheel events (if supported)
        if (event.bstate & BUTTON4_PRESSED) {
            // Scroll up
            scrollBy(-1);
            draw(true);
            return true;
        } else if (event.bstate & BUTTON5_PRESSED) {
            // Scroll down
            scrollBy(1);
            draw(true);
            return true;
        }

        if (mouseX >= widgetLeft && mouseX < widgetRight && mouseY >= widgetTop && mouseY <= widgetBottom) {
        // Proceed to handle the mouse event
            // Check if click is on the scrollbar
            int scrollbarX = position.x + cols * cellWidth; // Position of scrollbar
            if (mouseX == scrollbarX && mouseY >= position.y + 2 && mouseY <= position.y + 2 + visibleRows * 2) {
                if (event.bstate & BUTTON1_PRESSED) {
                    handleScrollbarClick(mouseY);
                    draw(true);
                    return true;
                } else if (event.bstate & BUTTON1_RELEASED) {
                    dragging = false;
                } else if (dragging && (event.bstate & REPORT_MOUSE_POSITION)) {
                    handleScrollbarClick(mouseY);
                    draw(true);
                    return true;
                }
            }
            return true; // Event was handled
        }

        // Handle clicks within table cells
        return handleMouseClick({mouseX, mouseY});
    }


    void activate() override {
        editItem("Activated");
    }

    void navigate(int dRow, int dCol = 0) {
        if (!content.empty()) {
            selectedRow = (selectedRow + dRow + content.size()) % content.size();
            selectedCol = (selectedCol + dCol + cols) % cols;

            // Adjust scrollOffset if necessary
            if (selectedRow < scrollOffset) {
                scrollOffset = selectedRow;
            } else if (selectedRow >= scrollOffset + visibleRows) {
                scrollOffset = selectedRow - visibleRows + 1;
            }

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
    Position position;
    int cols;
    int cellWidth;
    int padding;
    int selectedRow, selectedCol;
    int visibleRows;
    int scrollOffset;
    std::vector<std::vector<std::string>> content;
    std::vector<std::string> headers;
    bool dragging = false;

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
        int startRow = scrollOffset;
        int endRow = std::min(scrollOffset + visibleRows, static_cast<int>(content.size()));

        for (int i = startRow; i < endRow; ++i) {
            int drawRow = (i - scrollOffset) * 2 + 2; // Adjust for headers and lines
            drawCell(drawRow, i, selected && i == selectedRow);
            drawDashedLine(drawRow + 1);
        }

        // Draw scrollbar if content exceeds visible rows
        if (content.size() > visibleRows) {
            drawScrollbar();
        }
    }

    void drawCell(int row, int index, bool isSelected) const {
        for (int col = 0; col < cols; ++col) {
            int x = position.x + col * cellWidth + padding;
            int y = position.y + row;

            if (isSelected && col == selectedCol) {
                attron(A_REVERSE | COLOR_PAIR(GREEN_ON_BLACK)); // Highlight selected cell
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

    void drawScrollbar() const {
        int scrollbarHeight = visibleRows * 2;
        int scrollbarX = position.x + cols * cellWidth;

        float ratio = static_cast<float>(visibleRows) / content.size();
        int thumbSize = std::max(1, static_cast<int>(scrollbarHeight * ratio));
        float scrollRatio = static_cast<float>(scrollOffset) / (content.size() - visibleRows);
        int thumbPosition = position.y + 2 + static_cast<int>((scrollbarHeight - thumbSize) * scrollRatio);

        // Draw scrollbar background
        for (int y = position.y + 2; y < position.y + scrollbarHeight + 2; ++y) {
            mvaddch(y, scrollbarX, ACS_VLINE);
        }

        // Draw scrollbar thumb
        for (int y = thumbPosition; y < thumbPosition + thumbSize; ++y) {
            mvaddch(y, scrollbarX, ACS_CKBOARD);
        }
    }


    void handleScrollbarClick(int mouseY) {
        int scrollbarHeight = visibleRows * 2;
        int scrollbarTop = position.y + 2;

        int clickPosition = mouseY - scrollbarTop;
        float clickRatio = static_cast<float>(clickPosition) / scrollbarHeight;

        scrollOffset = static_cast<int>(clickRatio * (content.size() - visibleRows + 1));
        if (scrollOffset < 0) scrollOffset = 0;
        if (scrollOffset > content.size() - visibleRows) scrollOffset = content.size() - visibleRows;
    }

    void scrollBy(int delta) {
        scrollOffset += delta;
        if (scrollOffset < 0) scrollOffset = 0;
        if (scrollOffset > content.size() - visibleRows) scrollOffset = content.size() - visibleRows;
    }
};

#endif // TABLEWIDGET_H
