//
// Created by Windows on 05.05.2025.
//

#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <memory>
#include "Game.h"

class GameController {
private:
    std::unique_ptr<Game> gameMode;

public:
    explicit GameController(std::unique_ptr<Game> mode);

    void run() const;
};




#endif //GAMECONTROLLER_H