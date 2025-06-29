#ifndef HIGHSCORE_H
#define HIGHSCORE_H

#include <memory>
#include <string>
#include <iostream>


class HighScore
{
    int score;

protected:
    std::string difficulty;

public:
    explicit HighScore(int score);

    virtual ~HighScore() = default;

    [[nodiscard]] const std::string& getDifficulty() const;

    [[nodiscard]] int getScore() const;

    [[nodiscard]] bool isBetterThan(const HighScore& other) const;

    virtual void display(std::ostream& os) const = 0;

    [[nodiscard]] virtual std::shared_ptr<HighScore> clone() const = 0;

    friend std::ostream& operator<<(std::ostream& os, const HighScore& hs);
};

class EasyHighScore : public HighScore
{
public:
    explicit EasyHighScore(int score);

    void display(std::ostream& os) const override;

    [[nodiscard]] std::shared_ptr<HighScore> clone() const override;
};

class MediumHighScore : public HighScore
{
public:
    explicit MediumHighScore(int score);

    void display(std::ostream& os) const override;

    [[nodiscard]] std::shared_ptr<HighScore> clone() const override;
};

class HardHighScore : public HighScore
{
public:
    explicit HardHighScore(int score);

    void display(std::ostream& os) const override;

    [[nodiscard]] std::shared_ptr<HighScore> clone() const override;
};

class CustomHighScore : public HighScore
{
public:
    explicit CustomHighScore(int score);

    void display(std::ostream& os) const override;

    [[nodiscard]] std::shared_ptr<HighScore> clone() const override;
};

class ClassicHighScore : public HighScore
{
public:
    explicit ClassicHighScore(int score);

    void display(std::ostream& os) const override;

    [[nodiscard]] std::shared_ptr<HighScore> clone() const override;
};




#endif // HIGHSCORE_H
