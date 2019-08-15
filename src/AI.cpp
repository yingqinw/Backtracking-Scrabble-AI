#include "AI.h"
#include <sstream>

AI * AI::parseAI(std::string nameString)
{
	if(nameString.substr(0,4)=="cpus") return new ScoreAI();
	else if(nameString.substr(0,4)=="cpul") return new LengthAI();
	else return NULL;
}//parse to subclass by the name of the AI
void AI::ConstructTrie (Dictionary* dict)
{
	d=dict;
	std::set<std::string> allwords=dict->allWords();
	for(std::set<std::string>::iterator it=allwords.begin();it!=allwords.end();++it){
		trie.insert(*it);
	}
}//construct the trie for the AI

LengthAI::LengthAI():AI(){}
LengthAI::~LengthAI(){}

void LengthAI::setFirst(bool first)
{
	isfirst=first;
}//check if AI makes the first move

Move* LengthAI::getMove(const Board& board, Player & player)
{
	allmoves.clear();
	allscores.clear();
	lengths.clear();
	//clear everything
	std::string handtiles="";
	std::set<Tile*> hand= player.getHandTiles();
	for(std::set<Tile*>::iterator it= hand.begin();it!=hand.end();++it){
		handtiles+=(*it)->getLetter();
	}
	//initialize the hand of the AI 
	leftx=board.getLeftx();
	rightx=board.getRightx();
	topy=board.getTopy();
	bottomy=board.getBottomy();
	//get information from the board to get avaliable starting points

	for(unsigned int i=0;i<board.getRows();i++){
		for(unsigned int j=0;j<board.getColumns();j++){
			horizontalHelper(i+1,j+1,i+1,j+1,"","",handtiles,player,board);
			verticalHelper(i+1,j+1,i+1,j+1,"","",handtiles,player,board);
		}
	}
	//call helper functions
	if(allmoves.empty()){
		Move* m;
		m=m->parseMove("pass", player);
		std::cout<<"pass"<<std::endl;
		return m;
	}
	//if not collecting any available moves, return pass move
	int index, maxlength=-1;
	for(unsigned int i=0;i<lengths.size();i++){
		if(lengths[i]>maxlength){
			maxlength=lengths[i];
			index=i;
		}
	}//find the index of the move with largest length
	Move* m;
	m=m->parseMove(allmoves[index],player);
	std::cout<<allmoves[index]<<std::endl;
	return m;//return the placemove with largest length

}

