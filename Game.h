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
    Minefield minefield;
    Player player;
    bool gameOver;
    bool gameWon;
    std::chrono::steady_clock::time_point startTime;
    const std::chrono::seconds roundDuration;
    std::atomic<int> roundTimeLeftSeconds;

    void endGame();

    // [[nodiscard]] static bool isValidConfiguration(int rows, int cols, int mineCount);
protected:
    Game(const Game& other);

    Game& operator=(const Game& other);

    [[nodiscard]] int getScore() const;

    // void setHighScore(int score);

    [[nodiscard]] virtual int getHighScore() const = 0;

    virtual void updateHighScore()
    {
    }

    virtual void setupRound();

    void displayRemainingTime() const;

    virtual void displayMode(std::ostream&) const = 0;

public:
    Game(const Minefield& minefield, const Player& player,
         std::chrono::minutes duration);

    virtual ~Game() = default;

    friend std::ostream& operator<<(std::ostream& os, const Game& game);

    [[nodiscard]] Minefield& getMinefield();

    bool isGameOver();

    void resetGameOver();

    static void setTimeout(const std::function<void()>& func, std::chrono::milliseconds delay);

    void play();

    [[nodiscard]] virtual std::shared_ptr<Game> clone() const = 0;
};




#endif //GAME_H
