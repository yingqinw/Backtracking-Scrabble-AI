#include "Board.h"
#include <fstream>
#include <vector>
#include "Exceptions.h"
#include <iostream>

/* constructor that initializes the board state with an empty board
           and the size and bonuses and start square based on the given file. */
Board::Board (std::string board_file_namey){
	std::ifstream myfile(board_file_namey);
	int rows,columns,startx,starty;
	myfile>>rows>>columns>>startx>>starty;//read the information from board.cpp
	_rows=rows;
	_columns=columns;
	xstart=startx;
	ystart=starty;
	realboard=new Square**[_rows];
	for(unsigned int i=0;i<_rows;i++){
		realboard[i]=new Square*[_columns];
	}
	for(int i=0;i<rows;i++){
		for(int j=0;j<columns;j++){
			char temp;
			myfile>>temp;
			if(i==starty-1&&j==startx-1){
				if(temp=='.') realboard[i][j]=new Square(1,1, true);
				if(temp=='2') realboard[i][j]=new Square(2,1, true);
				if(temp=='3') realboard[i][j]=new Square(3,1, true);
				if(temp=='d') realboard[i][j]=new Square(1,2, true);
				if(temp=='t') realboard[i][j]=new Square(1,3, true);
			}
			else{
				if(temp=='.') realboard[i][j]=new Square(1,1, false);
				if(temp=='2') realboard[i][j]=new Square(2,1, false);
				if(temp=='3') realboard[i][j]=new Square(3,1, false);
				if(temp=='d') realboard[i][j]=new Square(1,2, false);
				if(temp=='t') realboard[i][j]=new Square(1,3, false);
			}
		}
	}//initialize the board

}

void Board::initialize (std::string init_file_name)
{
	std::ifstream myfile(init_file_name);
	for(unsigned int i=0;i<_rows;i++){
		for(unsigned int j=0;j<_columns;j++){
			char temp1,temp2,temp3;
			myfile>>temp1>>temp2>>temp3;
			if(temp1>='A' && temp1<='Z') temp1+=32;
			if(temp1>='a' && temp1<='z'){
				unsigned int score=10*((int)temp2-48)+(int)temp3-48;
				Tile* newtile=new Tile(temp1,score);
				realboard[i][j]->placeTile(newtile);
			}
		}
	}//initialize the board

}

Board::~Board (){
	for(unsigned int i=0;i<_rows;i++){
		for(unsigned int j=0;j<_columns;j++){
			delete realboard[i][j];
		}
		delete[] realboard[i];
	}
	delete[] realboard;//delete all elements in the board

}

	/* returns a vector of all words that would be formed by executing the
           given move. The first element of the pair is a string containing the word
           formed, and the second element is the score that that word earns
           (with all multipliers, but not the 50-point bonus for all letters).

       Words returned are all in uppercase.

	   The last entry of the vector is always the "main" word formed
	   in the direction chosen by the user; the others could be in arbitrary
	   order. (This is helpful for backtracking search.)

	   This function does not check that the words formed are actually in the dictionary.
	   The words returned from this function must be checked against the dictionary to
	   determine if the move is legal.	*/