void LengthAI::horizontalHelper(unsigned int y, unsigned int x, unsigned int starty,unsigned int startx, std::string used, std::string word, std::string remaining, Player & player, const Board & board)
{

	if(board.getSquare(startx,starty)->isOccupied()) return;

	if(board.getSquare(x,y)->isOccupied() && remaining.size()==0){
		unsigned int tempx=x;
		std::string tempword=word;
		std::vector<char> charbox;
		while(tempx<=15 && board.getSquare(tempx,y)->isOccupied()){
			charbox.push_back(board.getSquare(tempx,y)->getLetter());
			tempx++;
		}
		for(unsigned int i=0;i<charbox.size();i++){
			tempword+=charbox[i];
		}
		if(!d->isLegalWord(tempword)) return;

	}

	if(isfirst){//check if first move uses starting point
		if(starty!=board.getStarty()) return;
		else if(startx+player.getMaxTiles()<=board.getStartx()||startx>board.getStartx()) return;
	}
	else if(topy<=bottomy){
		if(startx+player.getMaxTiles()<leftx||startx>rightx+1) return;
		if(starty<topy-1||starty>bottomy+1) return;
	}
	//check if the starting points are valid

	if(word==""){
		unsigned int tempx=startx-1;
		std::vector<char> charbox;
		while(tempx>=1 && board.getSquare(tempx,starty)->isOccupied()){
			charbox.push_back(board.getSquare(tempx,starty)->getLetter());
			tempx--;
		}
		for(int i=(int)charbox.size()-1;i>=0;i--){
			word+=charbox[i];
		}
	}

	std::string _word;//new word formed
	std::string _used;//new used tiles
	std::string _remaining;//new remaining tiles

	if(!board.getSquare(x,y)->isOccupied()){//if the position on board is not occupied

		for(unsigned int i=0;i<remaining.size();i++){//backtrace through every remaining tiles

			if(remaining[i]=='?'){//if we meet a ?
				for(char letter='a';letter<='z';letter++){//bactrace through every possible char

					_used=used+'?'+letter;;
					_word=word+letter;
					_remaining=remaining.substr(0,i)+remaining.substr(i+1);
					//update the variables to pass in to recursive function

					if(trie.prefix(_word)){

						if(trie.prefix(_word)->inSet){
							//if the new word formed is a legal word

							std::stringstream ss;
							std::string sy;
							std::string sx;

							std::string moveString="place - ";
							ss<<starty;
							ss>>sy;
							ss.clear();
							moveString+=sy;
							ss<<startx;
							ss>>sx;
							moveString+=" ";
							moveString+=sx;
							moveString+=" ";
							moveString+=_used;
							//get the placemove

							Move* m;
							try{
								m=m->parseMove(moveString,player);

								PlaceMove* pm=(PlaceMove*) m;
								pm->setTile();
								pm->setfirstMove(isfirst);

								std::vector<std::pair<std::string, unsigned int>> result;
								result=board.getPlaceMoveResults(*pm);

								bool check=true;
						    	for(unsigned int k=0;k<result.size();k++){
						    		check=d->isLegalWord(result[k].first);
						    		if(!check) break;
						    	}
						    	//check if all the words formed are legal

						    	if(check){
						    		
						    		allmoves.push_back(moveString);

						    		unsigned int uselength=_used.length();
							    	for(unsigned int k=0;k<_used.length();k++){
							    		if(_used[k]=='?') uselength--;
							    	}
							    	lengths.push_back(uselength);

						    		unsigned int finalscore=0;
						    		for(unsigned int k=0;k<result.size();k++){
							    		finalscore+=result[k].second;
							    	}
							    	if(uselength==player.getMaxTiles()) finalscore+=50;
							    	allscores.push_back(finalscore);

						    	}
						    	//if all legal, push  the results to vectors
						    	delete m;

							}
							catch(FileException &e){
								delete m;
							}
						}
						if(x==board.getColumns()) continue;
						horizontalHelper(y, x+1, starty, startx, _used, _word, _remaining, player, board);

					}
				}
			}
			else{//if we don't meet a ?

				_used=used+remaining[i];
				_word=word+remaining[i];
				_remaining=remaining.substr(0,i)+remaining.substr(i+1);

				if(trie.prefix(_word)){

					if(trie.prefix(_word)->inSet){
						//if the new word formed is a legal word
						std::stringstream ss;
						std::string sy,sx;
						std::string moveString="place - ";
						ss<<starty;
						ss>>sy;
						moveString+=sy;
						ss.clear();
						ss<<startx;
						ss>>sx;
						moveString+=" ";
						moveString+=sx;
						moveString+=" ";
						moveString+=_used;
						//get the placemove

						Move* m;
						try{
							m=m->parseMove(moveString,player);
							PlaceMove* pm=(PlaceMove*) m;
							pm->setTile();
							pm->setfirstMove(isfirst);

							std::vector<std::pair<std::string, unsigned int>> result;
							result=board.getPlaceMoveResults(*pm);

							bool check=true;
					    	for(unsigned int k=0;k<result.size();k++){
					    		check=d->isLegalWord(result[k].first);
					    		if(!check) break;
					    	}
					    	//check if all the words formed are legal

					    	if(check){
					    		allmoves.push_back(moveString);

					    		unsigned int uselength=_used.length();
						    	for(unsigned int k=0;k<_used.length();k++){
						    		if(_used[k]=='?') uselength--;
						    	}
						    	lengths.push_back(uselength);

					    		unsigned int finalscore=0;
					    		for(unsigned int k=0;k<result.size();k++){
						    		finalscore+=result[k].second;
						    	}
						    	if(uselength==player.getMaxTiles()) finalscore+=50;
						    	allscores.push_back(finalscore);
					    	}//if all legal, push  the results to vectors
					    	delete m;

						}
						catch(FileException &e){
							delete m;
						}
					}
					if(x==board.getColumns()) continue;
					horizontalHelper(y, x+1, starty, startx, _used, _word, _remaining, player, board);
				}

			}
		}
	}

	else{//if the position on board is occupied

		char letter=board.getSquare(x,y)->getLetter();
		_word=word+letter;
		_used=used;
		_remaining=remaining;

		if(trie.prefix(_word)){

			if(trie.prefix(_word)->inSet){
				//if the new word formed is a legal word
				std::stringstream ss;
				std::string sy,sx;
				std::string moveString="place - ";
				ss<<starty;
				ss>>sy;
				moveString+=sy;
				ss.clear();
				ss<<startx;
				ss>>sx;
				moveString+=" ";
				moveString+=sx;
				moveString+=" ";
				moveString+=_used;
				//get the placemove

				Move* m;
				try{
					m=m->parseMove(moveString,player);
					PlaceMove* pm=(PlaceMove*) m;
					pm->setTile();
					pm->setfirstMove(isfirst);

					std::vector<std::pair<std::string, unsigned int>> result;
					result=board.getPlaceMoveResults(*pm);

					bool check=true;

			    	for(unsigned int k=0;k<result.size();k++){
			    		check=d->isLegalWord(result[k].first);
			    		if(!check) break;
			    	}//check if all the words formed are legal

			    	if(check){
			    		allmoves.push_back(moveString);

			    		unsigned int uselength=_used.length();
				    	for(unsigned int k=0;k<_used.length();k++){
				    		if(_used[k]=='?') uselength--;
				    	}
				    	lengths.push_back(uselength);

			    		unsigned int finalscore=0;
			    		for(unsigned int k=0;k<result.size();k++){
				    		finalscore+=result[k].second;
				    	}
				    	if(uselength==player.getMaxTiles()) finalscore+=50;
				    	allscores.push_back(finalscore);
			    	}//if all legal, push  the results to vectors
			    	delete m;

				}
				catch(FileException &e){
					delete m;
				}
			}
			if(x==board.getColumns()) return;
			horizontalHelper(y, x+1, starty, startx, _used, _word, _remaining, player, board);

		}
	}
}

