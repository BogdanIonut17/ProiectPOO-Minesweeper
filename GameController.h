//
// Created by Windows on 05.05.2025.
//

#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <memory>
#include "Game.h"

class GameController
{
private:
    std::shared_ptr<Game> gameMode;

public:
    explicit GameController(std::shared_ptr<Game> mode);

    GameController(const GameController& other);

    GameController& operator=(GameController other);

    friend void swap(GameController& lhs, GameController& rhs) noexcept;

    void run() const;
};


#endif //GAMECONTROLLER_H
