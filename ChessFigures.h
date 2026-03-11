#ifndef CHESSFIGURES_H
#define CHESSFIGURES_H

#include <string>

class ChessDesk;

enum class Color { White, Black };

// BASE CLASS
class ChessFigures
{
private:
	char m_figure;
	Color m_color;

public:
	ChessFigures(char figure, Color color);
	virtual ~ChessFigures() = default;

	Color getColor() const { return m_color; }
	virtual char getFigure() const;
	virtual std::string getName() const = 0;
	virtual bool canMove(int fromX, int fromY, int toX, int toY, const ChessDesk& desk) const = 0;

	bool isClear(int fromX, int fromY, int toX, int toY, const ChessDesk& desk) const;
	bool canCapture(int toX, int toY, const ChessDesk& desk) const;
	bool valid(int fromX, int fromY, int toX, int toY) const;
};

// Pawn
class Pawn : public ChessFigures
{
public:
	Pawn(Color color);

	bool canMove(int fromX, int fromY, int toX, int toY, const ChessDesk& desk) const override;
	std::string getName() const override;
};

// Bishop
class Bishop : public ChessFigures
{
public:
	Bishop(Color color);

	bool canMove(int fromX, int fromY, int toX, int toY, const ChessDesk& desk) const override;
	std::string getName() const override;
};

// Knight
class Knight : public ChessFigures
{
public:
	Knight(Color color);

	bool canMove(int fromX, int fromY, int toX, int toY, const ChessDesk& desk) const override;
	std::string getName() const override;
};

// Rook
class Rook : public ChessFigures
{
public:
	Rook(Color color);

	bool canMove(int fromX, int fromY, int toX, int toY, const ChessDesk& desk) const override;
	std::string getName() const override;
};

// Queen
class Queen : public ChessFigures
{
public:
	Queen(Color color);

	bool canMove(int fromX, int fromY, int toX, int toY, const ChessDesk& desk) const override;
	std::string getName() const override;
};

// King
class King : public ChessFigures
{
public:
	King(Color color);

	bool canMove(int fromX, int fromY, int toX, int toY, const ChessDesk& desk) const override;
	std::string getName() const override;
};

#endif // CHESSFIGURES_H