void LengthAI::verticalHelper(unsigned int y, unsigned int x, unsigned int starty, unsigned int startx, std::string used, std::string word, std::string remaining, Player & player, const Board & board)
{
	
	if(board.getSquare(startx,starty)->isOccupied()) return;

	if(board.getSquare(x,y)->isOccupied() && remaining.size()==0){
		unsigned int tempy=y;
		std::string tempword=word;
		std::vector<char> charbox;
		while(tempy<=15 && board.getSquare(x,tempy)->isOccupied()){
			charbox.push_back(board.getSquare(x,tempy)->getLetter());
			tempy++;
		}
		for(unsigned int i=0;i<charbox.size();i++){
			tempword+=charbox[i];
		}
		if(!d->isLegalWord(tempword)) return;
	}

	if(isfirst){//check if first move uses starting point
		if(startx!=board.getStartx()) return;
		else if(starty+player.getMaxTiles()<=board.getStarty()||starty>board.getStarty()) return;
	}
	else if(topy<=bottomy){
		if(startx<leftx-1||startx>rightx+1) return;
		if(starty+player.getMaxTiles()<topy||starty>bottomy+1) return;
	}
	//check if the starting points are valid

	if(word==""){
		unsigned int tempy=starty-1;
		std::vector<char> charbox;
		while(tempy>=1 && board.getSquare(startx,tempy)->isOccupied()){
			charbox.push_back(board.getSquare(startx,tempy)->getLetter());
			tempy--;
		}
		for(int i=(int)charbox.size()-1;i>=0;i--){
			word+=charbox[i];
		}
	}

	std::string _word;//new word formed
	std::string _used;//new used tiles
	std::string _remaining;//new remaining tiles

	if(!board.getSquare(x,y)->isOccupied()){//if the position on board is not occupied

		for(unsigned int i=0;i<remaining.size();i++){//backtrace through every remaining tiles

			if(remaining[i]=='?'){//if we meet a ?

				for(char letter='a';letter<='z';letter++){//bactrace through every possible char

					_used=used+'?'+letter;;
					_word=word+letter;
					_remaining=remaining.substr(0,i)+remaining.substr(i+1);
					//update the variables to pass in to recursive function

					if(trie.prefix(_word)){
						
						if(trie.prefix(_word)->inSet){
							//if the new word formed is a legal word
							std::stringstream ss;
							std::string sy,sx;
							std::string moveString="place | ";
							ss<<starty;
							ss>>sy;
							moveString+=sy;
							ss.clear();
							ss<<startx;
							ss>>sx;
							moveString+=" ";
							moveString+=sx;
							moveString+=" ";
							moveString+=_used;
							//get the placemove

							Move* m;
							try{
								m=m->parseMove(moveString,player);
								PlaceMove* pm=(PlaceMove*) m;
								pm->setTile();
								pm->setfirstMove(isfirst);

								std::vector<std::pair<std::string, unsigned int>> result;
								result=board.getPlaceMoveResults(*pm);

								bool check=true;
						    	for(unsigned int k=0;k<result.size();k++){
						    		check=d->isLegalWord(result[k].first);
						    		if(!check) break;
						    	}//check if all the words formed are legal

						    	if(check){
						    		allmoves.push_back(moveString);

						    		unsigned int uselength=_used.length();
							    	for(unsigned int k=0;k<_used.length();k++){
							    		if(_used[k]=='?') uselength--;
							    	}
							    	lengths.push_back(uselength);

						    		unsigned int finalscore=0;
						    		for(unsigned int k=0;k<result.size();k++){
							    		finalscore+=result[k].second;
							    	}
							    	if(uselength==player.getMaxTiles()) finalscore+=50;
							    	allscores.push_back(finalscore);
						    	}//if all legal, push  the results to vectors
						    	delete m;

							}
							catch(FileException &e){
								delete m;
							}
						}
						if(y==board.getRows()) continue;
						verticalHelper(y+1, x, starty, startx, _used, _word, _remaining, player, board);

					}
				}
			}
			else{//if we don't meet a ?

				_used=used+remaining[i];
				_word=word+remaining[i];
				_remaining=remaining.substr(0,i)+remaining.substr(i+1);

				if(trie.prefix(_word)){

					if(trie.prefix(_word)->inSet){
						//if the new word formed is a legal word
						std::stringstream ss;
						std::string sy,sx;
						std::string moveString="place | ";
						ss<<starty;
						ss>>sy;
						moveString+=sy;
						ss.clear();
						ss<<startx;
						ss>>sx;
						moveString+=" ";
						moveString+=sx;
						moveString+=" ";
						moveString+=_used;
						//get the placemove

						Move* m;
						try{
							m=m->parseMove(moveString,player);
							PlaceMove* pm=(PlaceMove*) m;
							pm->setTile();
							pm->setfirstMove(isfirst);

							std::vector<std::pair<std::string, unsigned int>> result;
							result=board.getPlaceMoveResults(*pm);

							bool check=true;
					    	for(unsigned int k=0;k<result.size();k++){
					    		check=d->isLegalWord(result[k].first);
					    		if(!check) break;
					    	}//check if all the words formed are legal

					    	if(check){
					    		allmoves.push_back(moveString);

					    		unsigned int uselength=_used.length();
						    	for(unsigned int k=0;k<_used.length();k++){
						    		if(_used[k]=='?') uselength--;
						    	}
						    	lengths.push_back(uselength);

					    		unsigned int finalscore=0;
					    		for(unsigned int k=0;k<result.size();k++){
						    		finalscore+=result[k].second;
						    	}
						    	if(uselength==player.getMaxTiles()) finalscore+=50;
						    	allscores.push_back(finalscore);
					    	}//if all legal, push  the results to vectors
					    	delete m;

						}
						catch(FileException &e){
							delete m;
						}
					}
					if(y==board.getRows()) continue;
					verticalHelper(y+1, x, starty, startx, _used, _word, _remaining, player, board);

				}
			}
		}
	}

	else{//if the position on board is occupied

		char letter=board.getSquare(x,y)->getLetter();
		_word=word+letter;
		_used=used;
		_remaining=remaining;

		if(trie.prefix(_word)){

			if(trie.prefix(_word)->inSet){
				//if the new word formed is a legal word
				std::stringstream ss;
				std::string sy,sx;
				std::string moveString="place | ";
				ss<<starty;
				ss>>sy;
				moveString+=sy;
				ss.clear();
				ss<<startx;
				ss>>sx;
				moveString+=" ";
				moveString+=sx;
				moveString+=" ";
				moveString+=_used;
				//get the placemove

				Move* m;
				try{
					m=m->parseMove(moveString,player);
					PlaceMove* pm=(PlaceMove*) m;
					pm->setTile();
					pm->setfirstMove(isfirst);

					std::vector<std::pair<std::string, unsigned int>> result;
					result=board.getPlaceMoveResults(*pm);

					bool check=true;
			    	for(unsigned int k=0;k<result.size();k++){
			    		check=d->isLegalWord(result[k].first);
			    		if(!check) break;
			    	}//check if all the words formed are legal

			    	if(check){
			    		allmoves.push_back(moveString);

			    		unsigned int uselength=_used.length();
				    	for(unsigned int k=0;k<_used.length();k++){
				    		if(_used[k]=='?') uselength--;
				    	}
				    	lengths.push_back(uselength);

			    		unsigned int finalscore=0;
			    		for(unsigned int k=0;k<result.size();k++){
				    		finalscore+=result[k].second;
				    	}
				    	if(uselength==player.getMaxTiles()) finalscore+=50;
				    	allscores.push_back(finalscore);
			    	}//if all legal, push  the results to vectors
			    	delete m;

				}
				catch(FileException &e){
					delete m;
				}
			}
			if(y==board.getRows()) return;
			verticalHelper(y+1, x, starty, startx, _used, _word, _remaining, player, board);
		}
	}
}

