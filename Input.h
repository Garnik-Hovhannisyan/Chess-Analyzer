#ifndef INPUT_H
#define INPUT_H

#include "ChessDesk.h"

class Input
{
private:
	std::unique_ptr<ChessFigures> createFigure(const std::string& type, Color color);
	static void toLower(std::string& text);
	bool process(const std::string& line, ChessDesk& desk);

public:
	bool addFromConsole(ChessDesk& desk);
	bool addFromFile(ChessDesk& desk, const std::string& filename);
};

#endif // INPUT_H