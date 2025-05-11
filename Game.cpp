//
// Created by Windows on 12.04.2025.
//

#include "Game.h"

#include <iostream>
#include <thread>

#include "GameController.h"

void Game::setGameOver()
{
    gameOver = true;
}

void Game::endGame()
{
    if (!gameWon)
    {
        std::cout << "\nYou revealed a mine!" << std::endl;
    }
    else
    {
        const auto endTime = std::chrono::steady_clock::now();
        const std::chrono::milliseconds elapsedTime =
            std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
        const std::chrono::milliseconds gameTimeLeft = roundDuration - elapsedTime;

        const int playerScore = std::max(0, static_cast<int>(gameTimeLeft.count()));
        player.setScore(playerScore);

        std::cout << "\nYou won!" << std::endl;
    }
    setGameOver();
}

bool Game::isGameOver()
{
    bool cellUnrevealed = false;
    for (int i = 0; i < minefield.getRows(); i++)
    {
        for (int j = 0; j < minefield.getCols(); j++)
        {
            if (minefield.getCell(i, j).isMine())
            {
                if (minefield.getCell(i, j).checkIfRevealed())
                {
                    return true;
                }
            }
            else if (!minefield.getCell(i, j).checkIfRevealed())
            {
                cellUnrevealed = true;
            }
        }
    }
    if (cellUnrevealed)
        return false;
    gameWon = true;
    return true;
}

bool Game::isValidConfiguration(const int rows, const int cols, const int mineCount)
{
    return rows > 0 && cols > 0 && mineCount > 0 && mineCount < rows * cols;
}

void Game::setTimeout(const std::function<void()>& func, std::chrono::milliseconds delay)
{
    std::thread([func, delay]
    {
        std::this_thread::sleep_for(delay);
        func();
    }).detach();
}

void Game::displayRemainingTime() const
{
    const int minutes = roundTimeLeftSeconds.load() / 60;
    const int seconds = roundTimeLeftSeconds.load() % 60;

    std::cout << "Time left: " << minutes << ":" << (seconds < 10 ? "0" : "") << seconds << "   "<< std::endl;
}

// Game::Game(const Minefield& minefield, const Player& player, const std::chrono::milliseconds time,
//            const std::chrono::minutes duration): minefield(minefield), player(player),
//                                                  gameOver(false), gameWon(false),
//                                                  startTime(std::chrono::steady_clock::now()),
//                                                  totalTime(time), roundDuration(duration),
//                                                  timeExpired(false), firstGame(true)
// {
// }

Game::Game(const Minefield& minefield, const Player& player,
           const std::chrono::minutes duration): minefield(minefield), player(player),
                                                 gameOver(false), gameWon(false),
                                                 startTime(std::chrono::steady_clock::now()),
                                                 roundDuration(duration),
                                                 timeExpired(false), firstGame(true)
{}

Game::Game(const Game& other): minefield(other.minefield),
                               player(other.player),
                               gameOver(other.gameOver),
                               gameWon(other.gameWon),
                               startTime(other.startTime),
                               totalTime(other.totalTime),
                               roundDuration(other.roundDuration),
                               firstGame(other.firstGame)
{
}

Game& Game::operator=(const Game& other)
{
    if (this == &other)
        return *this;
    minefield = other.minefield;
    player = other.player;
    gameOver = other.gameOver;
    gameWon = other.gameWon;
    startTime = other.startTime;
    totalTime = other.totalTime;
    firstGame = other.firstGame;
    return *this;
}

void Game::setupRound()
{
    gameOver = false;
    gameWon = false;
    player.setScore(0);
    minefield.setFirstMove();

    // int newRows = 0, newCols = 0, newMineCount = 0;
    // std::cout << "Enter board size (rows, cols) and number of mines: " << std::endl;
    // std::cin >> newRows >> newCols >> newMineCount;
    //
    // if (!isValidConfiguration(newRows, newCols, newMineCount))
    // {
    //     std::cout << "Invalid board configuration! Defaulting to 8x8 with 9 mines." << std::endl;
    //     newRows = 8;
    //     newCols = 8;
    //     newMineCount = 9;
    // }
    //
    // minefield.setFieldSize(newRows, newCols, newMineCount);

    std::cout << "Enter your nickname: " << std::endl;
    std::string newNickname;
    std::cin >> newNickname;
    player.setNickname(newNickname);

    std::cout << "Welcome to MineMaster, " << player.getNickname() << "!" << std::endl;

    // if (firstGame)
    // {
    //     setTimeout([this]
    //     {
    //         if (!gameOver)
    //         {
    //             std::cout << "\nTime's up! Game over!\n";
    //             timeExpired = true;
    //             setGameOver();
    //         }
    //         std::exit(0);
    //     }, totalTime);
    //     firstGame = false;
    // }
}

void Game::play()
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

    if (roundExpired && !gameOver)
    {
        std::cout << "\nThis round has expired!" << std::endl;
    }

    displayRemainingTime();
    std::cout << *this << std::endl;

    // while (!timeExpired)
    // {
    //     setupRound();
    //
    //     startTime = std::chrono::steady_clock::now();
    //     roundTimeLeftSeconds = static_cast<int>(roundDuration.count());
    //     std::atomic roundExpired = false;
    //
    //     std::thread roundTimer([this, &roundExpired]
    //     {
    //         while (!gameOver && roundTimeLeftSeconds > 0)
    //         {
    //             std::this_thread::sleep_for(std::chrono::seconds(1));
    //             --roundTimeLeftSeconds;
    //         }
    //
    //         if (!gameOver)
    //         {
    //             roundExpired = true;
    //             setGameOver();
    //         }
    //     });
    //
    //     while (!gameOver && !timeExpired && !roundExpired)
    //     {
    //         displayRemainingTime();
    //         std::cout << "\n" << minefield << std::endl;
    //
    //         minefield.processMove();
    //
    //         if (isGameOver())
    //         {
    //             endGame();
    //             break;
    //         }
    //     }
    //
    //     if (roundTimer.joinable())
    //         roundTimer.detach();
    //
    //     if (roundExpired)
    //     {
    //         std::cout << "\nThis round has expired!" << std::endl;
    //     }
    //
    //     // displayRemainingTime();
    //     std::cout << *this << std::endl;
    //
    //     if (timeExpired)
    //         return;

        // char choice;
        // std::cout << "Play again? (y/n): " << std::endl;
        // std::cin >> choice;
        // if (std::toupper(choice) != 'Y')
        //     break;
    // }


}


void Game::displayMode(std::ostream&) const
{
}

std::ostream& operator<<(std::ostream& os, const Game& game)
{
    if (game.gameOver)
    {
        os << "Game over!" << std::endl;
    }
    os << "\nDifficulty: ";
    game.displayMode(os);
    os << game.player << std::endl;
    os << game.minefield << std::endl;
    return os;
}
