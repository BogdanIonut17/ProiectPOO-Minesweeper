//
// Created by Windows on 12.04.2025.
//

#include <iostream>
#include "Player.h"

Player::Player(const std::string& nickname): nickname(nickname), score(0)
{
}

const std::string& Player::getNickname() const
{
    return nickname;
}

int Player::getScore() const
{
    return score;
}

void Player::setNickname(const std::string& newNickname)
{
    nickname = newNickname;
}

void Player::setScore(const int playerScore)
{
    score = playerScore;
}

std::ostream& operator<<(std::ostream& os, const Player& player)
{
    os << player.nickname << " " << player.score;
    return os;
}
