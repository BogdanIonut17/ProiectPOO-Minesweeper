//
// Created by Windows on 29.06.2025.
//

#ifndef CLASSICGAME_H
#define CLASSICGAME_H


#include "Game.h"


class ClassicGame : public Game
{
    static int classicHighScore;

public:
    ClassicGame();

private:
    [[nodiscard]] int getHighScore() const override;

    void updateHighScore() override;

    [[nodiscard]] std::string getDifficulty() const override;

    void setupDifficultyRound() override;

    void displayDifficulty(std::ostream&) const override;

    [[nodiscard]] std::shared_ptr<Game> clone() const override;
};


#endif //CLASSICGAME_H
