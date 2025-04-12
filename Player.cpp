//
// Created by Windows on 12.04.2025.
//

#include <iostream>
#include "Player.h"
int Player::highScore = 0;

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

int Player::getHighScore()
{
    return highScore;
}

void Player::setNickname(const std::string& newNickname)
{
    nickname = newNickname;
}

void Player::setScore(const int playerScore)
{
    score = playerScore;
}

void Player::setHighScore(const Player& player)
{
    if (player.score > highScore)
        highScore = player.score;
}

std::ostream& operator<<(std::ostream& os, const Player& player)
{
    os << player.nickname << " " << player.score;
    return os;
}
