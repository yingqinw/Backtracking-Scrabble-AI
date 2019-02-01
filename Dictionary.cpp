#include "Dictionary.h"
#include <fstream>

	/* The constructor gets passed the name of the file from which
	   to read the word list. */
Dictionary::Dictionary (std::string dictionary_file_name)
{
	std::ifstream myfile(dictionary_file_name);//open the dictionary file
	std::string myword;
	while(getline(myfile,myword)){
		words.insert(myword);//insert each word into the set
	}
}

Dictionary::~Dictionary (){
	words.clear();
}

std::set<std::string> Dictionary::allWords() const
{
	return words;
}

		/* Checks whether the given word is in the dictionary, and returns true if so.
	   Case should not matter. Must run in O(logn) for the number of words in the dictionary. */
bool Dictionary::isLegalWord (std::string const & word) const
{
	std::set<std::string>::iterator sit;
	sit=words.find(word);//check if the word is found
	return(sit!=words.end());

}