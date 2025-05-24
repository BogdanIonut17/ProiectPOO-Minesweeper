#include "MediumGame.h"

#include <iostream>
#include <thread>


int MediumGame::mediumHighScore = 0;

MediumGame::MediumGame() : Game(Minefield(16, 16, 40), Player("Player1"), std::chrono::minutes(5))
{
}

int MediumGame::getHighScore() const
{
    return mediumHighScore;
}

void MediumGame::updateHighScore()
{
    if (getPlayerScore() > mediumHighScore)
        mediumHighScore = getPlayerScore();
}

void MediumGame::setupDifficultyRound()
{
    std::cout << "Medium mode: 16x16 board with 40 mines" << std::endl;
    getMinefield().setFieldSize(16, 16, 40);
}

void MediumGame::displayMode(std::ostream& os) const
{
    os << "Medium" << std::endl;
}


std::shared_ptr<Game> MediumGame::clone() const
{
    return std::make_shared<MediumGame>(*this);
}
