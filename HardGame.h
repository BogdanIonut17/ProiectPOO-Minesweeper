#ifndef HARDGAME_H
#define HARDGAME_H
#include "Game.h"


class HardGame : public Game
{
    static int hardHighScore;

public:
    HardGame();

    [[nodiscard]] int getHighScore() const override;

    void updateHighScore() override;

    void setupRound() override;

    void displayMode(std::ostream&) const override;

    [[nodiscard]] std::shared_ptr<Game> clone() const override;
};


#endif //HARDGAME_H
