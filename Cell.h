#ifndef CELL_H
#define CELL_H

#include <iosfwd>

class Cell
{
    bool hasMine;
    bool isRevealed;
    bool isFlagged;
    int adjacentMines;

public:
    explicit Cell(bool hasMine = false);

    Cell(const Cell& other);

    Cell& operator=(const Cell& other);

    ~Cell();

    friend std::ostream& operator<<(std::ostream& os, const Cell& cell);

    [[nodiscard]] bool isMine() const;

    [[nodiscard]] bool checkIfRevealed() const;

    [[nodiscard]] bool checkIfFlagged() const;

    void reveal();

    void cover();

    void toggleFlag();

    void setAdjacentMines(int adjacent_mines);

    [[nodiscard]] int getAdjacentMines() const;
};




#endif //CELL_H
