#include "Trie.h"

TrieSet::TrieSet ()
{
	root=new TrieNode(NULL);
}

TrieSet::~TrieSet ()
{
	clear(root);

}
void TrieSet::clear(TrieNode * node)
{
	if(!node) return;
	for(int i=0;i<26;i++){
		clear(node->children[i]);
	}
	delete node;
}

void TrieSet::insert (std::string input)
{
	TrieNode* temp=root;
	for(unsigned int i=0;i<input.length();i++){
		int index=input[i]-'a';
		if(!temp->children[index]){
			TrieNode* p=new TrieNode(temp);
			temp->children[index]=p;
		}
		temp=temp->children[index];
	}
	temp->inSet=true;
}
   /* Add this string to the set.
      Do nothing if the string is already in the set. */

void TrieSet::remove (std::string input)
{
	TrieNode* temp=root;
	for(unsigned int i=0;i<input.length();i++){
		int index=input[i]-'a';
		if(!temp->children[index]){
			return;
		}
		temp=temp->children[index];
	}
	temp->inSet=false;
	bool nochildren=true;
	for(int i=0;i<26;i++){
		if(temp->children[i]){
			nochildren=false;
		}
	}

	while(!temp->inSet && temp->parent && nochildren){
		TrieNode* p=temp;
		temp=temp->parent;
		for(int i=0;i<26;i++){
			if(temp->children[i]==p){
				temp->children[i]=NULL;
				break;
			}
		}
		delete p;
		nochildren=true;
		for(int i=0;i<26;i++){
			if(temp->children[i]){
				nochildren=false;
			}
		}
	}
}
   /* Removes this string from the set.
      Do nothing if the string is not already in the set. */

TrieNode* TrieSet::prefix(std::string px)
{
	TrieNode* temp=root;
	for(unsigned int i=0;i<px.length();i++){
		int index=px[i]-'a';
		if(!temp->children[index]){
			return NULL;
		}
		temp=temp->children[index];
	}
	return temp;

}
   /* returns the TrieNode matching the provided string, or returns NULL 
      if it is not the prefix of a word in the Trie. */

void TrieSet::display(TrieNode* node, char str[], int level) 
{ 
    // If node is leaf node, it indiicates end 
    // of string, so a null charcter is added 
    // and string is displayed 

    if (node->inSet)  
    { 
        str[level] = '\0'; 
        std::cout << str << std::endl; 
    } 
  
    int i; 
    for (i = 0; i < 26; i++)  
    { 
        // if NON NULL child is found 
        // add parent key to str and 
        // call the display function recursively 
        // for child node 
        if (node->children[i])  
        { 
            str[level] = i + 'a'; 
            display(node->children[i], str, level + 1); 
        } 
    } 
} 

void TrieSet::display2(TrieNode* node, char str[], int level) 
{ 
    // If node is leaf node, it indiicates end 
    // of string, so a null charcter is added 
    // and string is displayed 

    bool nochildren=true;
    for(int i=0;i<26;i++){
    	if(node->children[i]){
    		nochildren=false;
    	}
    }
    if (nochildren)  
    { 
        str[level] = '\0'; 
        std::cout << str << std::endl; 
    } 
  
    int i; 
    for (i = 0; i < 26; i++)  
    { 
        // if NON NULL child is found 
        // add parent key to str and 
        // call the display function recursively 
        // for child node 
        if (node->children[i])  
        { 
            str[level] = i + 'a'; 
            display2(node->children[i], str, level + 1); 
        } 
    } 
} 