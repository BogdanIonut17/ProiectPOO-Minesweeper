//
// Created by Windows on 29.06.2025.
//

#include "ClassicGame.h"

#include <iostream>

#include "GameFactory.h"
#include "HighScores.h"


int ClassicGame::classicHighScore = 0;

ClassicGame::ClassicGame() : Game(Minefield(8, 8, 9), std::make_shared<Player>("Player1"), std::chrono::minutes(4))
{
}

int ClassicGame::getHighScore() const
{
    return classicHighScore;
}

void ClassicGame::updateHighScore()
{
    if (getPlayerScore() > classicHighScore)
    {
        classicHighScore = getPlayerScore();
        const GameFactory<HighScore>* factory = new ClassicHighscoreFactory();
        getPlayer()->addOrUpdateHighScore(factory->createHighScore(getHighScore()));
        delete factory;
        // const auto hs = std::make_shared<ClassicHighScore>(getHighScore());
        // getPlayer()->addOrUpdateHighScore(hs);
    }
}

void ClassicGame::setupDifficultyRound()
{
    std::cout << "Classic mode: 8x8 board with 9 mines" << std::endl;
    getMinefield().setFieldSize(8, 8, 9);
}

void ClassicGame::displayDifficulty(std::ostream& os) const
{
    os << "Classic" << std::endl;
}

std::shared_ptr<Game> ClassicGame::clone() const
{
    return std::make_shared<ClassicGame>(*this);
}
