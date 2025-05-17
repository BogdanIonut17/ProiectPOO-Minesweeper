#include <iostream>
#include "Cell.h"

Cell::Cell(const bool hasMine): hasMine(hasMine), isRevealed(false), isFlagged(false), adjacentMines(0)
{
}

Cell::~Cell()
{
}

Cell::Cell(const Cell& other): hasMine(other.hasMine), isRevealed(other.isRevealed), isFlagged(other.isFlagged),
                               adjacentMines(other.adjacentMines)
{
}

Cell& Cell::operator=(const Cell& other)
{
    if (this == &other)
        return *this;
    hasMine = other.hasMine;
    isRevealed = other.isRevealed;
    isFlagged = other.isFlagged;
    adjacentMines = other.adjacentMines;
    return *this;
}

bool Cell::isMine() const
{
    return hasMine;
}

bool Cell::checkIfRevealed() const
{
    return isRevealed;
}

bool Cell::checkIfFlagged() const
{
    return isFlagged;
}

void Cell::reveal()
{
    isRevealed = true;
}

void Cell::cover()
{
    isRevealed = false;
}

void Cell::toggleFlag()
{
    isFlagged = !isFlagged;
}

void Cell::setAdjacentMines(const int adjacent_mines)
{
    adjacentMines = adjacent_mines;
}

int Cell::getAdjacentMines() const
{
    return adjacentMines;
}

std::ostream& operator<<(std::ostream& os, const Cell& cell)
{
    if (cell.isRevealed)
    {
        if (cell.hasMine) os << "*";
        else if (cell.adjacentMines)
        {
            os << cell.adjacentMines;
        }
        else
            os << ".";
    }
    else if (cell.isFlagged)
    {
        os << "F";
    }
    else
    {
        os << "#";
    }
    return os;
}
