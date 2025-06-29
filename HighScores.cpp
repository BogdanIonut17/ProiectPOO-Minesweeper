#include "HighScores.h"

#include <iomanip>


HighScore::HighScore(const int score) : score(score) {}

// std::string HighScore::getDifficulty() const
// {
//     return difficulty;
// }

int HighScore::getScore() const {
    return score;
}

bool HighScore::isBetterThan(const HighScore& other) const
{
    return this->score > other.score;
}

std::ostream& operator<<(std::ostream& os, const HighScore& hs) {
    os << "Difficulty: ";
    hs.display(os);
    return os;
}


EasyHighScore::EasyHighScore(const int score)
    : HighScore(score)
{
    difficulty = "Easy";
}

void EasyHighScore::display(std::ostream& os) const {
    os << std::left << std::setw(7) << "Easy" << "| Score: " << getScore();
}

std::shared_ptr<HighScore> EasyHighScore::clone() const
{
    return std::make_shared<EasyHighScore>(getScore());
}


MediumHighScore::MediumHighScore(const int score)
    : HighScore(score) {}

void MediumHighScore::display(std::ostream& os) const {
    os << std::left << std::setw(7) << "Medium" << "| Score: " << getScore();
}

std::shared_ptr<HighScore> MediumHighScore::clone() const
{
    return std::make_shared<MediumHighScore>(getScore());
}


HardHighScore::HardHighScore(const int score)
    : HighScore(score) {}

void HardHighScore::display(std::ostream& os) const {
    os << std::left << std::setw(7) << "Hard" << "| Score: " << getScore();
}

std::shared_ptr<HighScore> HardHighScore::clone() const
{
    return std::make_shared<HardHighScore>(getScore());
}


CustomHighScore::CustomHighScore(const int score)
    : HighScore(score)
{
    difficulty = "Custom";
}

void CustomHighScore::display(std::ostream& os) const {
    os << std::left << std::setw(7) << "Custom" << "| Score: " << getScore();
}

std::shared_ptr<HighScore> CustomHighScore::clone() const
{
    return std::make_shared<CustomHighScore>(getScore());
}


