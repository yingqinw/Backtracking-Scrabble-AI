/*
 * Square.h
 *
 *  Created on: Sep 18, 2016
 *      Author: kempe
 */

/* This class encodes a single square of the Scrabble board.
   It stores the square's multipliers and the tile placed on it.
   It functions almost as a struct, and does not do interesting computation. */

// Do not change this file.

#ifndef SQUARE_H_
#define SQUARE_H_

#include "Tile.h"

class Square
{

protected:
	unsigned int _LMult, _WMult;
	bool _occupied;
	bool _start;
	Tile * _tile;

public:

	/* Simple constructor, taking the multipliers and whether this is a start square. */
	Square(unsigned int LMult, unsigned int WMult, bool start):
	_LMult(LMult),
	_WMult(WMult),
	_occupied(false),
	_start(start),
	_tile(nullptr)
	{
	}

	~Square()
	{
		delete _tile;
	}

	/* Places a tile on the square.
	   This marks the square as occupied, and turns the multipliers
	   to 1, since they cannot be reused later. */
	void placeTile(Tile * tile)
	{
		_occupied = true;
		_tile = tile;
		_LMult = 1;
		_WMult = 1;
	}

	/* Returns whether a tile is placed on this square. */
	bool isOccupied () const
	{ return _occupied; }

	/* Returns whether the square is the start square. */
	bool isStart () const
	{ return _start; }

	/* Gets the base score of the tile placed on the square (if any).
	   Does not factor in any letter or word multipliers.
	   The score of an empty square is 0. */
	unsigned int getScore () const
	{
		if(_tile == nullptr)
		{
			return 0;
		}
		else
		{
			return _tile->getPoints();
		}
	}

	/* Returns the letter placed on the square (if any).

	   If the square contains a ? tiles, returns the
	   character that the tile is used as (if any).

	   If the square is empty, it returns ' '. */
	char getLetter () const
	{
		if(_tile == nullptr)
		{
			return ' ';
		}
		else
		{
			return _tile->getUse();
		}
	}

	/* Returns the letter multiplier associated with the square.
   	If there is no letter multiplier, it returns 1. */
	unsigned int getLMult () const
	{ return _LMult; }

	/* Returns the word multiplier associated with the square.
	     If there is no word multiplier, it returns 1. */
	unsigned int getWMult () const
	{ return _WMult; }

};



#endif /* SQUARE_H_ */
