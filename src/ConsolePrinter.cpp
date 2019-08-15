// ----------------------------------------
// CS104 HW4 board and hand printer implementation
// ----------------------------------------

// STUDENTS: you must use this file as-is without modifications

#include "ConsolePrinter.h"
#include "rang.h"

#include <iostream>
#include <iomanip>
#include <cmath>

// UTF-8 Unicode characters can't be stored in single char variables, they have to be stored in strings
// However, we often need to "multiply" one of these strings by a number to repeat it multiple times,
// so let's define an operator to do that.

// Inspired by https://stackoverflow.com/questions/11843226/multiplying-a-string-by-an-int-in-c

std::string operator*(std::string const & string, std::string::size_type times)
{
	std::string sum;

	for(size_t count = 0; count < times; ++count)
	{
		sum.append(string);
	}

	return sum;
}

std::string operator*(std::string::size_type times, std::string const & string)
{
	return string * times;
}

// set colors used in the drawing
// hey, it's like prehistoric CSS!
#define FG_COLOR_LINE rang::fgB::green << rang::style::reset
#define FG_COLOR_LABEL rang::fg::reset
#define FG_COLOR_SCORE rang::fg::gray
#define FG_COLOR_LETTER rang::fg::reset << rang::style::bold
#define FG_COLOR_MULTIPLIER rang::fg::gray
#define FG_COLOR_HEADING rang::fgB::blue << rang::style::bold

#define BG_COLOR_NORMAL_SQUARE rang::bgB::black
#define BG_COLOR_START_SQUARE rang::bg::cyan
#define BG_COLOR_OUTSIDE_BOARD rang::bg::reset
#define BG_COLOR_WORD_MULTIPLIER_2X rang::bg::red
#define BG_COLOR_WORD_MULTIPLIER_3X rang::bgB::red
#define BG_COLOR_LETTER_MULTIPLIER_2X rang::bg::blue
#define BG_COLOR_LETTER_MULTIPLIER_3X rang::bgB::blue
#define BG_COLOR_PLAYER_HAND rang::bg::green

// Unicode line-drawing character constants
// from https://en.wikipedia.org/wiki/Box-drawing_character
const std::string horizLine("\u2500");
const std::string vertLine("\u2502");
const std::string topLeftCorner("\u250c");
const std::string topRightCorner("\u2510");
const std::string bottomLeftCorner("\u2514");
const std::string bottomRightCorner("\u2518");
const std::string downwardsT("\u252c");
const std::string upwardsT("\u2534");
const std::string rightFacingT("\u251c");
const std::string leftFacingT("\u2524");
const std::string cross("\u253c");

const std::string space(" ");

