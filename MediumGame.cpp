//
// Created by Windows on 28.04.2025.
//

#include "MediumGame.h"

#include <iostream>
#include <thread>

MediumGame::MediumGame() : Game(Minefield(16, 16, 40), Player("Player1"), std::chrono::minutes(5))
{
}

MediumGame::MediumGame(const MediumGame& other): Game(other)
{
}

MediumGame& MediumGame::operator=(const MediumGame& other)
{
    if (this == &other)
        return *this;
    Game::operator =(other);
    return *this;
}

void MediumGame::setupRound()
{
    minefield.setFieldSize(16, 16, 40);
    std::cout << "Medium mode: 16x16 board with 40 mines." << std::endl;
}

void MediumGame::displayMode(std::ostream& os) const
{
    os << "Medium" << std::endl;
}


std::shared_ptr<Game> MediumGame::clone() const
{
    return std::make_shared<MediumGame>(*this);
}
