#include "Player.h"
/* Constructor giving the player the given name, and setting their points to 0.
	   Does not give the player any tiles.
	*/
Player::Player (std::string const & name, size_t maxTiles)
{
	_name=name;
	score=0;
	maxsize=maxTiles;
	AI=false;

}

	/* Destructor for a player. Deletes all the tiles the player still has. */
Player::~Player ()
{
	for(std::set<Tile*>::iterator it = hand.begin();it!=hand.end();++it){
		delete *it;
	}
	
}

	/* Returns the set of tiles the player currently holds. */
std::set<Tile*> Player::getHandTiles() const
{
	return hand;
	
}

	/* Reads a move string and confirms that the player has the tiles necessary to
	   execute this move.

	   The move string is case-insensitive.

	   The boolean resolveBlanks states whether this is a PLACE or EXCHANGE move.
	   If resolveBlanks is true, then when the string contains a '?',
	   the next letter is interpreted as the letter to use the blank as.

	   By definition, if this function returns true, then takeTiles() would
	   succeed.
	 */

bool Player::hasTiles(std::string const & move, bool resolveBlanks) const
{
	std::set<Tile*> copy;//make a copy of the hand
	for(std::set<Tile*>::iterator sit=hand.begin(); sit!=hand.end();++sit){
		copy.insert(*sit);
	}
	if(!resolveBlanks){//if we don't need to resolve blank
		for(unsigned int i=0;i<move.length();i++){
			bool found=false;
			for(std::set<Tile*>::iterator sit=copy.begin(); sit!=copy.end();++sit){
				if((*sit)->getLetter()==move[i]){
					found=true;
					copy.erase(*sit);
					break;//check if each letter in the string is found
				}
			}
			if(!found) return false;
		}
	}
	else{
		for(unsigned int i=0;i<move.length();i++){		
			bool found=false;
			std::set<Tile*>::iterator sit;
			for(sit=copy.begin(); sit!=copy.end();++sit){
				if((*sit)->getLetter()==move[i]){
					found=true;//check if each letter in the string is found
					break;
				}
			}
			if(!found) return false;
			if(move[i]=='?'){//if the letter is ? then skip the next character  
				copy.erase(*sit);
				i++;
			}
			else{
				copy.erase(*sit);
			}
		}
	}
	return true;
	
}


	/* Reads a move string, finds the corresponding tiles in the player's hand, and
	   removes and returns those tiles in the order they appear in the move string.

	   The move string is case-insensitive.

	   The boolean resolveBlanks states whether this is a PLACE or EXCHANGE move.
	   If resolveBlanks is true, then when the string contains a '?',
	   the next letter is interpreted as the letter to use the blank as,
	   and the "use" field of the tile is set accordingly.

	   The move string is assumed to have correct syntax.
	*/
std::vector<Tile*> Player::takeTiles (std::string const & move, bool resolveBlanks)
{
	std::vector<Tile*> tilesToTake;//create a vector of tiles
	if(!resolveBlanks){
		for(unsigned int i=0;i<move.length();i++){
			std::set<Tile*>::iterator sit;
			for(sit=hand.begin(); sit!=hand.end();++sit){
				if((*sit)->getLetter()==move[i]){
					break;
				}
			}
			tilesToTake.push_back(*sit);//found out the tiles corresponded to the letter in the string 
			//and push to the vector
			hand.erase(*sit);//remove the letter from hand
		}
	}
	else{
		for(unsigned int i=0;i<move.length();i++){		
			std::set<Tile*>::iterator sit;
			for(sit=hand.begin(); sit!=hand.end();++sit){
				if((*sit)->getLetter()==move[i]){
					break;
				}//found out the tiles corresponded to the letter in the string
			}
			if(move[i]=='?'){//if the letter is ?
				(*sit)->useAs(move[i+1]);//set the use of the tile
				tilesToTake.push_back(*sit);//and push to the vector
				hand.erase(*sit);//and remove it from hand
				i++;//and skip the next character
			}
			else{
				tilesToTake.push_back(*sit);//push the tile to vector
				hand.erase(*sit);//remove it from hand
			}
		}
	}
	return tilesToTake;

	
}

	// Adds all the tiles in the vector to the player's hand.
void Player::addTiles (std::vector<Tile*> const & tilesToAdd)
{
	for(unsigned int i=0;i<tilesToAdd.size();i++){
		hand.insert(tilesToAdd[i]);
	}
	
}

int Player::getScore() const
{
	return score;
}

void Player::setScore(int newscore)
{
	score=newscore;
}

const size_t Player::getMaxTiles() const
{
	return maxsize;
}

std::string Player::getName() const
{
	return _name;
}

const bool Player::isAI() const
{
	return AI;
}

void Player::setAI()
{
	AI=true;
}


	//Add more public/protected/private functions/variables here.