#include <iostream>
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

    [[nodiscard]] bool isMine() const
    {
        return hasMine;
    }

    [[nodiscard]] bool checkIfRevealed() const
    {
        return isRevealed;
    }

    [[nodiscard]] bool checkIfFlagged() const
    {
        return isFlagged;
    }

    void reveal()
    {
        isRevealed = true;
    }

    void toggleFlag()
    {
        isFlagged = !isFlagged;
    }

    void setAdjacentMines(const int adjacent_mines)
    {
        adjacentMines = adjacent_mines;
    }

    [[nodiscard]] int getAdjacentMines() const
    {
        return adjacentMines;
    }
};

class Minefield
{
private:
    int rows, cols, mineCount;
    std::vector<std::vector<Cell>> grid;
    bool firstMove;

    [[nodiscard]] bool isValidMove(const int x, const int y) const
    {
        return x >= 0 && x < rows && y >= 0 && y < cols;
    }

    void generateMines(const int firstX, const int firstY)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution distRow(0, rows - 1);
        std::uniform_int_distribution distCol(0, cols - 1);
        int minesPlaced = 0;
        const int totalMineCount = mineCount + countFlags();
        while (minesPlaced < totalMineCount)
        {
            const int mineX = distRow(gen);
            const int mineY = distCol(gen);
            if ((mineX == firstX && mineY == firstY) || grid[mineX][mineY].isMine())
                continue;
            bool flagged = false;
            if (grid[mineX][mineY].checkIfFlagged())
                flagged = true;
            grid[mineX][mineY] = Cell(true);
            if (flagged)
            {
                grid[mineX][mineY].toggleFlag();
            }
            minesPlaced++;
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

    [[nodiscard]] int countFlags() const
    {
        int flagCount = 0;
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                if (grid[i][j].checkIfFlagged())
                {
                    flagCount++;
                }
            }
        }
        return flagCount;
    }

    void BFSReveal(int startX, int startY)
    {
        if (!isValidMove(startX, startY))
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

    void chordReveal(const int x, const int y)
    {
        int flagCount = 0;
        std::vector<std::pair<int, int>> toReveal;

        for (int dx = -1; dx <= 1; dx++)
        {
            for (int dy = -1; dy <= 1; dy++)
            {
                int adjacentX = x + dx, adjacentY = y + dy;
                if (isValidMove(adjacentX, adjacentY))
                {
                    if (grid[adjacentX][adjacentY].checkIfFlagged())
                    {
                        flagCount++;
                    }
                    else if (!grid[adjacentX][adjacentY].checkIfRevealed())
                    {
                        toReveal.emplace_back(adjacentX, adjacentY);
                    }
                }
            }
        }

        if (flagCount == grid[x][y].getAdjacentMines())
        {
            for (const auto& [adjX, adjY] : toReveal)
            {
                revealCell(adjX, adjY);
            }
        }
    }

public:
    Minefield(const int rows, const int cols, const int mineCount) : rows(rows), cols(cols),
                                                                     mineCount(mineCount),
                                                                     grid(rows, std::vector<Cell>(cols)),
                                                                     firstMove(true)
    {
    }

    [[nodiscard]] int getRows() const { return rows; }

    [[nodiscard]] int getCols() const { return cols; }

    Cell& getCell(const int x, const int y) { return grid[x][y]; }

    [[nodiscard]] const Cell& getCell(const int x, const int y) const { return grid[x][y]; }

    void setFirstMove()
    {
        firstMove = true;
    }

    void revealCell(const int cellX, const int cellY)
    {
        if (grid[cellX][cellY].checkIfFlagged())
            return;
        if (grid[cellX][cellY].checkIfRevealed() && grid[cellX][cellY].getAdjacentMines() > 0)
        {
            chordReveal(cellX, cellY);
            return;
        }
        if (firstMove)
        {
            generateMines(cellX, cellY);
            countAdjacentMines();
            firstMove = false;
        }
        if (grid[cellX][cellY].getAdjacentMines() == 0 && !grid[cellX][cellY].isMine())
        {
            BFSReveal(cellX, cellY);
        }
        else
        {
            grid[cellX][cellY].reveal();
        }
    }

    void shuffleMines()
    {
        std::vector<std::pair<int, int>> emptyCells;
        std::vector<std::pair<int, int>> mines;

        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                if (!grid[i][j].checkIfRevealed())
                {
                    if (grid[i][j].checkIfFlagged())
                    {
                        grid[i][j].toggleFlag();
                    }
                    if (grid[i][j].isMine())
                    {
                        mines.emplace_back(i, j);
                    }
                    else
                    {
                        emptyCells.emplace_back(i, j);
                    }
                }
            }
        }

        if (emptyCells.size() < mines.size())
        {
            return;
        }

        for (auto& [x, y] : mines)
        {
            grid[x][y] = Cell(false);
        }

        std::random_device rd;
        std::mt19937 gen(rd());
        std::ranges::shuffle(emptyCells, gen);

        mineCount = mines.size();

        for (int i = 0; i < mineCount; i++)
        {
            auto [x, y] = emptyCells[i];
            grid[x][y] = Cell(true);
        }

        countAdjacentMines();
    }

    void setFieldSize(const int newRows, const int newCols, const int newMineCount)
    {
        if (newRows <= 0 || newCols <= 0 || newMineCount < 0 || newMineCount >= newRows * newCols)
        {
            std::cout << "Invalid board size or mine count!" << std::endl;
            return;
        }
        rows = newRows;
        cols = newCols;
        mineCount = newMineCount;
        grid = std::vector(rows, std::vector<Cell>(cols));
    }

    void flagCell(const int x, const int y)
    {
        if (!grid[x][y].checkIfRevealed())
        {
            grid[x][y].toggleFlag();
            if (grid[x][y].checkIfFlagged())
            {
                mineCount--;
            }
            else
            {
                mineCount++;
            }
        }
    }

    void processMove()
    {
        int cellX, cellY;
        char action;
        std::cout << "Enter a move (R x y to reveal, F x y to flag/unflag a cell or S to shuffle the mines): " <<
            std::endl;
        std::cin >> action;
        if (std::toupper(action) != 'R' && std::toupper(action) != 'F' && std::toupper(action) != 'S')
        {
            std::cout << "Invalid move! Enter R x y to reveal, F x y to flag/unflag a cell or S to shuffle the mines: "
                << std::endl;
            return;
        }
        if (std::toupper(action) == 'S')
        {
            shuffleMines();
            return;
        }
        std::cin >> cellX >> cellY;
        if (!isValidMove(cellX, cellY))
        {
            std::cout << "Invalid move! Try again!" << std::endl;
            return;
        }
        if (std::toupper(action) == 'R') revealCell(cellX, cellY);
        else if (std::toupper(action) == 'F') flagCell(cellX, cellY);
    }

    friend std::ostream& operator<<(std::ostream& os, const Minefield& minefield)
    {

        for (int i = 0; i < minefield.rows; i++)
        {
            for (int j = 0; j < minefield.cols; j++)
            {
                os << minefield.grid[i][j] << " ";
                // minefield.grid[i][j].display();
            }
            os << std::endl;
        }
        std::cout << "\nNumber of mines: " << minefield.mineCount << std::endl;
        return os;
    }
};