std::vector<std::pair<std::string, unsigned int>> Board::getPlaceMoveResults(const PlaceMove &m) const
{
	std::vector<std::pair<std::string, unsigned int>> result;
	size_t x=m.getx();
	size_t y=m.gety();
	bool horiz=m.getHoriz();
	std::vector<Tile*> tileToTake=m.tileVector(); 
	unsigned int mainscore=0;
	std::string mainword="";
	bool adjacent=false;

	if(m.isfirstMove()){//check if first move uses starting point
		if(horiz){
			if(y!=getStarty()){
				throw FileException("Should start at the starting square!");
			}
			else{
				if(x+tileToTake.size()<=getStartx()||x>getStartx()){
					throw FileException("Should start at the starting square!");
				}
			}
		}
		else{
			if(x!=getStartx()){
				throw FileException("Should start at the starting square!");
			}
			else{
				if(y+tileToTake.size()<=getStarty()||y>getStarty()){
					throw FileException("Should start at the starting square!");
				}
			}
		}
	}

	if(realboard[y-1][x-1]->isOccupied()){
		throw FileException("Starting point occupied!");
	}

	if(horiz){//if the direction is horizontal
		if((int)x-2>=0&&realboard[y-1][x-2]->isOccupied()){
			adjacent=true;
			int k=-2;
			std::vector<char> charbox;

			while((int)x+k>=0&&realboard[y-1][x+k]->isOccupied()){
				mainscore+=realboard[y-1][x+k]->getScore();
				charbox.push_back(realboard[y-1][x+k]->getLetter());
				k--;

			}
			for(int i=(int)charbox.size()-1;i>=0;i--){
				mainword+=charbox[i];
			}
		}//check if there is already letters before the starting point
		int s=-1;
		unsigned int i;
		for(i=0;i<tileToTake.size();i++){//loop through each elements that we are going to place 

			if(x+i+s>=getColumns()){
				throw FileException("Out of Bound!");
			}//check if out of bound

			if(!realboard[y-1][x+i+s]->isOccupied()){//if the position is not occupied

				mainword+=tileToTake[i]->getUse();//add the letter and score to mainword
				mainscore+=tileToTake[i]->getPoints()*realboard[y-1][x+s+i]->getLMult ();
				
				if((int)y-2>=0&&y<getRows()&&realboard[y][x+s+i]->isOccupied()&&realboard[y-2][x+s+i]->isOccupied()){
					adjacent=true;
					std::string word="";
					unsigned int score=0;
					std::vector<char> charbox;
					int j=(int)y-2;
					while(j>=0&&realboard[j][x+s+i]->isOccupied()){
						charbox.push_back(realboard[j][x+s+i]->getLetter());
						score+=realboard[j][x+s+i]->getScore();
						j--;
					}
					for(int k=(int)charbox.size()-1;k>=0;k--){
						word+=charbox[k];
					}
					word+=tileToTake[i]->getUse();
					score+=tileToTake[i]->getPoints()*realboard[y-1][x+s+i]->getLMult();
					unsigned int f=y;
					while(f<getRows()&& realboard[f][x+s+i]->isOccupied()){
						word+=realboard[f][x+s+i]->getLetter();
						score+=realboard[f][x+s+i]->getScore();
						f++;
					}
					score*=realboard[y-1][x+s+i]->getWMult();
					std::pair<std::string, unsigned int> otherword;
					otherword.first=word;
					otherword.second=score;
					result.push_back(otherword);
				}//if there are letters both in the up side and down side of the position
				//push the words formed into the result

				else if(y<getRows()&&realboard[y][x+s+i]->isOccupied()){
					adjacent=true;
					std::string word="";
					unsigned int score=0;
					word+=tileToTake[i]->getUse();
					score+=tileToTake[i]->getPoints()*realboard[y-1][x+s+i]->getLMult();
					unsigned int j=y;
					while(j<getRows()&&realboard[j][x+s+i]->isOccupied()){
						word+=realboard[j][x+s+i]->getLetter();
						score+=realboard[j][x+s+i]->getScore();
						j++;
					}
					score*=realboard[y-1][x+s+i]->getWMult();
					std::pair<std::string, unsigned int> otherword;
					otherword.first=word;
					otherword.second=score;
					result.push_back(otherword);

				}//if there are letters in the down side of the position
				//push the words formed into the result


				else if((int)y-2>=0&&realboard[y-2][x+s+i]->isOccupied()){
					adjacent=true;
					std::string word="";
					unsigned int score=0;
					std::vector<char> charbox;
					int j=(int)y-2;
					while(j>=0&&realboard[j][x+s+i]->isOccupied()){
						charbox.push_back(realboard[j][x+s+i]->getLetter());
						score+=realboard[j][x+s+i]->getScore();
						j--;
					}
					for(int k=(int)charbox.size()-1;k>=0;k--){
						word+=charbox[k];
					}
					word+=tileToTake[i]->getUse();
					score+=tileToTake[i]->getPoints()*realboard[y-1][x+s+i]->getLMult();
					score*=realboard[y-1][x+s+i]->getWMult();
					std::pair<std::string, unsigned int> otherword;
					otherword.first=word;
					otherword.second=score;
					result.push_back(otherword);
				}//if there are letters in the up side of the position
				//push the words formed into the result
			}
			else{
				adjacent=true;
				mainword+=realboard[y-1][x+s+i]->getLetter();
				mainscore+=realboard[y-1][x+s+i]->getScore();
				s++;
				i--;
			}//if there are letters in the main direction, skip it
		}


		while(x+s+i<getColumns()&&realboard[y-1][x+s+i]->isOccupied()){
			adjacent=true;
			mainword+=realboard[y-1][x+s+i]->getLetter();
			mainscore+=realboard[y-1][x+s+i]->getScore();
			i++;
		}//if we reach the end, but the next square is occupied, keep adding the letters to the mainword
		for(int k=-1;k<s+(int)i;k++){
			mainscore*=realboard[y-1][x+k]->getWMult();
		}
		//multiply word multipliers
	}


	else{//if the direction is vertical
		if((int)y-2>=0&&realboard[y-2][x-1]->isOccupied()){
			adjacent=true;
			int k=-2;
			std::vector<char> charbox;

			while((int)y+k>=0&&realboard[y+k][x-1]->isOccupied()){
				mainscore+=realboard[y+k][x-1]->getScore();
				charbox.push_back(realboard[y+k][x-1]->getLetter());
				k--;

			}
			for(int i=(int)charbox.size()-1;i>=0;i--){
				mainword+=charbox[i];
			}
		}//check if there are letters before the starting position
		int s=-1;
		unsigned int i;
		for(i=0;i<tileToTake.size();i++){
			if(y+i+s>=getRows()){
				throw FileException("Out of Bound!");
			}//check if out of bound
			if(!realboard[y+i+s][x-1]->isOccupied()){//if the position is not occupied
				mainword+=tileToTake[i]->getUse();
				mainscore+=tileToTake[i]->getPoints()*realboard[y+i+s][x-1]->getLMult();
				//add the letter and the score to the mainword
				if((int)x-2>=0&&x<getColumns()&&realboard[y+s+i][x]->isOccupied()&&realboard[y+s+i][x-2]->isOccupied()){
					adjacent=true;
					std::string word="";
					unsigned int score=0;
					std::vector<char> charbox;
					int j=(int)x-2;
					while(j>=0&&realboard[y+s+i][j]->isOccupied()){
						charbox.push_back(realboard[y+s+i][j]->getLetter());
						score+=realboard[y+s+i][j]->getScore();
						j--;
					}
					for(int k=(int)charbox.size()-1;k>=0;k--){
						word+=charbox[k];
					}
					word+=tileToTake[i]->getUse();
					score+=tileToTake[i]->getPoints()*realboard[y+s+i][x-1]->getLMult();
					unsigned int f=x;
					while(f<getColumns()&& realboard[y+s+i][f]->isOccupied()){
						word+=realboard[y+s+i][f]->getLetter();
						score+=realboard[y+s+i][f]->getScore();
						f++;
					}
					score*=realboard[y+s+i][x-1]->getWMult();
					std::pair<std::string, unsigned int> otherword;
					otherword.first=word;
					otherword.second=score;
					result.push_back(otherword);
				}//if there are letters in the left side and the right side of the position
				//push the words formed to the result

				else if(x<getColumns() && realboard[y+s+i][x]->isOccupied()){
					adjacent=true;
					std::string word="";
					unsigned int score=0;
					word+=tileToTake[i]->getUse();
					score+=tileToTake[i]->getPoints()*realboard[y+s+i][x-1]->getLMult();
					unsigned int j=x;
					while(j<getColumns() && realboard[y+s+i][j]->isOccupied()){
						word+=realboard[y+s+i][j]->getLetter();
						score+=realboard[y+s+i][j]->getScore();
						j++;
					}
					score*=realboard[y+s+i][x-1]->getWMult();
					std::pair<std::string, unsigned int> otherword;
					otherword.first=word;
					otherword.second=score;
					result.push_back(otherword);

				}//if there are letters in the right side of the position
				//push the words formed to the result
				else if((int)x-2>=0&&realboard[y+s+i][x-2]->isOccupied()){
					adjacent=true;
					std::string word="";
					unsigned int score=0;
					std::vector<char> charbox;
					int j=(int)x-2;
					while(j>=0 && realboard[y+s+i][j]->isOccupied()){
						charbox.push_back(realboard[y+s+i][j]->getLetter());
						score+=realboard[y+s+i][j]->getScore();
						j--;
					}
					for(int k=(int)charbox.size()-1;k>=0;k--){
						word+=charbox[k];
					}
					word+=tileToTake[i]->getUse();
					score+=tileToTake[i]->getPoints()*realboard[y+i+s][x-1]->getLMult();
					score*=realboard[y+s+i][x-1]->getWMult();
					std::pair<std::string, unsigned int> otherword;
					otherword.first=word;
					otherword.second=score;
					result.push_back(otherword);
				}//if there are letters in the left side of the position
				//push the words formed to the result

			}
			else{
				adjacent=true;
				mainword+=realboard[y+i+s][x-1]->getLetter();
				mainscore+=realboard[y+s+i][x-1]->getScore();
				s++;
				i--;
			}//if there are letters in the main direction, skip it
		}
		
		while(y+s+i<getRows()&&realboard[y+s+i][x-1]->isOccupied()){
			adjacent=true;
			mainword+=realboard[y+s+i][x-1]->getLetter();
			mainscore+=realboard[y+s+i][x-1]->getScore();
			i++;
		}//if we reach the end, but the next square is occupied, keep adding the letters to the mainword

		for(int k=-1;k<s+(int)i;k++){
			mainscore*=realboard[y+k][x-1]->getWMult();
		}//multiply word multipliers
	}
	if(!adjacent && !m.isfirstMove()){
		throw FileException("Given move not adjacent!");
	}//check if adjacent

	if(mainword.length()==1){
		/*std::cout<<"The new word created:"<<std::endl;
		for(unsigned int k=0;k<result.size();k++){
			std::cout<<result[k].first<<std::endl;
		}*/
		return result;
	}//return result if the mainword is only a letter

	std::pair<std::string, unsigned int> mainwordpair;
	mainwordpair.first=mainword;
	mainwordpair.second=mainscore;
	result.push_back(mainwordpair);

	/*std::cout<<"The new word created:"<<std::endl;

	for(unsigned int k=0;k<result.size();k++){
		std::cout<<result[k].first<<std::endl;
	}*/
	return result;//add the mainword to result and return
	
}
	/* Executes the given move by taking tiles and placing them on the board.
	   This function does not check for correctness of the move, so could
	   segfault or cause other errors if called for an incorrect move.
	   When a blank tile '?' is placed on the board, it is treated as a letter,
	   i.e., the corresponding square has that letter (with score 0) placed on it.
	*/
