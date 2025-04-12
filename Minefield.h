//
// Created by Windows on 12.04.2025.
//

#ifndef MINEFIELD_H
#define MINEFIELD_H
#include <vector>



#include "Cell.h"


class Minefield
{
private:
    int rows, cols, mineCount;
    std::vector<std::vector<Cell>> grid;
    bool firstMove;

    [[nodiscard]] bool isValidMove(int x, int y) const;

    void generateMines(int firstX, int firstY);

    void countAdjacentMines();

    [[nodiscard]] int countFlags() const;

    void BFSReveal(int startX, int startY);

    void chordReveal(int x, int y);

public:
    Minefield(int rows, int cols, int mineCount);

    [[nodiscard]] int getRows() const;

    [[nodiscard]] int getCols() const;

    Cell& getCell(int x, int y);

    [[nodiscard]] const Cell& getCell(int x, int y) const;

    void setFirstMove();

    void revealCell(int cellX, int cellY);

    void shuffleMines();

    void setFieldSize(int newRows, int newCols, int newMineCount);

    void flagCell(int x, int y);

    void processMove();

    friend std::ostream& operator<<(std::ostream& os, const Minefield& minefield);
};




#endif //MINEFIELD_H