class Player
{
private:
    std::string nickname;
    int score;

public:
    explicit Player(const std::string& nickname) : nickname(nickname), score(0)
    {
    }

    [[nodiscard]] const std::string& getNickname() const
    {
        return nickname;
    }

    [[nodiscard]] int getScore() const
    {
        return score;
    }

    void setNickname(const std::string& newNickname)
    {
        nickname = newNickname;
    }

    void setScore(const int playerScore)
    {
        score = playerScore;
    }

    friend std::ostream& operator<<(std::ostream& os, const Player& player)
    {
        os << player.nickname << " " << player.score;
        return os;
    }
};

class Game
{
private:
    Minefield minefield;
    Player player;
    bool gameOver;
    std::chrono::steady_clock::time_point startTime;
    std::chrono::milliseconds totalTime;
    std::atomic<bool> timeExpired;
    bool firstGame;

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
                    std::cout << "\nYou revealed a mine!" << std::endl;
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
        const auto endTime = std::chrono::steady_clock::now();
        const std::chrono::milliseconds elapsedTime =
            std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
        const std::chrono::milliseconds remainingTime = totalTime - elapsedTime;

        const int playerScore = std::max(0, static_cast<int>(remainingTime.count()));
        player.setScore(playerScore);

        std::cout << "\nYou won!" << std::endl;
        setGameOver();
        return true;
    }

    void setTimeout(const std::function<void()>& func, std::chrono::milliseconds delay)
    {
        std::thread([func, delay]
        {
            std::this_thread::sleep_for(delay);
            func();
        }).detach();
    }

    void displayRemainingTime() const
    {
        const auto now = std::chrono::steady_clock::now();
        const auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - startTime).count();
        int remaining = std::chrono::duration_cast<std::chrono::seconds>(totalTime).count() - elapsed;
        if (remaining < 0) remaining = 0;

        const int minutes = remaining / 60;
        const int seconds = remaining % 60;

        std::cout << "Time left: " << minutes << ":" << (seconds < 10 ? "0" : "") << seconds  << std::endl;
    }

