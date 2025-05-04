//
// Created by Windows on 28.04.2025.
//

#ifndef EASYGAME_H
#define EASYGAME_H


#include <chrono>

#include "Game.h"

class EasyGame : public Game{
public:
    EasyGame();

    EasyGame(const EasyGame& other);

    EasyGame& operator=(const EasyGame& other);

    void setupRound() override;

    void play() override;

    // [[nodiscard]] std::unique_ptr<Game> clone() const override;
};




#endif //EASYGAME_H
