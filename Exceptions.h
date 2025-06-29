//
// Created by Windows on 19.05.2025.
//

#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H


#include <stdexcept>
#include <string>

class GameError : public std::runtime_error
{
    using std::runtime_error::runtime_error;
};

class InvalidConfiguration : public GameError
{
public:
    InvalidConfiguration(const int rows, const int cols, const int mines)
        : GameError("Invalid configuration: " +
            std::to_string(rows) + "x" +
            std::to_string(cols) + " with " +
            std::to_string(mines) + " mines.\nNumber of mines must be < rows * cols.")
    {
    }
};

class InvalidRoundDuration : public GameError
{
public:
    explicit InvalidRoundDuration(const int minutes)
        : GameError("Invalid round duration: " + std::to_string(minutes) + " minutes. Number must be between 1 and 59.")
    {
    }
};

class InputReadError : public GameError
{
public:
    InputReadError() : GameError("Failed to read numerical value input from user.")
    {
    }
};


#endif //EXCEPTIONS_H
