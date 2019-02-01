/*
 * Tile.h
 *
 *  Created on: Sep 18, 2016
 *      Author: kempe
 */

/* Encodes a single tile. Basically acts like a struct, 
   without interesting computation. */

// Do not change this file.

#ifndef TILE_H_
#define TILE_H_

class Tile {
public:
	Tile (char letter, unsigned int points)
	  /* Constructor, initializing the letter and points.
	     The use may eventually differ from the letter for blanks tiles only.
	  */
	{ _letter = letter; _points = points; _use = letter; }

	/* Returns the letter on the tile ('?' for blanks). */
	char getLetter () const { return _letter; }
	
	/* Returns the points for the tile. */
	unsigned int getPoints () const { return _points; }

	/* Returns whether the tile is blank tile. */
	bool isBlank () const { return (_letter == '?'); }

	/* Returns what the tile is used as.
	   For letter tiles, this is just the letter.
	   For blank tiles, it can be set by the player when placing the tile. */
	char getUse () const { return _use; }

	/* Sets the use of the tile to the given letter. */
	void useAs (char use) { _use = use; }

protected:
	char _letter;
	char _use;
	unsigned int _points;
};


#endif /* TILE_H_ */
