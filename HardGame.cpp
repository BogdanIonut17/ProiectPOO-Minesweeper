#include "HardGame.h"

#include <iostream>
#include <thread>


int HardGame::hardHighScore = 0;

HardGame::HardGame() : Game(Minefield(16, 30, 99), Player("Player1"), std::chrono::minutes(10))
{
}

int HardGame::getHighScore() const
{
    return hardHighScore;
}

void HardGame::updateHighScore()
{
    if (getPlayerScore() > hardHighScore)
        hardHighScore = getPlayerScore();
}

void HardGame::setupDifficultyRound()
{
    std::cout << "Hard mode: 16x30 board with 99 mines" << std::endl;
    getMinefield().setFieldSize(16, 30, 99);
}

void HardGame::displayMode(std::ostream& os) const
{
    os << "Hard" << std::endl;
}

std::shared_ptr<Game> HardGame::clone() const
{
    return std::make_shared<HardGame>(*this);
}

