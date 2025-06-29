#include "Minefield.h"
#include "Cell.h"
#include "Exceptions.h"

#include <iomanip>
#include <iostream>
#include <queue>
#include <algorithm>
#include <random>


bool Minefield::isValidMove(const int x, const int y) const
{
    return x >= 0 && x < rows && y >= 0 && y < cols;
}

void Minefield::generateMines(const int firstX, const int firstY)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution distRow(0, rows - 1);
    std::uniform_int_distribution distCol(0, cols - 1);
    int minesPlaced = 0;
    const int totalMineCount = mineCount + countFlags();
    while (minesPlaced < totalMineCount)
    {
        const int mineX = distRow(gen);
        const int mineY = distCol(gen);
        if ((mineX == firstX && mineY == firstY) || grid[mineX][mineY].isMine())
            continue;
        bool flagged = false;
        if (grid[mineX][mineY].checkIfFlagged())
            flagged = true;
        grid[mineX][mineY] = Cell(true);
        if (flagged)
        {
            grid[mineX][mineY].toggleFlag();
        }
        minesPlaced++;
    }
}

void Minefield::countAdjacentMines()
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (grid[i][j].isMine()) continue;
            int count = 0;
            for (int dx = -1; dx <= 1; dx++)
            {
                for (int dy = -1; dy <= 1; dy++)
                {
                    int x = i + dx, y = j + dy;
                    if (isValidMove(x, y) && grid[x][y].isMine())
                        count++;
                }
            }
            grid[i][j].setAdjacentMines(count);
        }
    }
}

int Minefield::countFlags() const
{
    int flagCount = 0;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (grid[i][j].checkIfFlagged())
            {
                flagCount++;
            }
        }
    }
    return flagCount;
}

// Reveals cells until a mine is found or a cell that has mine(s) adjacent to it
void Minefield::BFSReveal(int startX, int startY)
{
    if (!isValidMove(startX, startY))
    {
        return;
    }

    std::queue<std::pair<int, int>> q;
    q.emplace(startX, startY);
    std::vector visited(rows, std::vector(cols, false));

    while (!q.empty())
    {
        auto [x, y] = q.front();
        q.pop();

        if (visited[x][y]) continue;
        visited[x][y] = true;
        grid[x][y].reveal();

        if (grid[x][y].getAdjacentMines() > 0) continue;

        for (int dx = -1; dx <= 1; dx++)
        {
            for (int dy = -1; dy <= 1; dy++)
            {
                int adjacentX = x + dx, adjacentY = y + dy;
                if (isValidMove(adjacentX, adjacentY) && !grid[adjacentX][adjacentY].checkIfRevealed() && !visited[
                    adjacentX][adjacentY])
                {
                    q.emplace(adjacentX, adjacentY);
                }
            }
        }
    }
}

// When a revealed cell has as many adjacent flags as the number of mines displayed on it,
// revealing it will reveal the cells next to it that are not flagged
void Minefield::chordReveal(const int x, const int y)
{
    int flagCount = 0;
    std::vector<std::pair<int, int>> toReveal;

    for (int dx = -1; dx <= 1; dx++)
    {
        for (int dy = -1; dy <= 1; dy++)
        {
            int adjacentX = x + dx, adjacentY = y + dy;
            if (isValidMove(adjacentX, adjacentY))
            {
                if (grid[adjacentX][adjacentY].checkIfFlagged())
                {
                    flagCount++;
                }
                else if (!grid[adjacentX][adjacentY].checkIfRevealed())
                {
                    toReveal.emplace_back(adjacentX, adjacentY);
                }
            }
        }
    }

    if (flagCount == grid[x][y].getAdjacentMines())
    {
        for (const auto& [adjX, adjY] : toReveal)
        {
            revealCell(adjX, adjY);
        }
    }
}

Minefield::Minefield(const int rows, const int cols, const int mineCount): rows(rows), cols(cols),
                                                                           mineCount(mineCount),
                                                                           grid(rows, std::vector<Cell>(cols)),
                                                                           firstMove(true)
{
}

int Minefield::getRows() const
{ return rows; }

int Minefield::getCols() const
{ return cols; }

