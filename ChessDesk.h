#ifndef CHESSDESK_H
#define CHESSDESK_H

#include "Matrix.h"
#include "ChessFigures.h"

constexpr int BOARD_SIZE = 8;

class ChessDesk : public Matrix
{
public:
	ChessDesk();
	~ChessDesk() = default;

	void print() override;
	void reset() override;
	void analyze();

private:
	bool isAttacked(int x, int y, Color attackerColor) const;
	bool isCheck(Color kingColor) const;
	bool isMate(Color kingColor);
	bool isValidPosition() const;
	bool isStalemate(Color kingColor);
	bool hasLegalMoves(Color kingColor);
	bool mateInOne();
	bool mateInTwo();

	std::unique_ptr<ChessFigures> makeMove(int fromX, int fromY, int toX, int toY);
	void unmakeMove(int fromX, int fromY, int toX, int toY, std::unique_ptr<ChessFigures> capturedFigure);

	template<typename Func>
	bool simulateMove(int fromX, int fromY, int toX, int toY, Func func) 
	{
		auto captured = makeMove(fromX, fromY, toX, toY);
		bool result = func();
		unmakeMove(fromX, fromY, toX, toY, std::move(captured));
		return result;
	}

	bool leadToMate();
};

#endif // CHESSDESK_H