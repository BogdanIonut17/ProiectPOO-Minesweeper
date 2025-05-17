#ifndef EASYGAME_H
#define EASYGAME_H


#include <chrono>

#include "Game.h"

class EasyGame : public Game{
    static int easyHighScore;

public:
    EasyGame();

    [[nodiscard]] int getHighScore() const override;

    void updateHighScore() override;

    void setupRound() override;

    void displayMode(std::ostream& os) const override;

    [[nodiscard]] std::shared_ptr<Game> clone() const override;
};




#endif //EASYGAME_H
