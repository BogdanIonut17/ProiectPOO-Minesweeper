#include <iostream>
// #include <array>
#include <chrono>
#include <thread>

#include <SFML/Graphics.hpp>

#include <Helper.h>
#include <ostream>

//////////////////////////////////////////////////////////////////////
/// This class is used to test that the memory leak checks work as expected even when using a GUI
class SomeClass {
public:
    explicit SomeClass(int)
    {
    }
};


class Cell
{
private:
    bool hasMine;
    bool isRevealed;
    bool isFlagged;
    int adjacentMines;

public:
    explicit Cell(const bool hasMine = false) : hasMine(hasMine), isRevealed(false), isFlagged(false), adjacentMines(0)
    {
    }

    ~Cell()
    {
    }

    Cell(const Cell& other) : hasMine(other.hasMine), isRevealed(other.isRevealed), isFlagged(other.isFlagged),
                              adjacentMines(other.adjacentMines)
    {
    }

    Cell& operator=(const Cell& other)
    {
        if (this == &other)
            return *this;
        hasMine = other.hasMine;
        isRevealed = other.isRevealed;
        isFlagged = other.isFlagged;
        adjacentMines = other.adjacentMines;
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& os, const Cell& cell)
    {
        if (cell.isRevealed)
        {
            if (cell.hasMine)
            {
                os << "*";
            }
            else os << cell.adjacentMines;
        }
        else if (cell.isFlagged)
        {
            os << "F";
        }
        else os << "#";
        return os;
    }
};

class Minefield
{
private:
    int rows, cols, mineCount;
    std::vector<std::vector<Cell>> grid;

public:
    Minefield(const int rows, const int cols, const int mineCount) : rows(rows), cols(cols), mineCount(mineCount),
                                                                     grid(rows, std::vector<Cell>(cols))
    {
    }

    friend std::ostream& operator<<(std::ostream& os, const Minefield& minefield)
    {
        for (int i = 0; i < minefield.rows; i++)
        {
            for (int j = 0; j < minefield.cols; j++)
            {
                os << minefield.grid[i][j] << " ";
            }
            os << std::endl;
        }
        return os;
    }
};

class Player
{
private:
    std::string nickname;
    int highscore;

public:
    explicit Player(const std::string& nickname) : nickname(nickname), highscore(0)
    {
    }

    friend std::ostream& operator<<(std::ostream& os, const Player& player)
    {
        os << player.nickname << " " << player.highscore;
        return os;
    }
};


class Game
{
private:
    Minefield minefield;
    Player player;
    bool gameOver;

public:
    // Game(const int rows, const int cols, const int mineCount, const std::string& nickname): field(rows, cols, mineCount), player(nickname), gameOver(false) {}
    Game(const Minefield& minefield, const Player& player) : minefield(minefield), player(player),
                                                             gameOver(false)
    {
    }

    friend std::ostream& operator<<(std::ostream& os, const Game& game)
    {
        os << "Player: " << game.player << std::endl;
        os << "Minefield: \n" << game.minefield << std::endl;
        if (game.gameOver)
        {
            os << "Game Over" << std::endl;
        }
        return os;
    }
};

SomeClass* getC()
{
    return new SomeClass{2};
}


int main()
{
    // Minefield minefield(8,8,10);
    // Player player("Bogdan");
    // Game game(minefield, player);
    // std::cout << player << std::endl;
    // std::cout << minefield << std::endl;
    // std::cout << game << std::endl;

    /////////////////////////////////////////////////////////////////////////
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
    ///

    ///////////////////////////////////////////////////////////////////////////
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
    Helper helper;
    helper.help();
    ///////////////////////////////////////////////////////////////////////////

    SomeClass* c = getC();
    std::cout << c << "\n";
    delete c; // comentarea acestui rând ar trebui să ducă la semnalarea unui mem leak

    sf::RenderWindow window;
    ///////////////////////////////////////////////////////////////////////////
    /// NOTE: sync with env variable APP_WINDOW from .github/workflows/cmake.yml:31
    window.create(sf::VideoMode({800, 700}), "Minesweeper++", sf::Style::Default);

    ///////////////////////////////////////////////////////////////////////////
    /// NOTE: mandatory use one of vsync or FPS limit (not both)            ///
    /// This is needed so we do not burn the GPU                            ///
    window.setVerticalSyncEnabled(true);
    ///
       /// window.setFramerateLimit(60);                                       ///
       ///////////////////////////////////////////////////////////////////////////

    while (window.isOpen())
    {
        bool shouldExit = false;
        sf::Event e{};
        while (window.pollEvent(e))
        {
            switch (e.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::Resized:
                std::cout << "New width: " << window.getSize().x << '\n'
                    << "New height: " << window.getSize().y << '\n';
                break;
            case sf::Event::KeyPressed:
                std::cout << "Received key " << (e.key.code == sf::Keyboard::X ? "X" : "(other)") << "\n";
                if (e.key.code == sf::Keyboard::Escape)
                    shouldExit = true;
                break;
            default:
                break;
            }
        }
        if (shouldExit)
        {
            window.close();
            break;
        }
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(300ms);

        window.clear();
        window.display();
    }
    return 0;
}