ScoreAI::ScoreAI():AI(){}
ScoreAI::~ScoreAI(){}

void ScoreAI::setFirst(bool first)
{
	isfirst=first;
}

Move* ScoreAI::getMove(const Board& board,  Player & player)
{
	allmoves.clear();
	allscores.clear();
	lengths.clear();
	//clear everything
	std::string handtiles="";
	std::set<Tile*> hand=player.getHandTiles();
	for(std::set<Tile*>::iterator it= hand.begin();it!=hand.end();++it){
		handtiles+=(*it)->getLetter();
	}
	//initialize the hand of the AI
	leftx=board.getLeftx();
	rightx=board.getRightx();
	topy=board.getTopy();
	bottomy=board.getBottomy();
	//get information from the board to get available starting points
	for(unsigned int i=0;i<board.getRows();i++){
		for(unsigned int j=0;j<board.getColumns();j++){
			horizontalHelper(i+1,j+1,i+1,j+1,"","",handtiles,player,board);
			verticalHelper(i+1,j+1,i+1,j+1,"","",handtiles,player,board);
		}
	}
	//call helper functions
	if(allmoves.empty()){
		Move* m;
		m=m->parseMove("pass", player);
		std::cout<<"pass"<<std::endl;
		return m;
	}
	//is not collecting any available moves, return pass move
	int index, maxscore=-1;
	for(unsigned int i=0;i<allscores.size();i++){
		if(allscores[i]>maxscore){
			maxscore=allscores[i];
			index=i;
		}
	}//find the index of the move with largest score
	Move* m;
	m=m->parseMove(allmoves[index],player);
	std::cout<<allmoves[index]<<std::endl;
	return m;//return the placemove with the largest score

}

