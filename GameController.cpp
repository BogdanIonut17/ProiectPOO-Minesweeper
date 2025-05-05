//
// Created by Windows on 05.05.2025.
//

#include "GameController.h"

GameController::GameController(std::shared_ptr<Game> mode) : gameMode(std::move(mode))
{
}

GameController::GameController(const GameController& other): gameMode(other.gameMode)
{
}

GameController& GameController::operator=(GameController other)
{
    swap(*this, other);
    return *this;
}

void GameController::run() const
{
    gameMode->play();
}

void swap(GameController& lhs, GameController& rhs) noexcept
{
    using std::swap;
    swap(lhs.gameMode, rhs.gameMode);
}
