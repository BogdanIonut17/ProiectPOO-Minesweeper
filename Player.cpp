#include <iostream>
#include "Player.h"


Player::Player(const std::string& nickname): nickname(nickname), score(0)
{
}

const std::string& Player::getNickname() const
{
    return nickname;
}

void Player::setNickname(const std::string& newNickname)
{
    nickname = newNickname;
}

int Player::getScore() const
{
    return score;
}

void Player::setScore(const int playerScore)
{
    score = playerScore;
}

std::ostream& operator<<(std::ostream& os, const Player& player)
{
    os << "Player: " << player.nickname << std::endl;
    os << "Score: " << player.score << std::endl;
    return os;
}