void Board::executePlaceMove (const PlaceMove & m)
{
	size_t x=m.getx();
	size_t y=m.gety();
	bool horiz=m.getHoriz();
	std::vector<Tile*> tileToTake=m.tileVector(); 
	if(horiz){//if the direction is horizontal
		int s=-1;
		for(unsigned int i=0;i<tileToTake.size();i++){
			if(!realboard[y-1][x+i+s]->isOccupied()){
				realboard[y-1][x+i+s]->placeTile(tileToTake[i]);
			}//place tiles if positions not occupied
			else{
				s++;
				i--;
			}//skip if positions occupied
		}
	}
	else{//if the direction is vertical

		int s=-1;
		for(unsigned int i=0;i<tileToTake.size();i++){
			if(!realboard[y+i+s][x-1]->isOccupied()){
				realboard[y+i+s][x-1]->placeTile(tileToTake[i]);
			}//place tiles if positions not occupied
			else{
				s++;
				i--;
			}//skip if positions occupied
		}
	}

}

	/* Returns a pointer to the Square object representing the
       (y,x) position of the board. Indexing starts at 1 here.
	   This is needed only to display the board. */
Square * Board::getSquare (size_t x, size_t y) const
{
	return realboard[y-1][x-1];
	
}

	/* Returns the number of rows of the board.*/
