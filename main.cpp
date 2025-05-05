// #include <SFML/Graphics.hpp>

#include <iostream>

#include "Game.h"
#include "EasyGame.h"
#include "GameController.h"
#include "MediumGame.h"
#include "HardGame.h"


int main()
{
    int choice;
    std::cout << "Choose gamemode: \n1. Easy \n2. Medium \n3. Hard" << std::endl;
    std::cin >> choice;

    std::shared_ptr<Game> mode;

    switch (choice)
    {
    case 1: mode = std::make_shared<EasyGame>();
        break;
    case 2: mode = std::make_shared<MediumGame>();
        break;
    case 3: mode = std::make_shared<HardGame>();
        break;
    default:
        std::cout << "Invalid choice. Defaulting to Easy.\n";
        mode = std::make_shared<EasyGame>();
        break;
    }

    const GameController controller(std::move(mode));
    controller.run();

    // EasyGame game(minefield, player, std::chrono::minutes(10), std::chrono::minutes(3));
    // game.play();

    /// Observație: dacă aveți nevoie să citiți date de intrare de la tastatură,
    /// dați exemple de date de intrare folosind fișierul tastatura.txt
    /// Trebuie să aveți în fișierul tastatura.txt suficiente date de intrare
    /// (în formatul impus de voi) astfel încât execuția programului să se încheie.
    /// De asemenea, trebuie să adăugați în acest fișier date de intrare
    /// pentru cât mai multe ramuri de execuție.
    /// Dorim să facem acest lucru pentru a automatiza testarea codului, fără să
    /// mai pierdem timp de fiecare dată să introducem de la zero aceleași date de intrare.
    ///
    /// Pe GitHub Actions (bife), fișierul tastatura.txt este folosit
    /// pentru a simula date introduse de la tastatură.
    /// Bifele verifică dacă programul are erori de compilare, erori de memorie și memory leaks.
    ///
    /// Dacă nu puneți în tastatura.txt suficiente date de intrare, îmi rezerv dreptul să vă
    /// testez codul cu ce date de intrare am chef și să nu pun notă dacă găsesc vreun bug.
    /// Impun această cerință ca să învățați să faceți un demo și să arătați părțile din
    /// program care merg (și să le evitați pe cele care nu merg).


    /// Pentru date citite din fișier, NU folosiți tastatura.txt. Creați-vă voi
    /// alt fișier propriu cu ce alt nume doriți.
    /// Exemplu:
    /// std::ifstream fis("date.txt");
    /// for(int i = 0; i < nr2; ++i)
    ///     fis >> v2[i];
    ///
    ///////////////////////////////////////////////////////////////////////////
    ///                Exemplu de utilizare cod generat                     ///
    ///////////////////////////////////////////////////////////////////////////
    // Helper helper;
    // helper.help();

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
    return 0;
}
