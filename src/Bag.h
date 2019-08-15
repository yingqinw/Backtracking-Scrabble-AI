/*
 * Bag.h
 *
 *  Created on: Sep 18, 2016
 *      Author: kempe
 */

/* The class Bag encodes a bag of tiles for the players to draw tiles from.
   Note: the letters of all tiles in the bag will have the case they do in the tileset file.*/

// Do not change this file.

#ifndef BAG_H_
#define BAG_H_

#include <vector>
#include <string>
#include <set>
#include <map>
#include <random>

#include "Tile.h"

class Bag {

public:
	/* constructor that receives the name of the file to read the bag
       from and a random seed to use for the random draws. 
	   Throws a FileException with message "BAG" if the file could not be opened.
	*/
	Bag (const std::string bag_file_name, uint32_t random_seed);


	/* The destructor for the bag will delete all of the remaining
           tiles in the bag. */
	~Bag ();
        
 	/* adds a single Tile* to the bag. */
	void addTile (Tile *tile);
       
	/* adds all of the Tile* in the vector to the bag. */
	void addTiles (std::vector<Tile*> tiles);
        
	/* returns a random set of tiles of the given number.
           If the bag does not contain enough tiles, then it
           returns all of the remaining tiles. */
	std::vector<Tile*> drawTiles (size_t number);
        
	/* returns the number of tiles remaining in the bag. */
	size_t tilesRemaining () const;
        
	/* returns a map, giving for each character (letter or '?')
	   the number of copies of that tile that were *initially* in the bag. */
	std::map<char,int> const & initialTileCount() const;

	/* returns a map, giving for each character (letter or '?')
	   the number of copies of that tile that remain in the bag. */
	std::map<char,int> const & currentTileCount() const;

	/* returns a set of all letters known to the scrabble system.
	   Will include the wildcard character, '?'   */
	std::set<char> const & allLetters();	

private:
	std::vector<Tile*> _tiles;
	std::set<char>     _letters;
	std::map<char,int> _initcount;
	std::map<char,int> _currentcount;

	// random-generation class from the C++11 Random library
	std::mt19937 _randEngine;
};


#endif /* BAG_H_ */