void ConsolePrinter::printBoard(Board const &board)
{
	const size_t horizSquares = board.getRows();
	const size_t vertSquares = board.getColumns();

	// width and height of the blank area inside a square
	const size_t squareInnerWidth = 5;
	const size_t squareInnerHeight = 3;

	// width and height of a square including its border
	const size_t squareOuterWidth = squareInnerWidth + 2;

	// size of board in characters, 1-indexed
	const size_t boardWidth = (squareOuterWidth - 1) * horizSquares + 1;

	// margins of board, must be at least 3 to allow for labels
	const size_t leftMargin = 3;
	const size_t topMargin = 3;

	// draw horizontal number labels
	for(size_t marginLine = 0; marginLine < topMargin - 2; ++marginLine)
	{
		std::cout << std::endl;
	}
	std::cout << FG_COLOR_LABEL << space * leftMargin;
	for(size_t horizSquare = 1; horizSquare <= horizSquares; ++horizSquare)
	{
		std::cout << space * 2 << std::setw(2) << horizSquare << space * 2;
	}
	std::cout << std::endl;

	// draw top line
	std::cout << space * leftMargin << FG_COLOR_LINE << BG_COLOR_NORMAL_SQUARE << topLeftCorner;
	bool horizFirstLoop = true;
	for(size_t textColumn = 1; textColumn < boardWidth - 1; textColumn += (squareOuterWidth - 1))
	{
		if(horizFirstLoop)
		{
			horizFirstLoop = false;
		}
		else
		{
			std::cout << downwardsT;
		}

		std::cout << horizLine * squareInnerWidth;
	}
	std::cout << topRightCorner << BG_COLOR_OUTSIDE_BOARD << std::endl;

	// draw inner part of board
	bool vertFirstLoop = true;

	for(size_t vertSquare = 1; vertSquare <= vertSquares; ++vertSquare)
	{

		if(vertFirstLoop)
		{
			vertFirstLoop = false;
		}
		else
		{
			// draw bottom line from previous row
			std::cout << space * leftMargin << FG_COLOR_LINE << BG_COLOR_NORMAL_SQUARE << rightFacingT;
			horizFirstLoop = true;
			for(size_t textColumn = 1; textColumn < boardWidth - 1; textColumn += (squareOuterWidth - 1))
			{
				if(horizFirstLoop)
				{
					horizFirstLoop = false;
				}
				else
				{
					std::cout << cross;
				}

				std::cout << horizLine * squareInnerWidth;
			}
			std::cout << leftFacingT << BG_COLOR_OUTSIDE_BOARD << std::endl;
		}

		// draw insides of squares
		for(size_t line = 0; line < squareInnerHeight; ++line)
		{
			std::cout << FG_COLOR_LABEL << BG_COLOR_OUTSIDE_BOARD;
			if (line == 1)
			{
				// print vertical number label
				std::cout << space * (leftMargin - 3);
				std::cout << std::setw(2) << vertSquare;
				std::cout << ' ';
			}
			else
			{
				std::cout << space * leftMargin;
			}

			for (size_t horizSquare = 1; horizSquare <= horizSquares; ++horizSquare)
			{
				std::cout << FG_COLOR_LINE << BG_COLOR_NORMAL_SQUARE << vertLine;

				Square *currentSquare = board.getSquare(horizSquare, vertSquare);

				// figure out background color
				if (currentSquare->getWMult() == 2)
				{
					std::cout << BG_COLOR_WORD_MULTIPLIER_2X;
				}
				else if (currentSquare->getWMult() == 3)
				{
					std::cout << BG_COLOR_WORD_MULTIPLIER_3X;
				}
				else if (currentSquare->getLMult() == 2)
				{
					std::cout << BG_COLOR_LETTER_MULTIPLIER_2X;
				}
				else if (currentSquare->getLMult() == 3)
				{
					std::cout << BG_COLOR_LETTER_MULTIPLIER_3X;
				}
				else if(currentSquare->isStart())
				{
					std::cout << BG_COLOR_START_SQUARE;
				}

				// figure out text
				if(currentSquare->isStart() && line == 0)
				{
					// print a star to signify the start square
					std::cout << "  \u2605  ";
				}
				else if(currentSquare->getWMult() > 1 && line == 0)
				{
					std::cout << FG_COLOR_MULTIPLIER << space * (squareInnerWidth - 2) << 'W' << std::setw(1) << currentSquare->getWMult();
				}
				else if(currentSquare->getLMult() > 1 && line == 0)
				{
					std::cout << FG_COLOR_MULTIPLIER << space * (squareInnerWidth - 2) << 'L' << std::setw(1) << currentSquare->getLMult();
				}
				else if(currentSquare->isOccupied() && line == 1)
				{
					// print letter
					std::cout << space * 2 << FG_COLOR_LETTER << (char)toupper(currentSquare->getLetter()) << space * 2;
				}
				else if(currentSquare->isOccupied() && line == squareInnerHeight - 1)
				{
					//print score in bottom right corner
					std::cout << space * (squareInnerWidth - 2) << FG_COLOR_SCORE << std::setw(2) << currentSquare->getScore();
				}
				else
				{
					// blank
					std::cout << space * squareInnerWidth;
				}
			}
			std::cout << FG_COLOR_LINE << BG_COLOR_NORMAL_SQUARE << vertLine << BG_COLOR_OUTSIDE_BOARD << std::endl;
		}
	}

	// draw bottom line
	std::cout << space * leftMargin << FG_COLOR_LINE << BG_COLOR_NORMAL_SQUARE << bottomLeftCorner;
	horizFirstLoop = true;
	for(size_t textColumn = 1; textColumn < boardWidth - 1; textColumn += (squareOuterWidth - 1))
	{
		if(horizFirstLoop)
		{
			horizFirstLoop = false;
		}
		else
		{
			std::cout << upwardsT;
		}

		std::cout << horizLine * squareInnerWidth;
	}
	std::cout << bottomRightCorner << rang::style::reset << std::endl;
}

