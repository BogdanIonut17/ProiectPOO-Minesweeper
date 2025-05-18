#include "EasyGame.h"
#include <iostream>
#include <thread>


int EasyGame::easyHighScore = 0;

EasyGame::EasyGame() : Game(Minefield(9, 9, 10), Player("Player1"), std::chrono::minutes(3))
{
}

int EasyGame::getHighScore() const
{
    return easyHighScore;
}

void EasyGame::updateHighScore()
{
    if (getPlayerScore() > easyHighScore)
        easyHighScore = getPlayerScore();
}

void EasyGame::setupRound()
{
    std::cout << "Easy mode: 9x9 board with 10 mines" << std::endl;
    getMinefield().setFieldSize(9, 9, 10);
}

void EasyGame::displayMode(std::ostream& os) const
{
    os << "Easy" << std::endl;
}

std::shared_ptr<Game> EasyGame::clone() const
{
    return std::make_shared<EasyGame>(*this);
}