void ScoreAI::horizontalHelper(unsigned int y, unsigned int x, unsigned int starty,unsigned int startx, std::string used, std::string word, std::string remaining, Player & player, const Board & board)
{

	if(board.getSquare(startx,starty)->isOccupied()) return;

	if(board.getSquare(x,y)->isOccupied() && remaining.size()==0){
		unsigned int tempx=x;
		std::string tempword=word;
		std::vector<char> charbox;
		while(tempx<=15 && board.getSquare(tempx,y)->isOccupied()){
			charbox.push_back(board.getSquare(tempx,y)->getLetter());
			tempx++;
		}
		for(unsigned int i=0;i<charbox.size();i++){
			tempword+=charbox[i];
		}
		if(!d->isLegalWord(tempword)) return;
	}

	if(isfirst){//check if first move uses starting point
		if(starty!=board.getStarty()) return;
		else if(startx+player.getMaxTiles()<=board.getStartx()||startx>board.getStartx()) return;
	}
	else if(topy<=bottomy){
		if(startx+player.getMaxTiles()<leftx||startx>rightx+1) return;
		if(starty<topy-1||starty>bottomy+1) return;
	}//check if the starting points are valid

	if(word==""){
		unsigned int tempx=startx-1;
		std::vector<char> charbox;
		while(tempx>=1 && board.getSquare(tempx,starty)->isOccupied()){
			charbox.push_back(board.getSquare(tempx,starty)->getLetter());
			tempx--;
		}
		for(int i=(int)charbox.size()-1;i>=0;i--){
			word+=charbox[i];
		}
	}

	std::string _word;//new word formed
	std::string _used;//new used tiles
	std::string _remaining;//new remaining tiles

	if(!board.getSquare(x,y)->isOccupied()){//if the position on board is not occupied

		for(unsigned int i=0;i<remaining.size();i++){//backtrace through every remaining tiles

			if(remaining[i]=='?'){//if we meet a ?

				for(char letter='a';letter<='z';letter++){//bactrace through every possible char

					_used=used+'?'+letter;;
					_word=word+letter;
					_remaining=remaining.substr(0,i)+remaining.substr(i+1);
					//update the variables to pass in to recursive function

					if(trie.prefix(_word)){

						if(trie.prefix(_word)->inSet){
							//if the new word formed is a legal word
							std::stringstream ss;
							std::string sy;
							std::string sx;

							std::string moveString="place - ";
							ss<<starty;
							ss>>sy;
							ss.clear();
							moveString+=sy;
							ss<<startx;
							ss>>sx;
							moveString+=" ";
							moveString+=sx;
							moveString+=" ";
							moveString+=_used;
							//get the placemove

							Move* m;
							try{
								m=m->parseMove(moveString,player);
								PlaceMove* pm=(PlaceMove*) m;
								pm->setTile();
								pm->setfirstMove(isfirst);


								std::vector<std::pair<std::string, unsigned int>> result;
								result=board.getPlaceMoveResults(*pm);

								bool check=true;
						    	for(unsigned int k=0;k<result.size();k++){
						    		check=d->isLegalWord(result[k].first);
						    		if(!check) break;
						    	}//check if all the words formed are legal

						    	if(check){
						    		
						    		allmoves.push_back(moveString);

						    		unsigned int uselength=_used.length();
							    	for(unsigned int k=0;k<_used.length();k++){
							    		if(_used[k]=='?') uselength--;
							    	}
							    	lengths.push_back(uselength);

						    		unsigned int finalscore=0;
						    		for(unsigned int k=0;k<result.size();k++){
							    		finalscore+=result[k].second;
							    	}
							    	if(uselength==player.getMaxTiles()) finalscore+=50;
							    	allscores.push_back(finalscore);
						    	}//if all legal, push  the results to vectors
						    	delete m;

							}
							catch(FileException &e){
								delete m;
							}
						}
						if(x==board.getColumns()) continue;
						horizontalHelper(y, x+1, starty, startx, _used, _word, _remaining, player, board);

					}
				}
			}
			else{//if we don't meet a ?

				_used=used+remaining[i];
				_word=word+remaining[i];
				_remaining=remaining.substr(0,i)+remaining.substr(i+1);

				if(trie.prefix(_word)){

					if(trie.prefix(_word)->inSet){
						//if the new word formed is a legal word
						std::stringstream ss;
						std::string sy,sx;
						std::string moveString="place - ";
						ss<<starty;
						ss>>sy;
						moveString+=sy;
						ss.clear();
						ss<<startx;
						ss>>sx;
						moveString+=" ";
						moveString+=sx;
						moveString+=" ";
						moveString+=_used;
						//get the placemove

						Move* m;
						try{
							m=m->parseMove(moveString,player);
							PlaceMove* pm=(PlaceMove*) m;
							pm->setTile();
							pm->setfirstMove(isfirst);

							std::vector<std::pair<std::string, unsigned int>> result;
							result=board.getPlaceMoveResults(*pm);

							bool check=true;
					    	for(unsigned int k=0;k<result.size();k++){
					    		check=d->isLegalWord(result[k].first);
					    		if(!check) break;
					    	}//check if all the words formed are legal

					    	if(check){
					    		allmoves.push_back(moveString);

					    		unsigned int uselength=_used.length();
						    	for(unsigned int k=0;k<_used.length();k++){
						    		if(_used[k]=='?') uselength--;
						    	}
						    	lengths.push_back(uselength);

					    		unsigned int finalscore=0;
					    		for(unsigned int k=0;k<result.size();k++){
						    		finalscore+=result[k].second;
						    	}
						    	if(uselength==player.getMaxTiles()) finalscore+=50;
						    	allscores.push_back(finalscore);
					    	}//if all legal, push  the results to vectors
					    	delete m;

						}
						catch(FileException &e){
							delete m;
						}
					}
					if(x==board.getColumns()) continue;
					horizontalHelper(y, x+1, starty, startx, _used, _word, _remaining, player, board);
				}

			}
		}
	}
	else{//if the position on board is occupied
		char letter=board.getSquare(x,y)->getLetter();
		_word=word+letter;
		_used=used;
		_remaining=remaining;

		if(trie.prefix(_word)){

			if(trie.prefix(_word)->inSet){
				//if the new word formed is a legal word
				std::stringstream ss;
				std::string sy,sx;
				std::string moveString="place - ";
				ss<<starty;
				ss>>sy;
				moveString+=sy;
				ss.clear();
				ss<<startx;
				ss>>sx;
				moveString+=" ";
				moveString+=sx;
				moveString+=" ";
				moveString+=_used;
				//get the placemove

				Move* m;
				try{
					m=m->parseMove(moveString,player);
					PlaceMove* pm=(PlaceMove*) m;
					pm->setTile();
					pm->setfirstMove(isfirst);

					std::vector<std::pair<std::string, unsigned int>> result;
					result=board.getPlaceMoveResults(*pm);

					bool check=true;

			    	for(unsigned int k=0;k<result.size();k++){
			    		check=d->isLegalWord(result[k].first);
			    		if(!check) break;
			    	}//check if all the words formed are legal

			    	if(check){
			    		allmoves.push_back(moveString);

			    		unsigned int uselength=_used.length();
				    	for(unsigned int k=0;k<_used.length();k++){
				    		if(_used[k]=='?') uselength--;
				    	}
				    	lengths.push_back(uselength);

			    		unsigned int finalscore=0;
			    		for(unsigned int k=0;k<result.size();k++){
				    		finalscore+=result[k].second;
				    	}
				    	if(uselength==player.getMaxTiles()) finalscore+=50;
				    	allscores.push_back(finalscore);
			    	}//if all legal, push  the results to vectors
			    	delete m;

				}
				catch(FileException &e){
					delete m;
				}
			}
			if(x==board.getColumns()) return;
			horizontalHelper(y, x+1, starty, startx, _used, _word, _remaining, player, board);

		}
	}
}

