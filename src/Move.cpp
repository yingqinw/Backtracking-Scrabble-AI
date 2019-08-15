#include "Move.h"
#include <sstream>
#include "Exceptions.h"
#include "Board.h"
#include "Dictionary.h"
#include <iostream>
#include "ConsolePrinter.h"

	/* Parses the given move m, and constructs the corresponding move subclass.
	   m is assumed to be in the format described on the assignment sheet for a move command.
	   The player passed in is used to verify that the proposed tiles are
	   in fact in the player's hand.
	   It can handle all three types of move (PASS, EXCHANGE, PLACE).
	   Coordinates start with 1.
	*/

Move * Move::parseMove(std::string moveString, Player &p)
{
	
	std::string Type;
	std::stringstream move(moveString);//push the movestring into sstream
	move>>Type;//input the type of the move
	if(Type=="pass"){
		PassMove* pm=new PassMove(&p);
		return pm;//create a new passmove
	}
	else if(Type=="exchange"){
		std::string exchange;
		move>>exchange;
		ExchangeMove* em=new ExchangeMove(exchange, &p);
		return em;//create a new exchange move
	}
	else if(Type=="place"){
		char dir; 
		move>>dir;
		bool horiz=false;
		if(dir=='-') horiz=true;
		size_t x;
		size_t y;
		move>>y>>x;
		std::string place;
		move>>place;
		PlaceMove* plm=new PlaceMove(x, y, horiz, place, &p);
		return plm;//create a new placemove
	}
	else {//if the command is unkown
		throw FileException("Unknown Move Command!");
	}
	
}

Move::~Move(){}

Move::Move(Player * player)
{
	_player=player;
}


PassMove::PassMove(Player * player):Move(player)
{
	_player=player;
}
/* Executes this move, whichever type it is.
	   This may throw exceptions; students: it's up to you to
	   decide (and document) what exactly it throws*/
void PassMove::execute(Board & board, Bag & bag, Dictionary & dictionary)
{
	return;

}

/* Creates an EXCHANGE move, exchanging the tiles listed in the
	   string (formatted according to the EXCHANGE command description)
	   with new tiles from the bag.
	   */
ExchangeMove::ExchangeMove(std::string tileString, Player * p):Move(p)
{
	_player=p;
	_tileString=tileString;


}

	/* Executes this move, whichever type it is.
	   This may throw exceptions; students: it's up to you to
	   decide (and document) what exactly it throws*/
void ExchangeMove::execute(Board & board, Bag & bag, Dictionary & dictionary)
{
	bool blank=false;//no need to resolve blank
	if(!_player->hasTiles(_tileString,blank)){
		throw FileException("Player doesn't have these tiles");
	}//throw exceptions if the player doesn't have tiles
	else{//if player has tiles
		std::vector<Tile*> tilesToTake=_player->takeTiles(_tileString,blank);//take the tiles
		bag.addTiles (tilesToTake);//add it to the bag
		std::vector<Tile*> tilesToDraw= bag.drawTiles (tilesToTake.size());//draw tiles from bag
		_player->addTiles(tilesToDraw);//and add to player's hand
		std::cout<<"Your new hand is:"<<std::endl;
    	ConsolePrinter::printHand(*_player);//print out player's new hand
	}


}
/* Creates a PLACE move, starting at row y, column x, placing the tiles
	   described by the string tileString. If "horizontal" is true, then the tiles
	   are placed horizontally, otherwise vertically.
	   Coordinates start with 1.
	   The string m is in the format described in HW4; in particular, a '?'
	   must be followed by the letter it is to be used as.
	*/
PlaceMove::PlaceMove (size_t x, size_t y, bool horizontal, std::string tileString, Player * p):Move(p)
{
	_player=p;
	_tileString=tileString;
	_x=x;
	_y=y;
	_horizontal=horizontal;
	first_move=false;
}

	/* Returns the vector of tiles associated with a PLACE/EXCHANGE move.
	   Return value could be arbitrary for PASS moves. */
std::vector<Tile*> const & PlaceMove::tileVector () const
{
	return tilesToTake;

}

//set the tilevector for other classes to use
void PlaceMove::setTile()
{
	tilesToTake=_player->takeTiles(_tileString,true);
	_player->addTiles(tilesToTake);

}

	/* Executes this move, whichever type it is.
	   This may throw exceptions; students: it's up to you to
	   decide (and document) what exactly it throws*/

void PlaceMove::execute(Board & board, Bag & bag, Dictionary & dictionary)
{
	if(_x<=0||_x>board.getColumns()||_y<=0||_y>board.getRows()){
		throw FileException("Invalid starting point!");
	}//check valid starting point
	if(!_player->hasTiles(_tileString,true)){
		throw FileException("Player doesn't have these tiles");
	}//check if player has tiles
	else{
		setTile();

		std::vector<std::pair<std::string, unsigned int>> result;
		try{
    		result=board.getPlaceMoveResults(*this);
    	}
    	catch(FileException &e){//catch the error thrown in board.cpp
    		throw FileException(e.getMessage());//throw the error to main
    	}
    	bool check=true;
    	for(unsigned int k=0;k<result.size();k++){
    		check=dictionary.isLegalWord(result[k].first);
    		if(!check) break;
    	}//check if the word is legal
    	if(!check){
    		throw FileException("Created illegal words");
    	}//throw error if the words are illegal

    	_player->takeTiles(_tileString,true);
    	//take tiles from player and push to vector
		board.executePlaceMove (*this);//execute place move
		std::vector<Tile*> tilesToDraw= bag.drawTiles (tilesToTake.size());
		_player->addTiles(tilesToDraw);//draw tiles from bag and add to the player's hand
		ConsolePrinter::printBoard(board);
		std::cout<<"You created these new words, together with their points are:"<<std::endl;
    	for(unsigned int k=0;k<result.size();k++){
    		std::cout<<result[k].first<<" "<<result[k].second<<std::endl;
    		_player->setScore(_player->getScore()+result[k].second);
    	}//print out the new words formed and their scores
    	if(tilesToTake.size()==_player->getMaxTiles()){
    		std::cout<<"You used all tiles in your hand! Add 50 bonus points!"<<std::endl;
    		_player->setScore(_player->getScore()+50);
    	}//check if a player uses all his hand
    	std::cout<<"Now your new score is "<<_player->getScore()<<"!"<<std::endl;//print new score
    	std::cout<<"Your new hand is: "<<std::endl;
    	ConsolePrinter::printHand(*_player);//print new hand
	}

}

bool PlaceMove::isfirstMove() const
{
	return first_move;
}

void PlaceMove::setfirstMove(bool isfirst){
	first_move=isfirst;

}

size_t PlaceMove::getx() const{ return _x; }
size_t PlaceMove::gety() const { return _y; }
bool PlaceMove::getHoriz() const{ return _horizontal;}