const Cell& Minefield::getCell(const int x, const int y) const
{ return grid[x][y]; }

void Minefield::setFirstMove()
{
    firstMove = true;
}

void Minefield::revealCell(const int cellX, const int cellY)
{
    if (grid[cellX][cellY].checkIfFlagged())
        return;
    if (grid[cellX][cellY].checkIfRevealed() && grid[cellX][cellY].getAdjacentMines() > 0)
    {
        chordReveal(cellX, cellY);
        return;
    }
    if (firstMove)
    {
        generateMines(cellX, cellY);
        countAdjacentMines();
        firstMove = false;
    }
    if (grid[cellX][cellY].getAdjacentMines() == 0 && !grid[cellX][cellY].isMine())
    {
        BFSReveal(cellX, cellY);
    }
    else
    {
        grid[cellX][cellY].reveal();
    }
}

void Minefield::shuffleMines()
{
    std::vector<std::pair<int, int>> emptyCells;
    std::vector<std::pair<int, int>> mines;

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
                if (grid[i][j].checkIfFlagged())
                {
                    flagCell(i, j);
                }
                if (grid[i][j].isMine())
                {
                    mines.emplace_back(i, j);
                }
                else if (!grid[i][j].checkIfRevealed())
                {
                    emptyCells.emplace_back(i, j);
                }
        }
    }

    if (emptyCells.size() < mines.size() || mines.empty())
    {
        return;
    }

    for (auto& [x, y] : mines)
    {
        grid[x][y] = Cell(false);
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::ranges::shuffle(emptyCells, gen);

    for (int i = 0; i < mineCount; i++)
    {
        auto [x, y] = emptyCells[i];
        grid[x][y] = Cell(true);
    }

    countAdjacentMines();
}

void Minefield::setFieldSize(const int newRows, const int newCols, const int newMineCount)
{
    rows = newRows;
    cols = newCols;
    mineCount = newMineCount;
    grid = std::vector(rows, std::vector<Cell>(cols));
}

void Minefield::resetField()
{
    firstMove = false;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            grid[i][j].cover();
            if (grid[i][j].checkIfFlagged())
            {
                flagCell(i, j);
            }
        }
    }
}

void Minefield::flagCell(const int x, const int y)
{
    if (!grid[x][y].checkIfRevealed())
    {
        grid[x][y].toggleFlag();
        if (grid[x][y].checkIfFlagged())
        {
            mineCount--;
        }
        else
        {
            mineCount++;
        }
    }
}

void Minefield::processMove()
{
    int cellX, cellY;
    char action;
    std::cout << "Enter a move (R x y to reveal, F x y to flag/unflag a cell or S to shuffle the mines): " <<
        std::endl;
    std::cin >> action;
    if (std::toupper(action) != 'R' && std::toupper(action) != 'F' && std::toupper(action) != 'S')
    {
        std::cout << "Invalid action! Enter R to reveal, F to flag/unflag a cell or S to shuffle the mines: "
            << std::endl;
        return;
    }
    if (std::toupper(action) == 'S')
    {
        shuffleMines();
        return;
    }
    // std::cin >> cellX >> cellY;

    if (!(std::cin >> cellX >> cellY))
    {
        throw InputReadError();
    }

    if (!isValidMove(cellX, cellY))
    {
        std::cout << "Invalid cell coordinates! Try again!" << std::endl;
        return;
    }
    if (std::toupper(action) == 'R') revealCell(cellX, cellY);
    else if (std::toupper(action) == 'F') flagCell(cellX, cellY);
}

std::ostream& operator<<(std::ostream& os, const Minefield& minefield)
{
    os << "\nMinefield: " << std::endl;
    os << "   ";
    for (int j = 0; j < minefield.cols; j++)
    {
        os << std::setw(3) << j;
    }
    os << std::endl;

    for (int i = 0; i < minefield.rows; i++)
    {
        os << std::setw(3) << i;
        for (int j = 0; j < minefield.cols; j++)
        {
            os << std::setw(3) << minefield.grid[i][j];
        }
        os << std::endl;
    }

    os << "\nNumber of mines: " << minefield.mineCount << std::endl;
    return os;
}
