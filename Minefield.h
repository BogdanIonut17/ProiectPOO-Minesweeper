#ifndef MINEFIELD_H
#define MINEFIELD_H

#include <vector>



#include "Cell.h"


class Minefield
{
    int rows, cols, mineCount;
    std::vector<std::vector<Cell>> grid;
    bool firstMove;

    [[nodiscard]] bool isValidMove(int x, int y) const;

    void generateMines(int firstX, int firstY);

    void countAdjacentMines();

    [[nodiscard]] int countFlags() const;

    void revealCell(int cellX, int cellY);

    void BFSReveal(int startX, int startY);

    void chordReveal(int x, int y);

    void flagCell(int x, int y);

    void shuffleMines();

public:
    Minefield(int rows, int cols, int mineCount);

    [[nodiscard]] int getRows() const;

    [[nodiscard]] int getCols() const;

    [[nodiscard]] const Cell& getCell(int x, int y) const;

    void setFirstMove();

    void setFieldSize(int newRows, int newCols, int newMineCount);

    void resetField();

    void processMove();

    friend std::ostream& operator<<(std::ostream& os, const Minefield& minefield);
};




#endif //MINEFIELD_H
