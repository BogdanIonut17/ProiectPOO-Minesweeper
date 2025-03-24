#include <iostream>
// #include <array>
#include <chrono>
#include <thread>
#include <SFML/Graphics.hpp>
#include <queue>
#include <Helper.h>
#include <ostream>
#include <random>
#include <functional>

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
            if (cell.hasMine) os << "*";
            else if (cell.adjacentMines)
            {
                os << cell.adjacentMines;
            }
            else
                os << ".";
        }
        else if (cell.isFlagged)
        {
            os << "F";
        }
        else
        {
            os << "#";
        }
        return os;
    }

    bool isMine() const
    {
        return hasMine;
    }

    bool checkIfRevealed() const
    {
        return isRevealed;
    }

    void reveal()
    {
        isRevealed = true;
    }

    void toggleFlag()
    {
        isFlagged = !isFlagged;
    }

    void setAdjacentMines(int adjacent_mines)
    {
        adjacentMines = adjacent_mines;
    }

    int getAdjacentMines() const
    {
        return adjacentMines;
    }
};

class Minefield
{
private:
    int rows, cols, mineCount;
    std::vector<std::vector<Cell>> grid;

public:
    // Minefield() = default;

    Minefield(const int rows = 8, const int cols = 8, const int mineCount = 9) : rows(rows), cols(cols),
        mineCount(mineCount),
        grid(rows, std::vector<Cell>(cols))
    {
    }

    int getRows() const { return rows; }
    int getCols() const { return cols; }
    Cell& getCell(int x, int y) { return grid[x][y]; }
    const Cell& getCell(int x, int y) const { return grid[x][y]; }

    void generateMines()
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> distRow(0, rows - 1);
        std::uniform_int_distribution<int> distCol(0, cols - 1);
        int minesPlaced = 0;
        while (minesPlaced < mineCount)
        {
            int mine_x = distRow(gen);
            int mine_y = distCol(gen);
            if (!grid[mine_x][mine_y].isMine())
            {
                grid[mine_x][mine_y] = Cell(true);
                minesPlaced++;
            }
        }
    }

    void countAdjacentMines()
    {
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                if (grid[i][j].isMine()) continue;
                int count = 0;
                for (int dx = -1; dx <= 1; dx++)
                {
                    for (int dy = -1; dy <= 1; dy++)
                    {
                        int x = i + dx, y = j + dy;
                        if (isValidMove(x, y) && grid[x][y].isMine())
                            count++;
                    }
                }
                grid[i][j].setAdjacentMines(count);
            }
        }
    }

    void revealCell(const int cell_x, const int cell_y)
    {
        if (grid[cell_x][cell_y].checkIfRevealed())
            return;
        if (grid[cell_x][cell_y].getAdjacentMines() == 0)
        {
            BFSReveal(cell_x, cell_y);
        }
        else
        {
            grid[cell_x][cell_y].reveal();
        }
    }

    void BFSReveal(int startX, int startY)
    {
        if (!isValidMove(startX, startY) || grid[startX][startY].isMine())
        {
            return;
        }

        std::queue<std::pair<int, int>> q;
        q.emplace(startX, startY);
        std::vector visited(rows, std::vector(cols, false));

        while (!q.empty())
        {
            auto [x, y] = q.front();
            q.pop();

            if (visited[x][y]) continue;
            visited[x][y] = true;
            grid[x][y].reveal();

            if (grid[x][y].getAdjacentMines() > 0) continue;

            for (int dx = -1; dx <= 1; dx++)
            {
                for (int dy = -1; dy <= 1; dy++)
                {
                    int adjacentX = x + dx, adjacentY = y + dy;
                    if (isValidMove(adjacentX, adjacentY) && !grid[adjacentX][adjacentY].checkIfRevealed() && !visited[
                        adjacentX][adjacentY])
                    {
                        q.emplace(adjacentX, adjacentY);
                    }
                }
            }
        }
    }

    void setFieldSize(const int newRows, const int newCols, const int newMineCount)
    {
        if (newRows <= 0 || newCols <= 0 || newMineCount < 0 || newMineCount > newRows * newCols)
        {
            std::cout << "Invalid board size or mine count!" << std::endl;
            return;
        }
        rows = newRows;
        cols = newCols;
        mineCount = newMineCount;

        // grid = std::vector(rows, std::vector<Cell>(cols));
        grid.clear();
        grid.resize(rows, std::vector(cols, Cell(false)));
    }


    void flagCell(const int x, const int y)
    {
        grid[x][y].toggleFlag();
    }

    bool isValidMove(const int x, const int y) const
    {
        return x >= 0 && x < rows && y >= 0 and y < cols;
    }

    void processMove()
    {
        int cell_x, cell_y;
        char action;
        std::cout << "Enter a move (R x y to reveal, F x y to flag/unflag a cell): " << std::endl;
        std::cin >> action >> cell_x >> cell_y;
        if (!isValidMove(cell_x, cell_y))
        {
            std::cout << "Invalid move! Try again!" << std::endl;
            return;
        }
        if (action == 'R') revealCell(cell_x, cell_y);
        else if (action == 'F') flagCell(cell_x, cell_y);
        else std::cout << "Invalid move! Enter R x y to reveal, F x y to flag/unflag a cell: " << std::endl;
    }

    friend std::ostream& operator<<(std::ostream& os, const Minefield& minefield)
    {
        std::cout << "Total number of mines: " << minefield.mineCount << std::endl;
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

    const std::string& getNickname() const
    {
        return nickname;
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
        if (game.gameOver)
        {
            os << "Game over!" << std::endl;
        }
        os << "Player: " << game.player << std::endl;
        os << "Minefield: \n" << game.minefield << std::endl;
        return os;
    }

    void setGameOver()
    {
        gameOver = true;
    }

    bool isGameOver()
    {
        for (int i = 0; i < minefield.getRows(); i++)
        {
            for (int j = 0; j < minefield.getCols(); j++)
            {
                if (minefield.getCell(i, j).isMine() && minefield.getCell(i, j).checkIfRevealed())
                {
                    std::cout << "You revealed a mine!" << std::endl;
                    setGameOver();
                    return true;
                }
            }
        }
        for (int i = 0; i < minefield.getRows(); i++)
        {
            for (int j = 0; j < minefield.getCols(); j++)
            {
                if (!minefield.getCell(i, j).isMine() && !minefield.getCell(i, j).checkIfRevealed())
                    return false;
            }
        }
        std::cout << "You won!" << std::endl;
        setGameOver();
        return true;
    }

    void play()
    {
        std::cout << "Welcome to MineMaster, " << player.getNickname() << "!" << std::endl;
        setTimeout([]() {
            std::cout << "Time's up! You lost!" << std::endl;
            exit(0);
        }, std::chrono::minutes(10));

        while (!isGameOver())
        {
            std::cout << minefield << std::endl;
            minefield.processMove();
        }
        std::cout << *this << std::endl;
    }

    void setTimeout(std::function<void()> func, std::chrono::milliseconds delay)
    {
        std::thread([func, delay]()
        {
            std::this_thread::sleep_for(delay);
            func();
        }).detach();
    }
};

SomeClass* getC()
{
    return new SomeClass{2};
}


int main()
{
    Minefield minefield;

    // int newRows, newCols, newMineCount;
    // std::cout << "Enter board size (rows, cols) and number of mines: ";
    // std::cin >> newRows >> newCols >> newMineCount;
    // minefield.setFieldSize(newRows, newCols, newMineCount);

    minefield.generateMines();
    minefield.countAdjacentMines();

    Player player("Bogdan");
    Game game(minefield, player);
    game.play();

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
