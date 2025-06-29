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

void Player::addOrUpdateHighScore(const std::shared_ptr<HighScore>& hscore)
{
    auto& hs = highScores[nickname];
    if (hs.empty())
    {
        hs.push_back(hscore->clone());
        return;
    }
    // const auto& scoreRef = *score;  // evita evaluarea multiplă
    // for (auto& existing : hs)
    // {
    //     const auto& existingRef = *existing;
    //     if (typeid(existingRef) == typeid(scoreRef))
    //     {
    //         if (score->isBetterThan(existingRef))
    //         {
    //             existing = score;
    //         }
    //         return;
    //     }
    // }
    for (auto& existing : hs)
    {
        // if (typeid(*existing) == typeid(*hscore))
        if (existing->getDifficulty() == hscore->getDifficulty())
        {
            if (hscore->isBetterThan(*existing))
            {
                existing = hscore;
            }
            return;
        }
    }
    hs.push_back(hscore->clone());
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
