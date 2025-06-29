#ifndef PLAYER_H
#define PLAYER_H

#include <map>
#include <memory>
#include <string>
#include <vector>

class HighScore;

class Player
{
    std::string nickname;
    int score;
    static std::map<std::string, std::vector<std::shared_ptr<HighScore>>> highScores;

public:
    explicit Player(const std::string& nickname);

    [[nodiscard]] const std::string& getNickname() const;

    void setNickname(const std::string& newNickname);

    [[nodiscard]] int getScore() const;

    void setScore(int playerScore);

    void addOrUpdateHighScore(const std::shared_ptr<HighScore>& hscore);

    [[nodiscard]] static const std::map<std::string, std::vector<std::shared_ptr<HighScore>>>& getHighScores();

    friend std::ostream& operator<<(std::ostream& os, const Player& player);
};




#endif //PLAYER_H
