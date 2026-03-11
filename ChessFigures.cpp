#include "ChessFigures.h"
#include "ChessDesk.h"

#include <cmath>

// BASE CLASS CHESSFIGURES
ChessFigures::ChessFigures(char figure, Color color) : m_figure(figure), m_color(color) {}

char ChessFigures::getFigure() const { return m_figure; }

bool ChessFigures::isClear(int fromX, int fromY, int toX, int toY, const ChessDesk& desk) const
{
	if (fromX == toX && fromY == toY) return false;

	int stepX = (toX > fromX) ? 1 : (toX < fromX ? -1 : 0);
	int stepY = (toY > fromY) ? 1 : (toY < fromY ? -1 : 0);
	int currX = fromX + stepX;
	int currY = fromY + stepY;

	while (currX != toX || currY != toY)
	{
		if (desk.getAt(currX, currY) != nullptr) return false;
		currX += stepX;
		currY += stepY;
	}
	return true;
}

bool ChessFigures::canCapture(int toX, int toY, const ChessDesk& desk) const
{
	auto target = desk.getAt(toX, toY);
	return target == nullptr || target->getColor() != getColor();
}

bool ChessFigures::valid(int fromX, int fromY, int toX, int toY) const
{
	if (fromX == toX && fromY == toY) return false;
	if (toX < 0 || toX >= BOARD_SIZE || toY < 0 || toY >= BOARD_SIZE) return false;
	if (fromX < 0 || fromX >= BOARD_SIZE || fromY < 0 || fromY >= BOARD_SIZE) return false;

	return true;
}

// Pawn
Pawn::Pawn(Color color) : ChessFigures((color == Color::White ? 'P' : 'p'), color) {}

bool Pawn::canMove(int fromX, int fromY, int toX, int toY, const ChessDesk& desk) const
{
	if (!valid(fromX, fromY, toX, toY)) return false;

	int step = (getColor() == Color::White) ? -1 : 1;

	if (toX == fromX + step && toY == fromY)
	{
		return desk.getAt(toX, toY) == nullptr;
	}

	int start = (getColor() == Color::White) ? 6 : 1;

	if (fromX == start && toX == fromX + 2 * step && toY == fromY)
	{
		return desk.getAt(toX, toY) == nullptr && desk.getAt(fromX + step, fromY) == nullptr;
	}

	if (toX == fromX + step && (toY == fromY + 1 || toY == fromY - 1))
	{
		auto target = desk.getAt(toX, toY);
		return target != nullptr && target->getColor() != getColor();
	}

	return false;
}

std::string Pawn::getName() const { return "pawn"; }

// Bishop
Bishop::Bishop(Color color) : ChessFigures((color == Color::White ? 'B' : 'b'), color) {}

bool Bishop::canMove(int fromX, int fromY, int toX, int toY, const ChessDesk& desk) const
{
	if (!valid(fromX, fromY, toX, toY)) return false;

	if (std::abs(toX - fromX) != std::abs(toY - fromY)) return false;
	if (!isClear(fromX, fromY, toX, toY, desk)) return false;

	return canCapture(toX, toY, desk);
}

std::string Bishop::getName() const { return "bishop"; }

// Knight
Knight::Knight(Color color) : ChessFigures((color == Color::White ? 'N' : 'n'), color) {}

bool Knight::canMove(int fromX, int fromY, int toX, int toY, const ChessDesk& desk) const
{
	if (!valid(fromX, fromY, toX, toY)) return false;

	int dx = std::abs(toX - fromX);
	int dy = std::abs(toY - fromY);
	if (!((dx == 2 && dy == 1) || (dx == 1 && dy == 2))) return false;

	return canCapture(toX, toY, desk);
}

std::string Knight::getName() const { return "knight"; }

// Rook
Rook::Rook(Color color) : ChessFigures((color == Color::White ? 'R' : 'r'), color) {}

bool Rook::canMove(int fromX, int fromY, int toX, int toY, const ChessDesk& desk) const
{
	if (!valid(fromX, fromY, toX, toY)) return false;

	if (fromX != toX && fromY != toY) return false;
	if (!isClear(fromX, fromY, toX, toY, desk)) return false;

	return canCapture(toX, toY, desk);
}

std::string Rook::getName() const { return "rook"; }

// Queen 
Queen::Queen(Color color) : ChessFigures((color == Color::White ? 'Q' : 'q'), color) {}

bool Queen::canMove(int fromX, int fromY, int toX, int toY, const ChessDesk& desk) const
{
	if (!valid(fromX, fromY, toX, toY)) return false;

	bool straight = (fromX == toX || fromY == toY);
	bool diagonal = (std::abs(toX - fromX) == std::abs(toY - fromY));

	if (!(straight || diagonal)) return false;
	if (!isClear(fromX, fromY, toX, toY, desk)) return false;

	return canCapture(toX, toY, desk);
}

std::string Queen::getName() const { return "queen"; }

// King
King::King(Color color) : ChessFigures((color == Color::White ? 'K' : 'k'), color) {}

bool King::canMove(int fromX, int fromY, int toX, int toY, const ChessDesk& desk) const
{
	if (!valid(fromX, fromY, toX, toY)) return false;

	int dx = std::abs(toX - fromX);
	int dy = std::abs(toY - fromY);
	if (dx > 1 || dy > 1) return false;

	return canCapture(toX, toY, desk);
}

std::string King::getName() const { return "king"; }