void ConsolePrinter::printHand(Player const &player)
{
	const std::set<Tile *> tiles = player.getHandTiles();

	const size_t maxTiles = player.getMaxTiles();
	const size_t numTiles = tiles.size();

	// width and height of the blank area inside a square
	const size_t squareInnerWidth = 5;
	const size_t squareInnerHeight = 3;

	// width and height of a square including its border
	const size_t squareOuterWidth = squareInnerWidth + 2;
	const size_t squareOuterHeight = squareInnerHeight + 2;

	// number of tile-sized blank spaces to print on the right edge of the hand
	const size_t numBlankTiles = maxTiles - numTiles;

	const size_t blankTileAreaWidth = numBlankTiles * (squareOuterWidth - 1);

	const size_t topMargin = 3; // must be at least 3 to make room for title
	const size_t leftMargin = 24;

	// draw top margin
	for(size_t marginLine = 0; marginLine < topMargin - 2; ++marginLine)
	{
		std::cout << std::endl;
	}

	std::cout << space * leftMargin << FG_COLOR_HEADING << "Your Hand: " << std::endl;
	std::cout << std::endl;

	if(tiles.empty())
	{
		// print entirely emptiness
		for(size_t line = 0; line < squareOuterHeight; ++line)
		{
			std::cout << FG_COLOR_LABEL << BG_COLOR_OUTSIDE_BOARD << space * leftMargin;
			std::cout << BG_COLOR_NORMAL_SQUARE << space * (blankTileAreaWidth + 1) << BG_COLOR_OUTSIDE_BOARD << std::endl;
		}
		std::cout << rang::style::reset;
	}
	else
	{
		// draw top line
		std::cout << space * leftMargin << FG_COLOR_LINE << BG_COLOR_NORMAL_SQUARE << topLeftCorner;
		bool horizFirstLoop = true;
		for(size_t tile = 0; tile < numTiles; ++tile)
		{
			if(horizFirstLoop)
			{
				horizFirstLoop = false;
			}
			else
			{
				std::cout << downwardsT;
			}

			std::cout << horizLine * squareInnerWidth;
		}
		std::cout << topRightCorner << space * blankTileAreaWidth << BG_COLOR_OUTSIDE_BOARD << std::endl;

		// draw middle 3 lines
		for(size_t line = 0; line < squareInnerHeight; ++line)
		{
			std::cout << FG_COLOR_LABEL << BG_COLOR_OUTSIDE_BOARD << space * leftMargin;
			for (Tile * tile : tiles)
			{
				std::cout << FG_COLOR_LINE << BG_COLOR_NORMAL_SQUARE << vertLine;
				std::cout << BG_COLOR_PLAYER_HAND;

				if(line == 1)
				{
					// print letter
					std::cout << space * 2 << FG_COLOR_LETTER << (char)toupper(tile->getLetter()) << space * 2;
				}
				else if(line == squareInnerHeight - 1)
				{
					//print score in bottom right corner
					std::cout << FG_COLOR_SCORE << space * (squareInnerWidth - 2) << std::setw(2) << tile->getPoints();
				}
				else
				{
					std::cout << space * squareInnerWidth;
				}
			}
			std::cout << FG_COLOR_LINE << BG_COLOR_NORMAL_SQUARE << vertLine;
			std::cout << space * blankTileAreaWidth << BG_COLOR_OUTSIDE_BOARD << std::endl;

		}

		// draw bottom line
		std::cout << space * leftMargin << FG_COLOR_LINE << BG_COLOR_NORMAL_SQUARE << bottomLeftCorner;
		horizFirstLoop = true;
		for(size_t tile = 0; tile < numTiles; ++tile)
		{
			if(horizFirstLoop)
			{
				horizFirstLoop = false;
			}
			else
			{
				std::cout << upwardsT;
			}

			std::cout << horizLine * squareInnerWidth;
		}
		std::cout << bottomRightCorner << space * blankTileAreaWidth << rang::style::reset << std::endl;
	}


}