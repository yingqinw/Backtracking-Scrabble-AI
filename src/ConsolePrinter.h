/*
 * Bag.h
 *
 *  Created on: Aug 7, 2018
 *      Author: jamie
 */

/* The ConsolePrinter class handles the (somewhat complicated) task of printing the board and players' hands out to the console. */

#ifndef _CONSOLEPRINTER_H
#define _CONSOLEPRINTER_H

#include "Board.h"
#include "Player.h"

class ConsolePrinter
{
public:

	/* prints the board out to std::cout.
	   Tiles will be color-coded to show multipliers:
	   Light red = double word score, Dark red = triple word score,
	   Light blue = double word score, Dark blue = double word score.
	   Grey = normal
	  */
	static void printBoard(Board const & board);

	/* Prints the hand of the given player out. */
	static void printHand(Player const & player);
};

#endif //_CONSOLEPRINTER_H
