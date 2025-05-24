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

    void setupDifficultyRound() override;

    void displayMode(std::ostream&) const override;

    [[nodiscard]] std::shared_ptr<Game> clone() const override;
};



#endif //MEDIUMGAME_H
