//
// Created by Windows on 12.04.2025.
//

#ifndef GAME_H
#define GAME_H
#include <atomic>
#include <chrono>
#include <functional>

#include "Minefield.h"
#include "Player.h"


class Game
{
private:
    Minefield minefield;
    Player player;
    bool gameOver;
    std::chrono::steady_clock::time_point startTime;
    std::chrono::milliseconds totalTime;
    const std::chrono::seconds roundDuration;
    std::atomic<bool> timeExpired;
    bool firstGame;

    void setGameOver();

    bool isGameOver();

    [[nodiscard]] static bool isValidConfiguration(int rows, int cols, int mineCount) ;

    static void setTimeout(const std::function<void()>& func, std::chrono::milliseconds delay);

    void displayRemainingTime(std::chrono::steady_clock::time_point roundStart) const;

public:
    Game(const Minefield& minefield, const Player& player, std::chrono::milliseconds time,
         std::chrono::minutes duration);

    friend std::ostream& operator<<(std::ostream& os, const Game& game);

    void play();
};




#endif //GAME_H
