#include "Player.h"
#include "Move.h"
#include "Board.h"
#include "Dictionary.h"
#include "ConsolePrinter.h"
#include <set>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>
#include "Exceptions.h"
#include "AI.h"


int main(int argc, char*argv[]){
	if(argc < 2){//check if the number of command is correct
    	std::cerr << "Please provide the name of configuration file." << std::endl;
    	return 1;
    }
    std::ifstream ifile(argv[1]);//open the file
    if(ifile.fail()){//check if the file can be successfully open
    	std::cerr<<"Couldn't open the file"<<std::endl;
    	return 1;
    }
    std::string myline;
    size_t handsize;
    int seed;
    int num_players;
    std::string board_name,dic_name,bag_name, init_name;
    bool init=false;
    while(getline(ifile,myline)){
    	std::stringstream ss(myline);
    	std::string type;
    	ss>>type;
    	if(type=="HANDSIZE:"){		
    		ss>>handsize;
    	}
    	if(type=="TILES:"){
    		ss>>bag_name;
    	}
    	if(type=="DICTIONARY:"){
    		ss>>dic_name;
    	}
    	if(type=="BOARD:"){
    		ss>>board_name;    		
    	}
    	if(type=="SEED:"){
    		ss>>seed;	
    	}
        if(type=="INIT:"){
            ss>>init_name;
            init=true;
        }
    }//read the information from the configuration file
    Board* board=new Board(board_name);
    if(init) board->initialize(init_name);
    Dictionary* dictionary =new Dictionary(dic_name);
    Bag* bag= new Bag(bag_name,seed);
    std::vector<int> AIindexs;
    std::vector<AI*> AIlist;
    //initialize different class
    std::cout<<"Please enter the number of players"<<std::endl;
    std::cin>>num_players;
    std::cin.ignore();
    while(num_players<1||num_players>8){
    	std::cout<<"Invalid number of players, please re-enter"<<std::endl;
    	std::cin>>num_players;
    	std::cin.ignore();
    }
    std::vector<Player*> Playerlist;
    for(int i=0;i<num_players;i++){
    	std::string name;
    	std::cout<<"Please enter the name of the player"<<std::endl;
    	std::getline(std::cin,name);
        std::string temp=name;
        Player* p=new Player(name, handsize);
        Playerlist.push_back(p);

        transform(temp.begin(),temp.end(),temp.begin(),::tolower);
        AI* ai;
        ai=ai->parseAI(temp);
        if(ai){
            ai->ConstructTrie (dictionary);
            AIindexs.push_back(i);
            AIlist.push_back(ai);
            p->setAI();
        }
    }//enter the information of players

    for(int i=0;i<num_players;i++){
    	std::vector<Tile*> drawtiles=bag->drawTiles(handsize);
    	Playerlist[i]->addTiles(drawtiles);
    }//initialize each player's hands
    
    std::cout<< "---------------------------------" << std::endl;
    std::cout<<"The game starts now!"<<std::endl;
    std::cout<< "---------------------------------" << std::endl;

    bool end=false;
    bool isfirst=!init;
    int pass=0;

    while(!end){
    	for(int i=0;i<num_players;i++){
    		
    		ConsolePrinter::printBoard(*board);
    		ConsolePrinter::printHand(*Playerlist[i]);
    		std::cout<<"The Scores of all players:"<<std::endl;
    		for(int j=0;j<num_players;j++){
    			std::cout<<"Player "<<Playerlist[j]->getName()<<": "<<Playerlist[j]->getScore()<<std::endl;
    		}//print score board
    		bool success=false;
    		Move* m;
    		while(!success){
    			std::cout<<"It's Player "<<Playerlist[i]->getName()<<"'s turn to move, Please enter your move:"<<std::endl;
        		try{
                    if(Playerlist[i]->isAI()){
                        unsigned int index;
                        for(index=0;index<AIindexs.size();index++){
                            if(AIindexs[index]==i) break;
                        }
                        AI* ai=AIlist[index];
                        ai->setFirst(isfirst);
                        m=ai->getMove(*board,*Playerlist[i]);
                    }
                    else{
                        std::string movestring;//read the player's move
            			std::getline(std::cin,movestring);
            			transform(movestring.begin(),movestring.end(),movestring.begin(),::tolower);
            			Move* tempe;
            			m=tempe->parseMove(movestring, *Playerlist[i]);//initialize the move class
                    }
        			if(m->isWord()){
        				PlaceMove* pm=(PlaceMove*) m;
        				pm->setfirstMove(isfirst);
        			}
    				m->execute(*board, *bag, *dictionary);//try to execute the move
    				success=true;
    			}
    			catch(FileException &e){//catch error
    				std::cout<<"Error: "<<e.getMessage()<<std::endl;
    				success=false;
    				if(e.getMessage()!="Unknown Move Command!") delete m;
    			}
    		}
    		if(m->isPass()){//check the number of pass
    			pass++;
    		}
    		else{
    			pass=0;
    		}
    		//check if the game ends
    		if(m->isWord() && isfirst) isfirst=false;
    		for(int k=0;k<num_players;k++){
    			if(Playerlist[k]->getHandTiles().empty()){
    				end=true;
    			}
    		}

    		if(pass==num_players){
    			end=true;
    		}
    		delete m;
    		if(end) break;
    		std::cout<<"Press [enter] to continue"<<std::endl;
    		std::cin.ignore();
    	}
    }
    std::cout<< "---------------------------------" << std::endl;
    std::cout<<"The game ends!"<<std::endl;
    std::cout << "---------------------------------" << std::endl;
    unsigned int allhandtilescore=0;
    for(int j=0;j<num_players;j++){
    	unsigned int handtilescore=0;
    	std::set<Tile*> handtiles=Playerlist[j]->getHandTiles();
    	for(std::set<Tile*>::iterator sit=handtiles.begin();sit!=handtiles.end();++sit){
    		handtilescore+=(*sit)->getPoints();
    	}
    	allhandtilescore+=handtilescore;
    	Playerlist[j]->setScore(Playerlist[j]->getScore()-handtilescore);
        if(Playerlist[j]->getScore()<0) Playerlist[j]->setScore(0);  
    }//calculate each player's final score

    for(int k=0;k<num_players;k++){
    	if(Playerlist[k]->getHandTiles().empty()){
    		std::cout<<"Player "<<Playerlist[k]->getName()<<" use all his hands!"<<std::endl;
    		Playerlist[k]->setScore(Playerlist[k]->getScore()+allhandtilescore);
    	}
    }//add others' score to one player if he uses all his hand

    std::cout<<"The Scores of all players:"<<std::endl;
    for(int j=0;j<num_players;j++){
    	std::cout<<"Player "<<Playerlist[j]->getName()<<": "<<Playerlist[j]->getScore()<<std::endl;
    }//print score of players

    for(unsigned int i=num_players-1; i>0;i--){
    	for(unsigned int j=0;j<i;j++){
    		if(Playerlist[j]->getScore()>Playerlist[j+1]->getScore()){
    			Player* temp=Playerlist[j];
    			Playerlist[j]=Playerlist[j+1];
    			Playerlist[j+1]=temp;
    		}
    	}
    }//sort the playerlist
   int best=Playerlist[num_players-1]->getScore();
    for(int i=num_players-1;i>=0;i--){
    	if(Playerlist[i]->getScore()==best){
    		std::cout<<"The Winner is "<<Playerlist[i]->getName()<<"! Congratulations!"<<std::endl;
            std::cout << "---------------------------------" << std::endl;
    	}
    	else break;
    }//check the winner
    for(unsigned int i=0;i<Playerlist.size();i++){
        delete Playerlist[i];
    }
    for(unsigned int i=0;i<AIlist.size();i++){
        delete AIlist[i];
    }
    delete board;
    delete dictionary;
    delete bag;
    return 0;
}



