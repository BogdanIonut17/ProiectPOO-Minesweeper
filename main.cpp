// #include <SFML/Graphics.hpp>

#include <iostream>

#include "Exceptions.h"
#include "GameController.h"


int main()
{
    try
    {
        GameController controller;
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


    // sf::RenderWindow window;
    ///////////////////////////////////////////////////////////////////////////
    /// NOTE: sync with env variable APP_WINDOW from .github/workflows/cmake.yml:31
    // window.create(sf::VideoMode({800, 700}), "MineMaster", sf::Style::Default);

    ///////////////////////////////////////////////////////////////////////////
    /// NOTE: mandatory use one of vsync or FPS limit (not both)            ///
    /// This is needed so we do not burn the GPU                            ///
    // window.setVerticalSyncEnabled(true);
    /// window.setFramerateLimit(60);                                       ///

    // while (window.isOpen())
    // {
    //     bool shouldExit = false;
    //     sf::Event e{};
    //     while (window.pollEvent(e))
    //     {
    //         switch (e.type)
    //         {
    //         case sf::Event::Closed:
    //             window.close();
    //             break;
    //         case sf::Event::Resized:
    //             std::cout << "New width: " << window.getSize().x << '\n'
    //                 << "New height: " << window.getSize().y << '\n';
    //             break;
    //         case sf::Event::KeyPressed:
    //             std::cout << "Received key " << (e.key.code == sf::Keyboard::X ? "X" : "(other)") << "\n";
    //             if (e.key.code == sf::Keyboard::Escape)
    //                 shouldExit = true;
    //             break;
    //         default:
    //             break;
    //         }
    //     }
    //     if (shouldExit)
    //     {
    //         window.close();
    //         break;
    //     }
    //     using namespace std::chrono_literals;
    //     std::this_thread::sleep_for(300ms);
    //
    //     window.clear();
    //     window.display();
    // }
}
