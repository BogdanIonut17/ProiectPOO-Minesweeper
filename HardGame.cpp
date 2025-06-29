#include "HardGame.h"

#include <iostream>

#include "HighScores.h"


int HardGame::hardHighScore = 0;

HardGame::HardGame() : Game(Minefield(9, 9, 10), std::make_shared<Player>("Player1"), std::chrono::minutes(3))
{
}

int HardGame::getHighScore() const
{
    return hardHighScore;
}

void HardGame::updateHighScore()
{
    if (getPlayerScore() > hardHighScore)
    {
        hardHighScore = getPlayerScore();
        const auto hs = std::make_shared<HardHighScore>(getHighScore());
        getPlayer()->addOrUpdateHighScore(hs);
    }
}

void HardGame::setupDifficultyRound()
{
    std::cout << "Hard mode: 16x30 board with 99 mines" << std::endl;
    getMinefield().setFieldSize(16, 30, 99);
}

void HardGame::displayDifficulty(std::ostream& os) const
{
    os << "Hard" << std::endl;
}

std::shared_ptr<Game> HardGame::clone() const
{
    return std::make_shared<HardGame>(*this);
}