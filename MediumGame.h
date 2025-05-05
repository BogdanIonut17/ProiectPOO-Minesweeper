//
// Created by Windows on 04.05.2025.
//

#ifndef MEDIUMGAME_H
#define MEDIUMGAME_H
#include "Game.h"


class MediumGame : public Game{
public:
    MediumGame();

    MediumGame(const MediumGame& other);

    MediumGame& operator=(const MediumGame& other);

    void setupRound() override;

    void play() override;

    void displayMode(std::ostream&) const override;

    // [[nodiscard]] std::shared_ptr<Game> clone() const override;
};



#endif //MEDIUMGAME_H
