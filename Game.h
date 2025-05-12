//
// Created by Windows on 12.04.2025.
//

#ifndef GAME_H
#define GAME_H
#include <atomic>
#include <chrono>
#include <functional>
#include <memory>

#include "Minefield.h"
#include "Player.h"


class Game
{
protected:
    Minefield minefield;
    Player player;
    bool gameOver;
    bool gameWon;
    std::chrono::steady_clock::time_point startTime;
    const std::chrono::seconds roundDuration;
    std::atomic<int> roundTimeLeftSeconds;

    void endGame();

    bool isGameOver();

    // [[nodiscard]] static bool isValidConfiguration(int rows, int cols, int mineCount);

    virtual void setupRound();

    void displayRemainingTime() const;

    virtual void displayMode(std::ostream&) const;

public:
    Game(const Minefield& minefield, const Player& player,
         std::chrono::minutes duration);

    Game(const Game& other);

    Game& operator=(const Game& other);

    virtual ~Game() = default;

    friend std::ostream& operator<<(std::ostream& os, const Game& game);

    void setGameOver();

    static void setTimeout(const std::function<void()>& func, std::chrono::milliseconds delay);

    void play();

    [[nodiscard]] virtual std::shared_ptr<Game> clone() const = 0;

    // void displayStatus() const;
};




#endif //GAME_H
