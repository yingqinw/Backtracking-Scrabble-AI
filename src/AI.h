#include "Move.h"
#include "Dictionary.h"
#include "Board.h"
#include "Player.h"
#include "Trie.h"
#include <string>
#include "Exceptions.h"

class AI {
public:
	AI(){};
	AI * parseAI(std::string nameString);
	virtual~AI(){}
	void ConstructTrie (Dictionary* dict);
	//constructing trie for fast loopup
	virtual Move* getMove(const Board& board, Player & player)=0;
	virtual void setFirst(bool first)=0;
protected:
	TrieSet trie;
	Dictionary* d;
};

class LengthAI: public AI
{
public:
	LengthAI();
	~LengthAI();
	Move* getMove(const Board& board, Player & player);
	void setFirst(bool first);//check if AI is making the first move
	void horizontalHelper(unsigned int y, unsigned int x, unsigned int starty,unsigned int startx, std::string used, std::string word, std::string remaining, Player & player, const Board & board);
	void verticalHelper(unsigned int y, unsigned int x, unsigned int starty,unsigned int startx, std::string used, std::string word, std::string remaining, Player & player, const Board & board);
private:
	std::vector<std::string> allmoves;
	std::vector<int> allscores;
	std::vector<int> lengths;
	bool isfirst;
	size_t leftx;
	size_t rightx;
	size_t topy;
	size_t bottomy;
};

class ScoreAI: public AI
{
public:
	ScoreAI();
	~ScoreAI();
	Move* getMove(const Board& board, Player & player);
	void setFirst(bool first);//check if AI is making the first move
	void horizontalHelper(unsigned int y, unsigned int x, unsigned int starty,unsigned int startx, std::string used, std::string word, std::string remaining, Player & player, const Board & board);
	void verticalHelper(unsigned int y, unsigned int x, unsigned int starty,unsigned int startx, std::string used, std::string word, std::string remaining, Player & player, const Board & board);
private:
	std::vector<std::string> allmoves;
	std::vector<int> allscores;
	std::vector<int> lengths;
	bool isfirst;
	size_t leftx;
	size_t rightx;
	size_t topy;
	size_t bottomy;
};