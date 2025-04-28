//
// Created by Windows on 12.04.2025.
//

#ifndef PLAYER_H
#define PLAYER_H
#include <string>


class Player
{
private:
    std::string nickname;
    int score;
    static int highScore;

public:
    explicit Player(const std::string& nickname);

    [[nodiscard]] const std::string& getNickname() const;

    [[nodiscard]] int getScore() const;

    static int getHighScore();

    void setNickname(const std::string& newNickname);

    void setScore(int playerScore);

    friend std::ostream& operator<<(std::ostream& os, const Player& player);
};




#endif //PLAYER_H
