# Backtracking Scrabble AI

**Basic Information**

ScrabbleAI combines the classic game of scrabble with intelligent decision-making computer players. The game is configured to play on a standard 15 x 15 scrabble board with a standard English Dictionary. AI decision are made using recursive backtracking and a Trie of all legal words (for efficiency purposes). After each valid player move, the updated board and score are printed to the window, and the next human player is prompted to play. If no human players are used, AI's will continuously move until the game is concluded. On each turn, players can either pass, exchange, or place.

**End Game Conditions**

    (1) Consecitve choices to pass from every player in game
    (2) Bag is empty and a single player runs out of tiles to play (both must be met)

**Structure for Moves in Command Line:**

(1) Pass

    enter: pass or PASS (not case sensitive)
    ex: pass

(2) Exchange

    enter: exchange [all tiles to exchange] 
    ex: exchange CKTR
    
(3) Place 

    enter: place [directon of move] [starting vertical coordinate] [starting horizontal coordinate] [tiles to place down]
    
            direction: 
                '|' --> vertical
                '-' for horizontal
            starting coordinates:
                must be integers in bounds (1 - 15)
    ex: place - 8 8 dog (word dog placed )
    ex: place | 1 5 cat
     
    Notes: 
        (1) word intersecting with letters already on board should not include those letters
        
            ex: place - 8 8 dg
                (places letters 'd' and 'g' from hand, forms word with 'o' already on board at [8,9])
                
        (2) Using a wild card tile --> '?'
        
            - type ? in place where it will be used for letter that player doesn't have followed by letter it is to used as 

            ex: place - 8 8 d?og
                (places word dog down, takes 'd','?', and 'g' from player hand and uses the ? as 'o')
**To Run Game:**

	(1) download ScrabbleAI folder from github
	(2) in command line, navigate to downloaded folder
    ** please max command line window to entire screen (important for game visualization tool used) **
	(3) run the following commands:
        - make or make scrabble
        - ./scrabble config.txt
	(4) see below for initialization of game

**Initializing the Game:**

	# of Players:
	-------------
	(1) When prompted for # of players, make sure to enter only an integer.  This is very important.
    
	AI Choices:
	-----------
	When prompted with enter a player name, you have three options.
		(1) human player --> enter any string for a name that does not start with "CPUS" or "CPUL" 
        
			- user will have to manually input each play they want to make when turn arrives
			- if an invalid play is made, the user prompted to play again with reason for invalid play
            
		(2) Max Letters AI Player --> enter "CPUL" followed by any other characters and spaces 
        
			- this player will play the word that maximizes the number of letters used from their hand
			- if no valid move is found, the player will default to a Pass for that turn
		(3) Max Score AI Player --> enter "CPUS" followed by any other characters and spaces 
        
			- this player will play the word that maximizes the total increase in score
			- if no valid move is found, the player will default to a Pass for that turn
**Notes on Files:**

	.h and .cpp files are stored in src folder
	after compiling, .o files are created stored in created BIN folder

**User Interface**

![GitHub Logo](https://i.ibb.co/Ry6hykn/img-0747-orig.png)

**Enjoy the Game, and good luck against the AIs!**
