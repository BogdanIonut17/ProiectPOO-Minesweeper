//
// Created by Windows on 28.04.2025.
//

#include "HardGame.h"

#include <iostream>
#include <thread>

// HardGame::HardGame() : Game(Minefield(16, 30, 99), Player("Player1"), std::chrono::minutes(20), std::chrono::minutes(10))
// {
// }

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
    // gameOver = false;
    // gameWon = false;
    // player.setScore(0);
    // minefield.setFirstMove();
    //
    // std::cout << "Enter your nickname: " << std::endl;
    // std::string newNickname;
    // std::cin >> newNickname;
    // player.setNickname(newNickname);
    //
    // std::cout << "Welcome to MineMaster, " << player.getNickname() << "!" << std::endl;

    Game::setupRound();

    std::cout << "Hard mode: 16x30 board with 99 mines." << std::endl;
    minefield.setFieldSize(16, 30, 99);
}

// void HardGame::play()
// {
//     while (!timeExpired)
//     {
//         setupRound();
//
//         startTime = std::chrono::steady_clock::now();
//         roundTimeLeftSeconds = static_cast<int>(roundDuration.count());
//         std::atomic roundExpired = false;
//
//         std::thread roundTimer([this, &roundExpired]
//         {
//             while (!gameOver && roundTimeLeftSeconds > 0)
//             {
//                 std::this_thread::sleep_for(std::chrono::seconds(1));
//                 --roundTimeLeftSeconds;
//             }
//
//             if (!gameOver)
//             {
//                 roundExpired = true;
//                 setGameOver();
//             }
//         });
//
//         while (!gameOver && !timeExpired && !roundExpired)
//         {
//             displayRemainingTime();
//             std::cout << "\n" << minefield << std::endl;
//
//             minefield.processMove();
//
//             if (isGameOver())
//             {
//                 endGame();
//                 break;
//             }
//         }
//
//         if (roundTimer.joinable())
//             roundTimer.detach();
//
//         if (roundExpired)
//         {
//             std::cout << "\nThis round has expired!" << std::endl;
//         }
//
//         // displayRemainingTime();
//         std::cout << *this << std::endl;
//
//         if (timeExpired)
//             return;
//
//         char choice;
//         std::cout << "Play again? (y/n): " << std::endl;
//         std::cin >> choice;
//         if (std::toupper(choice) != 'Y')
//             break;
//     }
// }

void HardGame::displayMode(std::ostream& os) const
{
    os << "Hard" << std::endl;
}

std::shared_ptr<Game> HardGame::clone() const
{
    return std::make_shared<HardGame>(*this);
}

