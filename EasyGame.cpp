#include "EasyGame.h"

#include <iostream>

#include "HighScores.h"


int EasyGame::easyHighScore = 0;

EasyGame::EasyGame() : Game(Minefield(9, 9, 10), std::make_shared<Player>("Player1"), std::chrono::minutes(3))
{
}

int EasyGame::getHighScore() const
{
    return easyHighScore;
}

void EasyGame::updateHighScore()
{
    if (getPlayerScore() > easyHighScore)
    {
        easyHighScore = getPlayerScore();
        const auto hs = std::make_shared<EasyHighScore>(getHighScore());
        getPlayer()->addOrUpdateHighScore(hs);
    }
}

void EasyGame::setupDifficultyRound()
{
    std::cout << "Easy mode: 9x9 board with 10 mines" << std::endl;
    getMinefield().setFieldSize(9, 9, 10);
}

void EasyGame::displayDifficulty(std::ostream& os) const
{
    os << "Easy" << std::endl;
}

std::shared_ptr<Game> EasyGame::clone() const
{
    return std::make_shared<EasyGame>(*this);
}