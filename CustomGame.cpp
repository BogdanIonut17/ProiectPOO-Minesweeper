//
// Created by Windows on 18.05.2025.
//

#include "CustomGame.h"
#include "Exceptions.h"
#include <iostream>


int CustomGame::customHighScore = 0;

CustomGame::CustomGame() : Game(Minefield(8, 8, 9), Player("Player1"), std::chrono::minutes(3)), savedRows(8),
                           savedCols(8), savedMineCount(9), customRoundDuration(std::chrono::minutes(3)),
                           isReplay(false)
{
}

int CustomGame::getHighScore() const
{
    return customHighScore;
}

void CustomGame::updateHighScore()
{
    if (getPlayerScore() > customHighScore)
        customHighScore = getPlayerScore();
}

void CustomGame::markReplay()
{
    isReplay = true;
}

void CustomGame::setupRound()
{
    if (isReplay)
    {
        std::cout << "Replaying previous custom configuration: "
            << savedRows << "x" << savedCols
            << " board with " << savedMineCount << " mines." << std::endl;

        getMinefield().setFieldSize(savedRows, savedCols, savedMineCount);
        isReplay = false;
        return;
    }

    int newRows = 0, newCols = 0, newMineCount = 0;
    std::cout << "Enter board size (rows, cols) and number of mines: " << std::endl;
    // std::cin >> newRows >> newCols >> newMineCount;
    if (!(std::cin >> newRows >> newCols >> newMineCount))
        throw InputReadError();

    if (!isValidConfiguration(newRows, newCols, newMineCount))
    {
        throw InvalidConfiguration(newRows, newCols, newMineCount);
        // std::cout << "Invalid board configuration! Defaulting to 8x8 with 9 mines." << std::endl;
        // newRows = 8;
        // newCols = 8;
        // newMineCount = 9;
    }

    int minutes;
    std::cout << "Enter round duration in minutes: " << std::endl;
    // std::cin >> minutes;
    if (!(std::cin >> minutes)) {
        throw InputReadError();
    }
    if (minutes <= 0 || minutes > 59)
    {
        throw InvalidRoundDuration(minutes);
        // std::cout << "Invalid round duration. Defaulting to 3 minutes." << std::endl;
        // minutes = 3;
    }
    customRoundDuration = std::chrono::minutes(minutes);

    std::cout << "Custom mode: " << newRows << "x" << newCols
        << " board with " << newMineCount << " mines." << std::endl;

    savedRows = newRows;
    savedCols = newCols;
    savedMineCount = newMineCount;

    getMinefield().setFieldSize(savedRows, savedCols, savedMineCount);
    setRoundDuration(customRoundDuration);
}

void CustomGame::displayMode(std::ostream& os) const
{
    os << "Custom" << std::endl;
}

std::shared_ptr<Game> CustomGame::clone() const
{
    return std::make_shared<CustomGame>(*this);
}
