#ifndef EASYGAME_H
#define EASYGAME_H


#include <chrono>

#include "Game.h"

class EasyGame : public Game
{
    static int easyHighScore;

public:
    EasyGame();

private:
    [[nodiscard]] int getHighScore() const override;

    void updateHighScore() override;

    [[nodiscard]] std::string getDifficulty() const override;

    void setupDifficultyRound() override;

    void displayDifficulty(std::ostream& os) const override;

    [[nodiscard]] std::shared_ptr<Game> clone() const override;
};




#endif //EASYGAME_H
