//
// Created by Windows on 04.05.2025.
//

#ifndef HARDGAME_H
#define HARDGAME_H
#include "Game.h"


class HardGame : public Game{
public:
    HardGame();

    HardGame(const HardGame& other);

    HardGame& operator=(const HardGame& other);

    void setupRound() override;

    void play() override;

    [[nodiscard]] std::unique_ptr<Game> clone() const override;
};




#endif //HARDGAME_H
