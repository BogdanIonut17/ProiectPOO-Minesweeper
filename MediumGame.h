#ifndef MEDIUMGAME_H
#define MEDIUMGAME_H


#include "Game.h"


class MediumGame : public Game{
    static int mediumHighScore;

public:
    MediumGame();

private:
    [[nodiscard]] int getHighScore() const override;

    void updateHighScore() override;

    [[nodiscard]] std::string getDifficulty() const override;

    void setupDifficultyRound() override;

    void displayDifficulty(std::ostream&) const override;

    [[nodiscard]] std::shared_ptr<Game> clone() const override;
};


#endif //MEDIUMGAME_H
