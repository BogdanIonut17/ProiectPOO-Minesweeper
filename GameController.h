#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <memory>

#include "Game.h"

class GameController
{
    std::shared_ptr<Game> gameMode;
    static bool firstRound;

    static void showMenu();

    static void showHighScores();

    std::shared_ptr<Game> createGame(int choice);

    // void setGameMode(const std::shared_ptr<Game>& game);

public:
    GameController();

    explicit GameController(std::shared_ptr<Game> mode);

    GameController(const GameController& other);

    GameController& operator=(GameController other);

    friend void swap(GameController& lhs, GameController& rhs) noexcept;

    void run();

};


#endif //GAMECONTROLLER_H
