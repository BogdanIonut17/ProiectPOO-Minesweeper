#ifndef PLAYER_H
#define PLAYER_H
#include <string>


class Player
{
    std::string nickname;
    int score;

public:
    explicit Player(const std::string& nickname);

    [[nodiscard]] const std::string& getNickname() const;

    void setNickname(const std::string& newNickname);

    [[nodiscard]] int getScore() const;

    void setScore(int playerScore);

    friend std::ostream& operator<<(std::ostream& os, const Player& player);
};




#endif //PLAYER_H
