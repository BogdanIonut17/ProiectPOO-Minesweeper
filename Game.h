#ifndef GAME_H
#define GAME_H

#include <atomic>
#include <chrono>
#include <functional>
#include <memory>
#include <set>

#include "Minefield.h"
#include "Player.h"


class Game
{
    Minefield minefield;
    std::shared_ptr<Player> player;
    bool gameOver;
    bool gameWon;
    std::chrono::steady_clock::time_point startTime;
    std::chrono::seconds roundDuration;
    std::atomic<int> roundTimeLeftSeconds;
    // static std::vector<std::shared_ptr<Player>> allPlayers;

    [[nodiscard]] virtual int getHighScore() const = 0;

    virtual void updateHighScore() = 0;

    virtual void setupDifficultyRound() = 0;

    void displayRemainingTime() const;

    virtual void displayDifficulty(std::ostream&) const = 0;

protected:
    Game(const Game& other);

    Game& operator=(const Game& other);

    [[nodiscard]] Minefield& getMinefield();

    [[nodiscard]] const std::shared_ptr<Player>& getPlayer();

    [[nodiscard]] int getPlayerScore() const;

    [[nodiscard]] static bool isValidConfiguration(int rows, int cols, int mineCount);

    void setRoundDuration(std::chrono::minutes customDuration);

public:
    Game(const Minefield& minefield, const std::shared_ptr<Player>& player, std::chrono::minutes duration);

    virtual ~Game() = default;

    friend std::ostream& operator<<(std::ostream& os, const Game& game);

    bool isGameOver();

    void resetGameOver();

    // static const std::vector<std::shared_ptr<Player>>& getAllPlayers();

    static void setTimeout(const std::function<void()>& func, std::chrono::milliseconds delay);

    void setupRound();

    void play();

    void endGame();

    [[nodiscard]] virtual std::shared_ptr<Game> clone() const = 0;
};




#endif //GAME_H
