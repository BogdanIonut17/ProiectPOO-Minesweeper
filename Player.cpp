#include <iostream>
#include "Player.h"

#include "HighScores.h"

std::map<std::string, std::vector<std::shared_ptr<HighScore>>> Player::highScores;

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

void Player::addOrUpdateHighScore(const std::shared_ptr<HighScore>& score)
{
    auto& hs = highScores[nickname];
    if (hs.empty())
    {
        hs.push_back(score->clone());
        return;
    }
    for (auto& existing : hs) {
        if (typeid(*existing) == typeid(*score)) {
            if (score->isBetterThan(*existing)) {
                existing = score;
            }
            return;
        }
    }
    hs.push_back(score->clone());
}

const std::map<std::string, std::vector<std::shared_ptr<HighScore>>>& Player::getHighScores()
{
    return highScores;
}

std::ostream& operator<<(std::ostream& os, const Player& player)
{
    os << "Player: " << player.nickname << std::endl;
    os << "Score: " << player.score << std::endl;
    return os;
}
