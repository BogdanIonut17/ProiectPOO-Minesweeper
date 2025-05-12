//
// Created by Windows on 05.05.2025.
//

#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <memory>

#include "Game.h"

class GameController
{
    std::shared_ptr<Game> gameMode;
    static bool firstGame;

public:
    GameController();

    explicit GameController(std::shared_ptr<Game> mode);

    GameController(const GameController& other);

    GameController& operator=(GameController other);

    friend void swap(GameController& lhs, GameController& rhs) noexcept;

    static void showMenu();

    std::shared_ptr<Game> createGame(char choice);

    void setGameMode(const std::shared_ptr<Game>& game);

    void run();
};


#endif //GAMECONTROLLER_H
