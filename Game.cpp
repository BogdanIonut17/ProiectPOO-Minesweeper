//
// Created by Windows on 12.04.2025.
//

#include "Game.h"

#include <iostream>
#include <thread>

void Game::setGameOver()
{
    gameOver = true;
}

bool Game::isGameOver()
{
    for (int i = 0; i < minefield.getRows(); i++)
    {
        for (int j = 0; j < minefield.getCols(); j++)
        {
            if (minefield.getCell(i, j).isMine() && minefield.getCell(i, j).checkIfRevealed())
            {
                std::cout << "\nYou revealed a mine!" << std::endl;
                setGameOver();
                return true;
            }
        }
    }
    for (int i = 0; i < minefield.getRows(); i++)
    {
        for (int j = 0; j < minefield.getCols(); j++)
        {
            if (!minefield.getCell(i, j).isMine() && !minefield.getCell(i, j).checkIfRevealed())
                return false;
        }
    }
    const auto endTime = std::chrono::steady_clock::now();
    const std::chrono::milliseconds elapsedTime =
        std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    const std::chrono::milliseconds gameTimeLeft = roundDuration - elapsedTime;

    const int playerScore = std::max(0, static_cast<int>(gameTimeLeft.count()));
    player.setScore(playerScore);
    Player::setHighScore(player);

    std::cout << "\nYou won!" << std::endl;
    setGameOver();
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

void Game::displayRemainingTime(const std::chrono::steady_clock::time_point roundStart) const
{
    const auto now = std::chrono::steady_clock::now();
    const auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - roundStart);
    auto remaining = std::chrono::duration_cast<std::chrono::seconds>(roundDuration - elapsed);

    if (remaining.count() < 0)
        remaining = std::chrono::seconds(0);

    const int minutes = static_cast<int>(remaining.count() / 60);
    const int seconds = static_cast<int>(remaining.count() % 60);

    std::cout << "Time left: " << minutes << ":" << (seconds < 10 ? "0" : "") << seconds << std::endl;
}


Game::Game(const Minefield& minefield, const Player& player, const std::chrono::milliseconds time,
    const std::chrono::minutes duration): minefield(minefield), player(player),
                                          gameOver(false),
                                          startTime(std::chrono::steady_clock::now()),
                                          totalTime(time), roundDuration(duration),
                                          timeExpired(false), firstGame(true)
{
}

void Game::play()
{
    while (!timeExpired)
    {
        gameOver = false;
        player.setScore(0);
        minefield.setFirstMove();

        int newRows = 0, newCols = 0, newMineCount = 0;
        std::cout << "Enter board size (rows, cols) and number of mines: " << std::endl;
        std::cin >> newRows >> newCols >> newMineCount;

        if (!isValidConfiguration(newRows, newCols, newMineCount))
        {
            std::cout << "Invalid board configuration! Defaulting to 8x8 with 9 mines." << std::endl;
            newRows = 8;
            newCols = 8;
            newMineCount = 9;
        }

        minefield.setFieldSize(newRows, newCols, newMineCount);

        std::cout << "Enter your nickname: " << std::endl;
        std::string newNickname;
        std::cin >> newNickname;
        player.setNickname(newNickname);

        std::cout << "Welcome to MineMaster, " << player.getNickname() << "!" << std::endl;

        if (firstGame)
        {
            setTimeout([this]
            {
                if (!gameOver)
                {
                    std::cout << "\nTime's up! Game over!\n";
                    timeExpired = true;
                    setGameOver();
                }
                std::exit(0);
            }, totalTime);
            firstGame = false;
        }

        startTime = std::chrono::steady_clock::now();
        std::atomic roundExpired = false;

        std::thread roundTimer([&]
        {
            const auto roundStart = startTime;
            while (!gameOver && !roundExpired)
            {
                auto now = std::chrono::steady_clock::now();
                if (now - roundStart >= roundDuration)
                {
                    roundExpired = true;
                    gameOver = true;
                    break;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
            }
        });

        while (!gameOver && !timeExpired && !roundExpired)
        {
            displayRemainingTime(startTime);
            std::cout << "\n" << minefield << std::endl;

            minefield.processMove();

            if (isGameOver())
            {
                gameOver = true;
                break;
            }
        }

        if (roundTimer.joinable())
            roundTimer.detach();

        if (roundExpired)
        {
            std::cout << "\nThis round has expired!" << std::endl;
        }

        displayRemainingTime(startTime);
        std::cout << "\n" << *this << std::endl;

        if (timeExpired)
            return;

        char choice;
        std::cout << "Play again? Type y: " << std::endl;
        std::cin >> choice;
        if (std::toupper(choice) != 'Y')
            break;
    }
}

std::ostream& operator<<(std::ostream& os, const Game& game)
{
    if (game.gameOver)
    {
        os << "Game over!" << std::endl;
    }
    os << "Player: " << game.player.getNickname() << std::endl;
    os << "Score: " << game.player.getScore() << std::endl;
    os << "High score: " << Player::getHighScore() << std::endl;
    os << "Minefield: " << std::endl;
    os << game.minefield << std::endl;
    return os;
}
