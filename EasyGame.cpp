//
// Created by Windows on 28.04.2025.
//

#include "EasyGame.h"
#include <iostream>
#include <thread>

EasyGame::EasyGame() : Game(Minefield(9, 9, 10), Player("Player1"), std::chrono::minutes(10), std::chrono::minutes(3)) {}

EasyGame::EasyGame(const EasyGame& other): Game(other)
{
}

EasyGame& EasyGame::operator=(const EasyGame& other)
{
    if (this == &other)
        return *this;
    Game::operator =(other);
    return *this;
}

void EasyGame::setupRound()
{
    gameOver = false;
    gameWon = false;
    player.setScore(0);
    minefield.setFieldSize(9, 9, 10);
    minefield.setFirstMove();

    std::cout << "Enter your nickname: " << std::endl;
    std::string newNickname;
    std::cin >> newNickname;
    player.setNickname(newNickname);

    std::cout << "Welcome to MineMaster, " << player.getNickname() << "!" << std::endl;
    std::cout << "Easy mode: 9x9 board with 10 mines." << std::endl;
}

void EasyGame::play() {
    while (!timeExpired)
    {
        setupRound();

        startTime = std::chrono::steady_clock::now();
        roundTimeLeftSeconds = static_cast<int>(roundDuration.count());
        std::atomic roundExpired = false;

        std::thread roundTimer([this, &roundExpired]
        {
            while (!gameOver && roundTimeLeftSeconds > 0)
            {
                std::this_thread::sleep_for(std::chrono::seconds(1));
                --roundTimeLeftSeconds;
            }

            if (!gameOver)
            {
                roundExpired = true;
                setGameOver();
            }
        });

        while (!gameOver && !timeExpired && !roundExpired)
        {
            displayRemainingTime();
            std::cout << "\n" << minefield << std::endl;

            minefield.processMove();

            if (isGameOver())
            {
                endGame();
                break;
            }
        }

        if (roundTimer.joinable())
            roundTimer.detach();

        if (roundExpired)
        {
            std::cout << "\nThis round has expired!" << std::endl;
        }

        displayRemainingTime();
        std::cout << "\n" << *this << std::endl;

        if (timeExpired)
            return;

        char choice;
        std::cout << "Play again? (y/n): " << std::endl;
        std::cin >> choice;
        if (std::toupper(choice) != 'Y')
            break;
    }
}

// std::unique_ptr<Game> EasyGame::clone() const {
//     return std::make_unique<EasyGame>(*this);
// }