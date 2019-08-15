#include <string>
#include <cstdlib>
#include <iostream>

struct TrieNode {
  bool inSet;   // Is this node the end of a string in your dictionary?
  TrieNode *parent;
  TrieNode *children[26]; // one child for each letter of the alphabet.
  TrieNode (TrieNode *_parent){
      inSet=false;
      parent=_parent;
      for(int i=0;i<26;i++){
          children[i]=NULL;
      }
  }
};

class TrieSet {
 public:

  TrieSet (); // a constructor for an empty trie
  ~TrieSet (); // destructor
  TrieNode* getRoot(){return root;}

  void insert (std::string input);
   /* Add this string to the set.
      Do nothing if the string is already in the set. */

  void remove (std::string input);
   /* Removes this string from the set.
      Do nothing if the string is not already in the set. */

  TrieNode* prefix(std::string px); 
   /* returns the TrieNode matching the provided string, or returns NULL 
      if it is not the prefix of a word in the Trie. */
  void display(TrieNode* node, char str[], int level);//just for testing
  void display2(TrieNode* node, char str[], int level);//just for testing
protected:
	TrieNode * root;
  	void clear(TrieNode * node);
};