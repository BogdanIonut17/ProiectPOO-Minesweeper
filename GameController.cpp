#include "GameController.h"

#include <iostream>

#include "CustomGame.h"
#include "EasyGame.h"
#include "Exceptions.h"
#include "HardGame.h"
#include "MediumGame.h"


bool GameController::firstRound = true;

GameController::GameController() = default;

GameController::GameController(std::shared_ptr<Game> mode) : gameMode(std::move(mode))
{
}

GameController::GameController(const GameController& other): gameMode(other.gameMode)
{
    firstRound = true;
}

GameController& GameController::operator=(GameController other)
{
    swap(*this, other);
    return *this;
}

void GameController::showMenu()
{
    std::cout << "\nSelect an option: " << std::endl;
    std::cout << "1. Easy" << std::endl;
    std::cout << "2. Medium" << std::endl;
    std::cout << "3. Hard" << std::endl;
    std::cout << "4. Custom" << std::endl;
    std::cout << "5. Keep previous difficulty" << std::endl;
    std::cout << "6. Replay last round" << std::endl;
    std::cout << "0. Exit" << std::endl;
    std::cout << "Enter choice: " << std::endl;
}

std::shared_ptr<Game> GameController::createGame(const int choice)
{
    switch (choice)
    {
    case 0:
        {
            std::cout << "\nGoodbye!" << std::endl;
            return nullptr;
        }
    case 1: return std::make_shared<EasyGame>();
    case 2: return std::make_shared<MediumGame>();
    case 3: return std::make_shared<HardGame>();
    case 4: return std::make_shared<CustomGame>();
    case 5:
        {
            if (gameMode && !firstRound)
            {
                setGameMode(gameMode->clone());
                gameMode->resetGameOver();
                return gameMode;
            }
            std::cout << "\nThis is your first round. Choose a difficulty (1, 2, 3) or type 0 to exit:" << std::endl;
            showMenu();
            int newChoice;
            // std::cin >> newChoice;
            if (!(std::cin >> newChoice))
                throw InputReadError();
            auto game = createGame(newChoice);
            return game;
        }
    case 6:
        {
            if (gameMode && !firstRound)
            {
                auto customGame = std::dynamic_pointer_cast<CustomGame>(gameMode);
                if (customGame)
                {
                    customGame->markReplay();
                    customGame->resetGameOver();
                }
                return gameMode;
            }

            std::cout << "\nThis is your first round. Choose a difficulty (1, 2, 3) or type 0 to exit:" << std::endl;
            showMenu();
            int newChoice;
            // std::cin >> newChoice;
            if (!(std::cin >> newChoice))
                throw InputReadError();
            auto game = createGame(newChoice);
            return game;
        }
    default: return std::make_shared<EasyGame>();
    }
}

void GameController::setGameMode(const std::shared_ptr<Game>& game)
{
    gameMode = game;
}

void GameController::run()
{
    static std::atomic timeExpired = false;
    static auto totalTime = std::chrono::minutes(8);

    if (firstRound)
    {
        showMenu();
        int choice;
        // std::cin >> choice;
        if (!(std::cin >> choice))
            throw InputReadError();

        const auto game = createGame(choice);
        firstRound = false;

        if (game)
        {
            setGameMode(game);
        }
        else return;

        Game::setTimeout([this]
        {
            std::cout << "\nTime's up! Goodbye!" << std::endl;
            timeExpired = true;
            if (!gameMode->isGameOver())
            {
                std::cout << "Game over!" << std::endl;
                std::cout << *gameMode << std::endl;
            }
            std::exit(0);
        }, totalTime);
    }
    while (!timeExpired)
    {
        gameMode->play();

        showMenu();
        int choice;
        // std::cin >> choice;
        if (!(std::cin >> choice))
            throw InputReadError();

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
    GameController::firstRound = true;
}

