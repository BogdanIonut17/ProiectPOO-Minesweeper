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
    std::chrono::seconds roundDuration;
    std::atomic<int> roundTimeLeftSeconds;

    [[nodiscard]] virtual int getHighScore() const = 0;

    virtual void updateHighScore() = 0;

    virtual void setupDifficultyRound() = 0;

    void displayRemainingTime() const;

    virtual void displayMode(std::ostream&) const = 0;

protected:
    Game(const Game& other);

    Game& operator=(const Game& other);

    [[nodiscard]] Minefield& getMinefield();

    [[nodiscard]] int getPlayerScore() const;

    [[nodiscard]] static bool isValidConfiguration(int rows, int cols, int mineCount);

    void setRoundDuration(std::chrono::minutes customDuration);

public:
    Game(const Minefield& minefield, const Player& player,
         std::chrono::minutes duration);

    virtual ~Game() = default;

    friend std::ostream& operator<<(std::ostream& os, const Game& game);

    bool isGameOver();

    void resetGameOver();

    static void setTimeout(const std::function<void()>& func, std::chrono::milliseconds delay);

    void setupRound();

    void play();

    void endGame();

    [[nodiscard]] virtual std::shared_ptr<Game> clone() const = 0;
};




#endif //GAME_H
