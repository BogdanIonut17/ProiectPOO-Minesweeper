//
// Created by Windows on 18.05.2025.
//

#ifndef CUSTOMGAME_H
#define CUSTOMGAME_H
#include "Game.h"


class CustomGame : public Game
{
    static int customHighScore;
    int savedRows;
    int savedCols;
    int savedMineCount;
    std::chrono::minutes customRoundDuration;
    bool isReplay;

public:
    CustomGame();

    [[nodiscard]] int getHighScore() const override;

    void updateHighScore() override;

    void markReplay();

    void setupRound() override;

    void displayMode(std::ostream& os) const override;

    [[nodiscard]] std::shared_ptr<Game> clone() const override;
};


#endif //CUSTOMGAME_H