void ScoreAI::verticalHelper(unsigned int y, unsigned int x, unsigned int starty, unsigned int startx, std::string used, std::string word, std::string remaining, Player & player, const Board & board)
{
	if(board.getSquare(startx,starty)->isOccupied()) return;

	if(board.getSquare(x,y)->isOccupied() && remaining.size()==0){
		unsigned int tempy=y;
		std::string tempword=word;
		std::vector<char> charbox;
		while(tempy<=15 && board.getSquare(x,tempy)->isOccupied()){
			charbox.push_back(board.getSquare(x,tempy)->getLetter());
			tempy++;
		}
		for(unsigned int i=0;i<charbox.size();i++){
			tempword+=charbox[i];
		}
		if(!d->isLegalWord(tempword)) return;
	}

	if(isfirst){//check if first move uses starting point
		if(startx!=board.getStartx()) return;
		else if(starty+player.getMaxTiles()<=board.getStarty()||starty>board.getStarty()) return;
	}
	else if(topy<=bottomy){
		if(startx<leftx-1||startx>rightx+1) return;
		if(starty+player.getMaxTiles()<topy||starty>bottomy+1) return;
	}//check if the starting points are valid

	if(word==""){
		unsigned int tempy=starty-1;
		std::vector<char> charbox;
		while(tempy>=1 && board.getSquare(startx,tempy)->isOccupied()){
			charbox.push_back(board.getSquare(startx,tempy)->getLetter());
			tempy--;
		}
		for(int i=(int)charbox.size()-1;i>=0;i--){
			word+=charbox[i];
		}
	}

	std::string _word;//new word formed
	std::string _used;//new used tiles
	std::string _remaining;//new remaining tiles

	if(!board.getSquare(x,y)->isOccupied()){//if the position on board is not occupied

		for(unsigned int i=0;i<remaining.size();i++){//backtrace through every remaining tiles

			if(remaining[i]=='?'){//if we meet a ?

				for(char letter='a';letter<='z';letter++){//bactrace through every possible char

					_used=used+'?'+letter;;
					_word=word+letter;
					_remaining=remaining.substr(0,i)+remaining.substr(i+1);
					//update the variables to pass in to recursive function

					if(trie.prefix(_word)){
						
						if(trie.prefix(_word)->inSet){
							//if the new word formed is a legal word
							std::stringstream ss;
							std::string sy,sx;
							std::string moveString="place | ";
							ss<<starty;
							ss>>sy;
							moveString+=sy;
							ss.clear();
							ss<<startx;
							ss>>sx;
							moveString+=" ";
							moveString+=sx;
							moveString+=" ";
							moveString+=_used;
							//get the placemove

							Move* m;
							try{
								m=m->parseMove(moveString,player);
								PlaceMove* pm=(PlaceMove*) m;
								pm->setTile();
								pm->setfirstMove(isfirst);

								std::vector<std::pair<std::string, unsigned int>> result;
								result=board.getPlaceMoveResults(*pm);

								bool check=true;
						    	for(unsigned int k=0;k<result.size();k++){
						    		check=d->isLegalWord(result[k].first);
						    		if(!check) break;
						    	}//check if all the words formed are legal

						    	if(check){
						    		allmoves.push_back(moveString);

						    		unsigned int uselength=_used.length();
							    	for(unsigned int k=0;k<_used.length();k++){
							    		if(_used[k]=='?') uselength--;
							    	}
							    	lengths.push_back(uselength);

						    		unsigned int finalscore=0;
						    		for(unsigned int k=0;k<result.size();k++){
							    		finalscore+=result[k].second;
							    	}
							    	if(uselength==player.getMaxTiles()) finalscore+=50;
							    	allscores.push_back(finalscore);
						    	}//if all legal, push  the results to vectors
						    	delete m;

							}
							catch(FileException &e){
								delete m;
							}
						}
						if(y==board.getRows()) continue;
						verticalHelper(y+1, x, starty, startx, _used, _word, _remaining, player, board);

					}
				}
			}
			else{//if we don't meet a ?

				_used=used+remaining[i];
				_word=word+remaining[i];
				_remaining=remaining.substr(0,i)+remaining.substr(i+1);

				if(trie.prefix(_word)){

					if(trie.prefix(_word)->inSet){
						//if the new word formed is a legal word
						std::stringstream ss;
						std::string sy,sx;
						std::string moveString="place | ";
						ss<<starty;
						ss>>sy;
						moveString+=sy;
						ss.clear();
						ss<<startx;
						ss>>sx;
						moveString+=" ";
						moveString+=sx;
						moveString+=" ";
						moveString+=_used;
						//get the placemove

						Move* m;
						try{
							m=m->parseMove(moveString,player);
							PlaceMove* pm=(PlaceMove*) m;
							pm->setTile();
							pm->setfirstMove(isfirst);

							std::vector<std::pair<std::string, unsigned int>> result;
							result=board.getPlaceMoveResults(*pm);

							bool check=true;
					    	for(unsigned int k=0;k<result.size();k++){
					    		check=d->isLegalWord(result[k].first);
					    		if(!check) break;
					    	}//check if all the words formed are legal

					    	if(check){
					    		allmoves.push_back(moveString);

					    		unsigned int uselength=_used.length();
						    	for(unsigned int k=0;k<_used.length();k++){
						    		if(_used[k]=='?') uselength--;
						    	}
						    	lengths.push_back(uselength);

					    		unsigned int finalscore=0;
					    		for(unsigned int k=0;k<result.size();k++){
						    		finalscore+=result[k].second;
						    	}
						    	if(uselength==player.getMaxTiles()) finalscore+=50;
						    	allscores.push_back(finalscore);
					    	}//if all legal, push  the results to vectors
					    	delete m;

						}
						catch(FileException &e){
							delete m;
						}
					}
					if(y==board.getRows()) continue;
					verticalHelper(y+1, x, starty, startx, _used, _word, _remaining, player, board);

				}
			}
		}
	}

	else{//if the position on board is occupied
		char letter=board.getSquare(x,y)->getLetter();
		_word=word+letter;
		_used=used;
		_remaining=remaining;

		if(trie.prefix(_word)){

			if(trie.prefix(_word)->inSet){
				//if the new word formed is a legal word
				std::stringstream ss;
				std::string sy,sx;
				std::string moveString="place | ";
				ss<<starty;
				ss>>sy;
				moveString+=sy;
				ss.clear();
				ss<<startx;
				ss>>sx;
				moveString+=" ";
				moveString+=sx;
				moveString+=" ";
				moveString+=_used;
				//get the placemove

				Move* m;
				try{
					m=m->parseMove(moveString,player);
					PlaceMove* pm=(PlaceMove*) m;
					pm->setTile();
					pm->setfirstMove(isfirst);

					std::vector<std::pair<std::string, unsigned int>> result;
					result=board.getPlaceMoveResults(*pm);

					bool check=true;
			    	for(unsigned int k=0;k<result.size();k++){
			    		check=d->isLegalWord(result[k].first);
			    		if(!check) break;
			    	}//check if all the words formed are legal

			    	if(check){
			    		allmoves.push_back(moveString);

			    		unsigned int uselength=_used.length();
				    	for(unsigned int k=0;k<_used.length();k++){
				    		if(_used[k]=='?') uselength--;
				    	}
				    	lengths.push_back(uselength);

			    		unsigned int finalscore=0;
			    		for(unsigned int k=0;k<result.size();k++){
				    		finalscore+=result[k].second;
				    	}
				    	if(uselength==player.getMaxTiles()) finalscore+=50;
				    	allscores.push_back(finalscore);
			    	}//if all legal, push  the results to vectors
			    	delete m;

				}
				catch(FileException &e){
					delete m;
				}
			}
			if(y==board.getRows()) return;
			verticalHelper(y+1, x, starty, startx, _used, _word, _remaining, player, board);
		}
	}
}