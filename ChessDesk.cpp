#include <iostream>

#include "ChessDesk.h"

ChessDesk::ChessDesk() : Matrix(8, 8) {}

void ChessDesk::print()
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        std::cout << BOARD_SIZE - i << " ";
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (m_matrix[i][j] != nullptr)
            {
                std::cout << m_matrix[i][j]->getFigure() << " ";
            }
            else
            {
                std::cout << "_ ";
            }
        }
        std::cout << std::endl;
    }

    std::cout << "  ";

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        std::cout << char('a' + i) << " ";
    }
}

void ChessDesk::reset()
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            setAt(i, j, nullptr);
        }
    }
}

bool ChessDesk::isAttacked(int x, int y, Color attackerColor) const 
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            ChessFigures* f = getAt(i, j);
            if (f != nullptr && f->getColor() == attackerColor) 
            {
                if (f->getFigure() == 'P' || f->getFigure() == 'p')
                {
                    int step = (attackerColor == Color::White) ? -1 : 1;

                    if (x == i + step && (y == j + 1 || y == j - 1)) return true;
                }
                else
                {
                    if (f->canMove(i, j, x, y, *this)) return true;
                }
            }
        }
    }

    return false;
}

bool ChessDesk::isCheck(Color kingColor) const 
{
    int kingX = -1, kingY = -1;
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            ChessFigures* f = getAt(i, j);
            if (f != nullptr && f->getFigure() == (kingColor == Color::White ? 'K' : 'k'))
            {
                kingX = i; 
                kingY = j;
                break;
            }
        }
    }
    if (kingX == -1 || kingY == -1) return false;

    Color attackerColor = (kingColor == Color::White) ? Color::Black : Color::White;

    return isAttacked(kingX, kingY, attackerColor);
}

bool ChessDesk::hasLegalMoves(Color kingColor)
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            ChessFigures* f = getAt(i, j);
            if (f != nullptr && f->getColor() == kingColor)
            {
                for (int x = 0; x < BOARD_SIZE; x++)
                {
                    for (int y = 0; y < BOARD_SIZE; y++)
                    {
                        if (f->canMove(i, j, x, y, *this))
                        {
                            auto captured = makeMove(i, j, x, y);

                            bool stillCheck = isCheck(kingColor);

                            unmakeMove(i, j, x, y, std::move(captured));

                            if (!stillCheck) return true;
                        }
                    }
                }
            }
        }
    }

    return false;
}

bool ChessDesk::isMate(Color kingColor)
{
    return isCheck(kingColor) && !hasLegalMoves(kingColor);
}

bool ChessDesk::isStalemate(Color kingColor)
{
    return !isCheck(kingColor) && !hasLegalMoves(kingColor);
}

bool ChessDesk::isValidPosition() const
{
    bool whiteKingFound = false;
    bool blackKingFound = false;

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            ChessFigures* f = getAt(i, j);
            if (f != nullptr)
            {
                if (f->getFigure() == 'K') whiteKingFound = true;
                if (f->getFigure() == 'k') blackKingFound = true;
            }
        }
    }

    if (whiteKingFound == false || blackKingFound == false)
    {
        std::cout << "Error: Both kings must be on the board.\n";
        return false;
    }

    return true;
}

void ChessDesk::analyze()
{
    if (isValidPosition() == false) return;

    std::cout << '\n';

    if (mateInOne())
    {
        std::cout << "White can mate in one move\n";
        return;
    }
    else if (mateInTwo())
    {
        std::cout << "White can mate in two moves\n";
        return;
    }
    else
    {
        std::cout << "There is no checkmate in one or two moves.\n";

        std::string colorName = "Black";

        if (isCheck(Color::Black))
        {
            if (isMate(Color::Black)) std::cout << colorName << " king is checkmated.\n";
            else std::cout << colorName << " king is in check position.\n";
        }
        else
        {
            std::cout << colorName << " king is safe.\n";
        }
    }
}

bool ChessDesk::mateInOne()
{
    Color playerColor = Color::White;

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            ChessFigures* f = getAt(i, j);
            if (f != nullptr && f->getColor() == playerColor)
            {
                for (int x = 0; x < BOARD_SIZE; x++)
                {
                    for (int y = 0; y < BOARD_SIZE; y++)
                    {
                        if (f->canMove(i, j, x, y, *this))
                        {
                            if (simulateMove(i, j, x, y, [&]()
                                {
                                    return !isCheck(Color::White) && isMate(Color::Black);
                                }))
                            {
                                return true;
                            }
                        }
                    }
                }
            }
        }
    }

    return false;
}

bool ChessDesk::mateInTwo()
{
    Color playerColor = Color::White;

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            ChessFigures* f = getAt(i, j);
            if (f != nullptr && f->getColor() == playerColor)
            {
                for (int x = 0; x < BOARD_SIZE; x++)
                {
                    for (int y = 0; y < BOARD_SIZE; y++)
                    {
                        if (f->canMove(i, j, x, y, *this))
                        {
                            bool temp = simulateMove(i, j, x, y, [&]
                            {
                                if (isCheck(Color::White)) return false;

                                return leadToMate();
                            });

                            if (temp) return true;
                        }
                    }
                }
            }
        }
    }

    return false;
}

std::unique_ptr<ChessFigures> ChessDesk::makeMove(int fromX, int fromY, int toX, int toY)
{
    std::unique_ptr<ChessFigures> captured = std::move(m_matrix[toX][toY]);

    m_matrix[toX][toY] = std::move(m_matrix[fromX][fromY]);

    return captured;
}

void ChessDesk::unmakeMove(int fromX, int fromY, int toX, int toY, std::unique_ptr<ChessFigures> capturedFigure)
{
    m_matrix[fromX][fromY] = std::move(m_matrix[toX][toY]);

    m_matrix[toX][toY] = std::move(capturedFigure);
}

bool ChessDesk::leadToMate()
{
    Color opponentColor = Color::Black;
    bool hasLegalMove = false;
    for (int u = 0; u < BOARD_SIZE; u++)
    {
        for (int v = 0; v < BOARD_SIZE; v++)
        {
            ChessFigures* opp = getAt(u, v);
            if (opp != nullptr && opp->getColor() == opponentColor)
            {
                for (int p = 0; p < BOARD_SIZE; p++)
                {
                    for (int q = 0; q < BOARD_SIZE; q++)
                    {
                        if (opp->canMove(u, v, p, q, *this))
                        {
                            bool result = simulateMove(u, v, p, q, [&]() 
                                {
                                    if (isCheck(opponentColor)) return true;

                                    hasLegalMove = true;

                                    return mateInOne();
                                });
                            if (!result) return false;
                        }
                    }
                }
            }
        }
    }
    return hasLegalMove;
}