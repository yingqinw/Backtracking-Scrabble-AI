// ----------------------------------------
// CS104 HW4 Bag implementation
// ----------------------------------------

// STUDENTS: you must use this file as-is without modifications

#include "Bag.h"
#include "Exceptions.h"

#include <fstream>

Bag::Bag(const std::string bag_file_name, uint32_t random_seed):
_tiles(),
_letters(),
_initcount(),
_currentcount(),
_randEngine()
{
	_randEngine.seed(random_seed);

	std::ifstream bagFileStream(bag_file_name);

	if(!bagFileStream)
	{
		throw FileException("BAG");
	}

	while(!bagFileStream.eof())
	{
		char tileLetter;
		unsigned int points;
		unsigned int numTiles;

		bagFileStream >> tileLetter;
		bagFileStream >> points;
		bagFileStream >> numTiles;

		// create tile
		for(size_t tilesMade = 0; tilesMade < numTiles; ++tilesMade)
		{
			_tiles.push_back(new Tile(tileLetter, points));
		}

		// update other data members
		_letters.insert(tileLetter);
		_initcount[tileLetter] = numTiles;
		_currentcount[tileLetter] = numTiles;
	}
}

Bag::~Bag()
{
	for(std::vector<Tile*>::iterator tileIter = _tiles.begin(); tileIter != _tiles.end(); ++tileIter)
	{
		delete *tileIter;
	}
}

void Bag::addTile(Tile *tile)
{
	_tiles.push_back(tile);
	++_currentcount[tile->getLetter()];
}

void Bag::addTiles(std::vector<Tile*> tiles)
{
	for(std::vector<Tile*>::iterator tileIter = tiles.begin(); tileIter != tiles.end(); ++tileIter)
	{
		addTile(*tileIter);
	}
}

std::vector<Tile*> Bag::drawTiles(size_t number)
{
	std::vector<Tile*> tilesDrawn;

	while(!_tiles.empty() && tilesDrawn.size() < number)
	{
		// create a random distributor sized for our current tile list
		std::uniform_int_distribution<size_t> randDistributor(0, _tiles.size() - 1);

		// now move that tile from the bag to the vector
		size_t tileIndex = randDistributor(_randEngine);
		Tile * tile = _tiles[tileIndex];
		tilesDrawn.push_back(tile);
		_tiles.erase(_tiles.begin() + tileIndex);

		--_currentcount[tile->getLetter()];
	}

	return tilesDrawn;
}

size_t Bag::tilesRemaining () const
{
	return _tiles.size();
}

std::map<char,int> const & Bag::initialTileCount() const
{
	return _initcount;
}

std::map<char,int> const & Bag::currentTileCount() const
{
	return _currentcount;
}

std::set<char> const & Bag::allLetters()
{
	return _letters;
}