public:
    Game(const Minefield& minefield, const Player& player) : minefield(minefield), player(player),
                                                             gameOver(false), startTime(std::chrono::steady_clock::now()),totalTime(std::chrono::minutes(5)),
                                                             timeExpired(false), firstGame(true)
    {
    }

    friend std::ostream& operator<<(std::ostream& os, const Game& game)
    {
        if (game.gameOver)
        {
            os << "Game over!" << std::endl;
        }
        os << "Player: " << game.player.getNickname() << std::endl;
        os << "Score: " << game.player.getScore() << std::endl;
        os << "Minefield: " << std::endl;
        os << game.minefield << std::endl;
        return os;
    }

    void play()
    {
        int newRows = 8, newCols = 8, newMineCount = 9;
        std::cout << "Enter board size (rows, cols) and number of mines: " << std::endl;
        std::cin >> newRows >> newCols >> newMineCount;
        minefield.setFieldSize(newRows, newCols, newMineCount);

        std::cout << "Enter your nickname: " << std::endl;
        std::string newNickname;
        std::cin >> newNickname;
        player.setNickname(newNickname);

        std::cout << "Welcome to MineMaster, " << player.getNickname() << "!" << std::endl;

        setTimeout([this]
        {
            if (!gameOver)
            {
                std::cout << "\nTime's up! Game over!" << std::endl;
                timeExpired = true;
                setGameOver();
            }
            exit(0);
        }, totalTime);

        if (firstGame)
        {
            startTime = std::chrono::steady_clock::now();
            firstGame = false;
        }

        while (!timeExpired && !isGameOver())
        {
            displayRemainingTime();
            std::cout << "\n" << minefield << std::endl;
            minefield.processMove();
        }

        if (timeExpired)
            return;

        displayRemainingTime();
        std::cout << "\n" << *this << std::endl;

        char choice;
        std::cout << "Play again? (y/n): " << std::endl;
        std::cin >> choice;
        if (std::toupper(choice) == 'Y')
        {
            gameOver = false;
            timeExpired = false;
            minefield.setFirstMove();
            play();
        }
    }
};
SomeClass* getC()
{
    return new SomeClass{2};
}


int main()
{
    Minefield minefield(8, 8, 9);
    Player player("Player1");
    Game game(minefield, player);
    game.play();

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
    Helper helper;
    helper.help();

    SomeClass* c = getC();
    std::cout << c << "\n";
    delete c; // comentarea acestui rând ar trebui să ducă la semnalarea unui mem leak

    sf::RenderWindow window;
    ///////////////////////////////////////////////////////////////////////////
    /// NOTE: sync with env variable APP_WINDOW from .github/workflows/cmake.yml:31
    window.create(sf::VideoMode({800, 700}), "MineMaster", sf::Style::Default);

    ///////////////////////////////////////////////////////////////////////////
    /// NOTE: mandatory use one of vsync or FPS limit (not both)            ///
    /// This is needed so we do not burn the GPU                            ///
    window.setVerticalSyncEnabled(true);
    /// window.setFramerateLimit(60);                                       ///

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
        if (shouldExit){
        
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

