#include <iostream>

#include "Exceptions.h"
#include "GameController.h"


int main()
{
    try
    {
        // GameController controller;
        auto& controller = GameController::getInstance();
        controller.run();
    }
    catch (const GameError& err)
    {
        std::cout << "\n[Game Error] " << err.what() << std::endl;
    }
    catch (const std::exception& err)
    {
        std::cout << "\n[Error] " << err.what() << std::endl;
    }
    catch (...)
    {
        std::cout << "\n[Unknown Error]" << std::endl;
    }
    return 0;

    // int choice;
    // std::cout << "Choose gamemode: \n1. Easy \n2. Medium \n3. Hard" << std::endl;
    // std::cin >> choice;
    //
    // std::shared_ptr<Game> mode;
    //
    // switch (choice)
    // {
    // case 1: mode = std::make_shared<EasyGame>();
    //     break;
    // case 2: mode = std::make_shared<MediumGame>();
    //     break;
    // case 3: mode = std::make_shared<HardGame>();
    //     break;
    // default:
    //     std::cout << "Invalid choice. Defaulting to Easy.\n";
    //     mode = std::make_shared<EasyGame>();
    //     break;
    // }

    // const GameController controller(std::move(mode));
    // controller.run();
}
