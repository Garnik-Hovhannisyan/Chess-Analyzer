#include <iostream>
#include <string>
#include <memory>
#include <algorithm>
#include <cctype>
#include <sstream>
#include <fstream>

#include "Input.h"

void Input::toLower(std::string& text)
{
    std::transform(text.begin(), text.end(), text.begin(), [](unsigned char c)
    {
        return std::tolower(c);
    });
}

std::unique_ptr<ChessFigures> Input::createFigure(const std::string& type, Color color)
{
    if (type == "king")   return std::make_unique<King>(color);
    if (type == "queen")  return std::make_unique<Queen>(color);
    if (type == "rook")   return std::make_unique<Rook>(color);
    if (type == "bishop") return std::make_unique<Bishop>(color);
    if (type == "knight") return std::make_unique<Knight>(color);
    if (type == "pawn")   return std::make_unique<Pawn>(color);

    return nullptr;
}

bool Input::process(const std::string& line, ChessDesk& desk)
{
    std::string text = line;
    Color figureColor;
    int x, y;

    toLower(text);

    std::stringstream ss(text);
    std::string color, type, pos;

    if (!(ss >> color >> type >> pos))
    {
        throw std::runtime_error("Invalid input format: " + line);
    }

    // CHECK TYPE
    if (type != "king" && type != "queen" && type != "rook" &&
        type != "bishop" && type != "knight" && type != "pawn")
    {
        throw std::runtime_error("Invalid figrue type: " + type);
    }

    // CHECK COLOR
    if (color == "white") figureColor = Color::White;
    else if (color == "black") figureColor = Color::Black;
    else
    {
        throw std::runtime_error("Invalid color: " + color);
    }

    // CHECK POSITION
    if (pos.length() == 2 && pos[0] >= 'a' && pos[0] <= 'h' && pos[1] >= '1' && pos[1] <= '8')
    {
        x = 8 - (pos[1] - '0');
        y = pos[0] - 'a';
    }
    else
    {
        throw std::runtime_error("Invalid position: " + pos);
    }

    // CHECK KING COUNT
    if (type == "king")
    {
        int kingCount = 0;
        char kingSymbol = (figureColor == Color::White) ? 'K' : 'k';

        for (int i = 0; i < BOARD_SIZE; i++)
        {
            for (int j = 0; j < BOARD_SIZE; j++)
            {
                ChessFigures* f = desk.getAt(i, j);
                if (f != nullptr && f->getFigure() == kingSymbol) { kingCount++; }
            }
        }

        if (kingCount >= 1)
        {
            throw std::runtime_error("King already exists - Each color can have only one king.");
        }
    }

    // CHECK KINGS EACH OTHER
    if (type == "king")
    {
        for (int i = x - 1; i <= x + 1; i++)
        {
            for (int j = y - 1; j <= y + 1; j++)
            {
                if (i >= 0 && i < BOARD_SIZE && j >= 0 && j < BOARD_SIZE)
                {
                    ChessFigures* f = desk.getAt(i, j);
                    if (f != nullptr && (f->getFigure() == 'K' || f->getFigure() == 'k'))
                    {
                        throw std::runtime_error("Error: Kings cannot stand next to each other.");
                    }
                }
            }
        }
    }

    // CHECK PAWN POSITION
    if (type == "pawn" && (x == 0 || x == 7))
    {
        throw std::runtime_error("Pawns cannot be on the 1st or 8th lines");
    }

    // CHECK POSITION VALIDITY
    if (desk.getAt(x, y) != nullptr)
    {
        ChessFigures* existing = desk.getAt(x, y);
        throw std::runtime_error("Square occupied at " + pos + " by " +
            (existing->getColor() == Color::White ? "white " : "black ") +
            existing->getName());
    }

    // CHECK AND ADD FIUGRE TO DESK
    auto figure = createFigure(type, figureColor);
    if (figure == nullptr)
    {
        throw std::runtime_error("Unknown figure type: " + type);
    }
   
    desk.setAt(x, y, std::move(figure));
    std::cout << "Added: " << color << " " << type << " at " << pos << "\n";

    return true;
}

bool Input::addFromConsole(ChessDesk& desk)
{
    std::string text;
    std::cout << "\nPlease enter figure in format: [Color] [FigureType] [Position]\nor (type 'Exit' to finish input)\n";

    if (!std::getline(std::cin, text)) return false;
    if (text.empty()) return true;

    std::string lower = text;
    toLower(lower);
    if (lower == "exit") return false;

    try
    {
        return process(text, desk);
    }
    catch (const std::exception& ex)
    {
        std::cout << "Error: " << ex.what() << '\n';
        return true;
    }

}

bool Input::addFromFile(ChessDesk& desk, const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cout << "Can't open file: " << filename << std::endl;
        return false;
    }

    std::cout << "\nFile " << filename << " opened successfully.\n";

    desk.reset();
    std::string line;

    while (std::getline(file, line))
    {
        try
        {
            process(line, desk);
        }
        catch (const std::exception& ex)
        {
            std::cout << "Error: " << ex.what() << '\n';
            std::cout << "Stopping load from file\n";
            break;
        }
    }

    std::cout << "--- File loading finished ---\n";

    return true;
}