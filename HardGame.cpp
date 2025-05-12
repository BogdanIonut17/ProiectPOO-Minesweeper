//
// Created by Windows on 28.04.2025.
//

#include "HardGame.h"

#include <iostream>
#include <thread>

HardGame::HardGame() : Game(Minefield(16, 30, 99), Player("Player1"), std::chrono::minutes(10))
{
}

HardGame::HardGame(const HardGame& other): Game(other)
{
}

HardGame& HardGame::operator=(const HardGame& other)
{
    if (this == &other)
        return *this;
    Game::operator =(other);
    return *this;
}

void HardGame::setupRound()
{
    std::cout << "Hard mode: 16x30 board with 99 mines." << std::endl;
    minefield.setFieldSize(16, 30, 99);
}

void HardGame::displayMode(std::ostream& os) const
{
    os << "Hard" << std::endl;
}

std::shared_ptr<Game> HardGame::clone() const
{
    return std::make_shared<HardGame>(*this);
}