size_t Board::getRows() const
{
	return _rows;
	
}

	/* Returns the number of columns of the board.*/
size_t Board::getColumns() const
{
	return _columns;
	
}

size_t Board::getStartx() const
{
	return xstart;
}

size_t Board::getStarty() const
{
	return ystart;
}

size_t Board::getLeftx() const
{
	size_t left=_columns+1;
	for(unsigned int i=0;i<_rows;i++){
		for(unsigned int j=0;j<_columns;j++){
			if(realboard[i][j]->isOccupied()){
				if(j+1<left) left=j+1;
			}
		}
	}
	return left;

}

size_t Board::getRightx() const
{
	size_t right=0;
	for(unsigned int i=0;i<_rows;i++){
		for(unsigned int j=0;j<_columns;j++){
			if(realboard[i][j]->isOccupied()){
				if(j+1>right) right=j+1;
			}
		}
	}
	return right;
	
}

size_t Board::getTopy() const
{
	size_t top=_rows+1;
	for(unsigned int i=0;i<_rows;i++){
		for(unsigned int j=0;j<_columns;j++){
			if(realboard[i][j]->isOccupied()){
				if(i+1<top) top=i+1;
			}
		}
	}
	return top;

}

size_t Board::getBottomy() const
{
	size_t bottom=0;
	for(unsigned int i=0;i<_rows;i++){
		for(unsigned int j=0;j<_columns;j++){
			if(realboard[i][j]->isOccupied()){
				if(i+1>bottom) bottom=i+1;
			}
		}
	}
	return bottom;

}