#include "Game.h"

#include <iostream>
#include <thread>

#include "Exceptions.h"
#include "GameController.h"

// std::vector<std::shared_ptr<Player>> Game::allPlayers;
//
// const std::vector<std::shared_ptr<Player>>& Game::getAllPlayers() {
//     return allPlayers;
// }

void Game::resetGameOver()
{
    gameOver = false;
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
        player->setScore(playerScore);
        updateHighScore();

        std::cout << "\nYou won!" << std::endl;
    }
    gameOver = true;
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

std::shared_ptr<HighScore> Game::getPlayerHighScore() const
{
    const auto& allScores = Player::getHighScores();
    const auto it = allScores.find(player->getNickname());
    if (it == allScores.end())
    {
        return nullptr;
    }

    const auto& scores = it->second;

    for (const auto& hs : scores) {
        if (hs && hs->getDifficulty() == this->getDifficulty()) {
            return hs;
        }
    }

    return nullptr;
}

void Game::displayRemainingTime() const
{
    const int minutes = roundTimeLeftSeconds.load() / 60;
    const int seconds = roundTimeLeftSeconds.load() % 60;

    std::cout << "Time left: " << minutes << ":" << (seconds < 10 ? "0" : "") << seconds << "   " << std::endl;
}

Game::Game(const Minefield& minefield, const std::shared_ptr<Player>& player, const std::chrono::minutes duration)
    : minefield(minefield), player(player), gameOver(false), gameWon(false),
      startTime(std::chrono::steady_clock::now()), roundDuration(duration)
{
    // Game::allPlayers.push_back(player);
}


Game::Game(const Game& other): minefield(other.minefield),
                               player(other.player),
                               gameOver(other.gameOver),
                               gameWon(other.gameWon),
                               startTime(other.startTime),
                               roundDuration(other.roundDuration)
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
    return *this;
}

Minefield& Game::getMinefield()
{
    return minefield;
}

const std::shared_ptr<Player>& Game::getPlayer()
{
    return player;
}


[[nodiscard]] int Game::getPlayerScore() const
{
    return player->getScore();
}

void Game::setRoundDuration(const std::chrono::minutes customDuration)
{
    roundDuration = customDuration;
}

void Game::setupRound()
{
    gameOver = false;
    gameWon = false;
    player->setScore(0);
    minefield.setFirstMove();

    std::cout << "Enter your nickname: " << std::endl;
    std::string newNickname;
    std::cin >> newNickname;
    if (!newNickname.empty())
        player->setNickname(newNickname);

    std::cout << "Welcome to MineMaster, " << player->getNickname() << "!" << std::endl;

    setupDifficultyRound();
}

void Game::play()
{
    if (gameOver)
    {
        minefield.resetField();
        gameOver = false;
        gameWon = false;
        player->setScore(0);
    }
    else
    {
        setupRound();
    }

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
            gameOver = true;
        }
    });

    while (!gameOver && !roundExpired)
    {
        displayRemainingTime();
        std::cout << "\n" << minefield << std::endl;

        if (roundTimer.joinable())
            roundTimer.detach();

        minefield.processMove();

        if (isGameOver())
        {
            endGame();
            break;
        }
    }

    // if (roundTimer.joinable())
    //     roundTimer.detach();

    if (roundExpired)
    {
        std::cout << "\nThis round has expired!" << std::endl;
    }

    displayRemainingTime();
    std::cout << *this << std::endl;
}

std::ostream& operator<<(std::ostream& os, const Game& game)
{
    if (game.gameOver)
    {
        os << "Game over!" << std::endl;
    }

    os << "\nDifficulty: ";
    game.displayDifficulty(os);
    os << *game.player;
    os << "Highscore: " << game.getHighScore() << std::endl;
    os << "Your Highscore: ";

    if (game.getPlayerHighScore())
    {
       os << game.getPlayerHighScore()->getScore() << std::endl;
    }
    else
    {
        os << "0" << std::endl;
    }

    os << game.minefield << std::endl;
    return os;
}
