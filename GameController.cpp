//
// Created by Windows on 05.05.2025.
//

#include "GameController.h"

#include <iostream>

#include "EasyGame.h"
#include "HardGame.h"
#include "MediumGame.h"

bool GameController::firstGame = true;

GameController::GameController() = default;

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

void GameController::showMenu()
{
    std::cout << "\nChoose an option:\n";
    std::cout << "1. Easy\n";
    std::cout << "2. Medium\n";
    std::cout << "3. Hard\n";
    std::cout << "4. Keep previous difficulty\n";
    std::cout << "0. Exit\n";
    std::cout << "Enter choice:\n ";
}

std::shared_ptr<Game> GameController::createGame(const char choice)
{
    switch (choice) {
    case '0':
        {
            std::cout << "\nGoodbye!\n";
            return nullptr;
        }
    case '1': return std::make_shared<EasyGame>();
    case '2': return std::make_shared<MediumGame>();
    case '3': return std::make_shared<HardGame>();
    case '4':
        {
            if (gameMode && !firstGame)
            {
                setGameMode(gameMode->clone());
                return gameMode;
            }
            else
            {
                std::cout << "\nThis is the first game. Choose a difficulty (1, 2, 3) or type 0 to exit:\n";
                showMenu();
                char newChoice;
                std::cin >> newChoice;
                auto game = createGame(newChoice);
                return game;
            }
        }
    default: return std::make_shared<EasyGame>();
    }
}

std::shared_ptr<Game> GameController::getGameMode() const
{
    return gameMode;
}

void GameController::setGameMode(const std::shared_ptr<Game>& game)
{
    gameMode = game;
}

void GameController::run()
{
    static std::atomic globalTimeExpired = false;
    // static bool firstGame = true;
    static auto totalTime = std::chrono::minutes(10);

    if (firstGame)
    {

        showMenu();
        char choice;
        std::cin >> choice;

        // if (choice == '4')
        // {
        //     std::cout << "\nThis is the first game. Choose a difficulty (1, 2, 3) or type 0 to exit:\n";
        //     showMenu();
        //     char newChoice;
        //     std::cin >> newChoice;
        //     choice = newChoice;
        //     firstGame = false;
        // }
        auto game = createGame(choice);
        firstGame = false;

        if (game)
        {
            setGameMode(game);
        }
        else return;

        Game::setTimeout([this]
        {
            if (!globalTimeExpired)
            {
                std::cout << "\nTime's up! Game over!\n";
                globalTimeExpired = true;
                gameMode->setGameOver();
                std::exit(0);
            }
        }, totalTime);
    }

    while (!globalTimeExpired)
    {
        gameMode->play();

        if (globalTimeExpired)
            return;

        showMenu();
        char choice;
        std::cin >> choice;

        // if (choice == '4')
        // {
        //     setGameMode(gameMode->clone());
        // }
        // else
        auto newGame = createGame(choice);
        if (newGame)
        {
            setGameMode(newGame);
        }
        else return;
        }
}

void swap(GameController& lhs, GameController& rhs) noexcept
{
    using std::swap;
    swap(lhs.gameMode, rhs.gameMode);
}
