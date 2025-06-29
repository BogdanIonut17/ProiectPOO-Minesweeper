//
// Created by Windows on 29.06.2025.
//

#ifndef GAMEFACTORY_H
#define GAMEFACTORY_H


#include <memory>

#include "Game.h"
#include "ClassicGame.h"
#include "EasyGame.h"
#include "MediumGame.h"
#include "HardGame.h"
#include "CustomGame.h"
#include "HighScores.h"


template <class T>
class GameFactory
{
public:
    virtual ~GameFactory() = default;

    [[nodiscard]] virtual std::shared_ptr<T> createGame() const = 0;

    [[nodiscard]] virtual std::shared_ptr<T> createHighScore(int score) const = 0;
};


class ClassicGameFactory : public GameFactory<Game>
{
public:
    [[nodiscard]] std::shared_ptr<Game> createGame() const override;

    [[nodiscard]] std::shared_ptr<Game> createHighScore(int score) const override;
};

inline std::shared_ptr<Game> ClassicGameFactory::createGame() const
{
    return std::make_shared<ClassicGame>();
}

inline std::shared_ptr<Game> ClassicGameFactory::createHighScore(int) const
{
    return nullptr;
}


class EasyGameFactory : public GameFactory<Game>
{
public:
    [[nodiscard]] std::shared_ptr<Game> createGame() const override;

    [[nodiscard]] std::shared_ptr<Game> createHighScore(int score) const override;
};

inline std::shared_ptr<Game> EasyGameFactory::createGame() const
{
    return std::make_shared<EasyGame>();
}

inline std::shared_ptr<Game> EasyGameFactory::createHighScore(int) const
{
    return nullptr;
}


class MediumGameFactory : public GameFactory<Game>
{
public:
    [[nodiscard]] std::shared_ptr<Game> createGame() const override;

    [[nodiscard]] std::shared_ptr<Game> createHighScore(int score) const override;
};

inline std::shared_ptr<Game> MediumGameFactory::createGame() const
{
    return std::make_shared<MediumGame>();
}

inline std::shared_ptr<Game> MediumGameFactory::createHighScore(int) const
{
    return nullptr;
}


class HardGameFactory : public GameFactory<Game>
{
public:
    [[nodiscard]] std::shared_ptr<Game> createGame() const override;

    [[nodiscard]] std::shared_ptr<Game> createHighScore(int score) const override;
};

inline std::shared_ptr<Game> HardGameFactory::createGame() const
{
    return std::make_shared<HardGame>();
}

inline std::shared_ptr<Game> HardGameFactory::createHighScore(int) const
{
    return nullptr;
}


class CustomGameFactory : public GameFactory<Game>
{
public:
    [[nodiscard]] std::shared_ptr<Game> createGame() const override;

    [[nodiscard]] std::shared_ptr<Game> createHighScore(int score) const override;
};

inline std::shared_ptr<Game> CustomGameFactory::createGame() const
{
    return std::make_shared<CustomGame>();
}

inline std::shared_ptr<Game> CustomGameFactory::createHighScore(int) const
{
    return nullptr;
}


class ClassicHighscoreFactory : public GameFactory<HighScore>
{
public:
    [[nodiscard]] std::shared_ptr<HighScore> createHighScore(int score) const override;

    [[nodiscard]] std::shared_ptr<HighScore> createGame() const override;
};

inline std::shared_ptr<HighScore> ClassicHighscoreFactory::createHighScore(const int score) const
{
    return std::make_shared<ClassicHighScore>(score);
}

inline std::shared_ptr<HighScore> ClassicHighscoreFactory::createGame() const
{
    return nullptr;
}


class EasyHighscoreFactory : public GameFactory<HighScore>
{
public:
    [[nodiscard]] std::shared_ptr<HighScore> createHighScore(int score) const override;

    [[nodiscard]] std::shared_ptr<HighScore> createGame() const override;
};

inline std::shared_ptr<HighScore> EasyHighscoreFactory::createHighScore(const int score) const
{
    return std::make_shared<EasyHighScore>(score);
}

inline std::shared_ptr<HighScore> EasyHighscoreFactory::createGame() const
{
    return nullptr;
}


class MediumHighscoreFactory : public GameFactory<HighScore>
{
public:
    [[nodiscard]] std::shared_ptr<HighScore> createHighScore(int score) const override;

    [[nodiscard]] std::shared_ptr<HighScore> createGame() const override;
};

inline std::shared_ptr<HighScore> MediumHighscoreFactory::createHighScore(const int score) const
{
    return std::make_shared<MediumHighScore>(score);
}

inline std::shared_ptr<HighScore> MediumHighscoreFactory::createGame() const
{
    return nullptr;
}


class HardHighscoreFactory : public GameFactory<HighScore>
{
public:
    [[nodiscard]] std::shared_ptr<HighScore> createHighScore(int score) const override;

    [[nodiscard]] std::shared_ptr<HighScore> createGame() const override;
};

inline std::shared_ptr<HighScore> HardHighscoreFactory::createHighScore(const int score) const
{
    return std::make_shared<HardHighScore>(score);
}

inline std::shared_ptr<HighScore> HardHighscoreFactory::createGame() const
{
    return nullptr;
}


class CustomHighscoreFactory : public GameFactory<HighScore>
{
public:
    [[nodiscard]] std::shared_ptr<HighScore> createHighScore(int score) const override;

    [[nodiscard]] std::shared_ptr<HighScore> createGame() const override;
};

inline std::shared_ptr<HighScore> CustomHighscoreFactory::createHighScore(const int score) const
{
    return std::make_shared<CustomHighScore>(score);
}

inline std::shared_ptr<HighScore> CustomHighscoreFactory::createGame() const
{
    return nullptr;
}


#endif //GAMEFACTORY_H
