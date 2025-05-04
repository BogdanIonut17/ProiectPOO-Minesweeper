//
// Created by Windows on 05.05.2025.
//

#include "GameController.h"

GameController::GameController(std::unique_ptr<Game> mode) : gameMode(std::move(mode))
{
}

void GameController::run() const
{
    gameMode->play();
}
