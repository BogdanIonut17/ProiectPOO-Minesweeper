#include "GameController.h"

#include <iostream>

#include "ClassicGame.h"
#include "CustomGame.h"
#include "EasyGame.h"
#include "Exceptions.h"
#include "HardGame.h"
#include "MediumGame.h"
#include "HighScores.h"


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
    std::cout << "1. Classic" << std::endl;
    std::cout << "2. Easy" << std::endl;
    std::cout << "3. Medium" << std::endl;
    std::cout << "4. Hard" << std::endl;
    std::cout << "5. Custom" << std::endl;
    std::cout << "6. Keep previous difficulty" << std::endl;
    std::cout << "7. Replay last round" << std::endl;
    std::cout << "8. Display highscores" << std::endl;
    std::cout << "0. Exit" << std::endl;
    std::cout << "Enter choice: " << std::endl;
}

void GameController::showHighScores()
{
    std::cout << "\n=== Highscores ===" << std::endl;
    for (const auto& [name, hscores] : Player::getHighScores())
    {
        std::cout << "Player: " << name << std::endl;
        for (const auto& hs : hscores)
        {
            std::cout << "  - " << *hs << std::endl;
        }
    }

    // const auto& players = Game::getAllPlayers();
    // if (players.empty()) {
    //     std::cout << "\nNo players registered yet.\n";
    //     return;
    // }
    //
    // std::cout << "\n=== HIGH SCORES ===\n";
    //
    // for (const auto& player : players) {
    //     std::cout << "\nPlayer: " << player->getNickname() << "\n";
    //     const auto& scores = player->getHighScores();
    //     if (scores.empty()) {
    //         std::cout << "  No scores yet.\n";
    //     } else {
    //         for (const auto& hs : scores) {
    //             std::cout << "  - " << *hs << "\n";
    //         }
    //     }
    // }
    //
    // std::cout << "=====================\n";
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
    case 1: return std::make_shared<ClassicGame>();
    case 2: return std::make_shared<EasyGame>();
    case 3: return std::make_shared<MediumGame>();
    case 4: return std::make_shared<HardGame>();
    case 5: return std::make_shared<CustomGame>();
    case 6:
        {
            if (gameMode && !firstRound)
            {
                // setGameMode(gameMode->clone());

                gameMode = gameMode->clone();
                gameMode->resetGameOver();
                return gameMode;
            }

            std::cout << "\nThis is your first round. Choose a difficulty (1, 2, 3, 4) or type 0 to exit:" << std::endl;
            showMenu();
            int newChoice;
            // std::cin >> newChoice;
            if (!(std::cin >> newChoice))
                throw InputReadError();
            auto game = createGame(newChoice);
            return game;
        }
    case 7:
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

            std::cout << "\nThis is your first round. Choose a difficulty (1, 2, 3, 4 and 5) or type 0 to exit:" << std::endl;
            showMenu();
            int newChoice;
            // std::cin >> newChoice;
            if (!(std::cin >> newChoice))
                throw InputReadError();
            auto game = createGame(newChoice);
            return game;
        }
    case 8:
        {
            if (gameMode && !firstRound)
            {
                showHighScores();
                showMenu();
                int newChoice;
                // std::cin >> newChoice;
                if (!(std::cin >> newChoice))
                    throw InputReadError();
                auto game = createGame(newChoice);
                return game;
            }

            std::cout << "\nThis is your first round. Choose a difficulty (1, 2, 3, 4 and 8) or type 0 to exit:" << std::endl;
            showMenu();
            int newChoice;
            // std::cin >> newChoice;
            if (!(std::cin >> newChoice))
                throw InputReadError();
            auto game = createGame(newChoice);
            return game;
        }
    default: return std::make_shared<ClassicGame>();
    }
}

// void GameController::setGameMode(const std::shared_ptr<Game>& game)
// {
//     gameMode = game;
// }

void GameController::run()
{
    static std::atomic timeExpired = false;
    static auto totalTime = std::chrono::minutes(10);

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
            // setGameMode(game);
            gameMode = game;
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
            // setGameMode(newGame);
            gameMode = newGame;
        }
        // if (choice == 7) {
        //     showHighScores();
        // }
        else return;
    }
}

void swap(GameController& lhs, GameController& rhs) noexcept
{
    using std::swap;
    swap(lhs.gameMode, rhs.gameMode);
    GameController::firstRound = true;
}
