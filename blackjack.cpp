/******************************************************************
*File: HW2.cpp
*Author: Raul Lopez
History: Raul Lopez 3/7/2021
				 Raul Lopez 3/12/2021
				 Raul Lopez 3/13/2021
				 Raul Lopez 3/14/2021
Procedures: 
card						-class to represent a playing card with suit and face value
deck						-class to hold a deck of cards
player					-class to represent a player's hand
main						-initializes data and runs 21 games
player_one_play	-function to simulate player one strategy 
player_two_play	-funciton to simulate player two strategy
dealer_play			-function for dealer strategy
determine_winner-function to see if player or dealer won
output					-function to print information
******************************************************************/
#include <iostream>
#include <ctime>
#include <string>
#include <vector>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#define SIZE 52
#define NUM_TRIALS 1000

using namespace std;

/******************************************************************
class card
Author: Raul Lopez
Date: 3/7/2021
Description: Used to represent a playing card. Stores face value, suit, and if it is an ace. 

Parameters:
	face		I/P string	The face of the card
	suit		I/P string	The suit of the card
	value		O/P	int 		The face value of the card
	is_ace	O/P	bool		Whether the card is an ace or not
	print		O/P	string	Return face and suit of card

******************************************************************/
class card {
public:
	// default constructor
	card();
	// constructor with values
	card(string card_face, string card_suit);
	//print function
	string print();
	// return value of card
	int value();
	bool is_ace();

private:
	string face;
	string suit;
};

card::card() {
	// empty constructor
}
card::card(string card_face, string card_suit) {
	face = card_face;
	suit = card_suit;
}

string card::print() {
	string out = face + " of " + suit;
	return out;
}

int card::value() {
	if (face.compare("Ace") == 0)
		return 11;
	else if (face.compare("2") == 0)
		return 2;
	else if (face.compare("3") == 0)
		return 3;
	else if (face.compare("4") == 0)
		return 4;
	else if (face.compare("5") == 0)
		return 5;
	else if (face.compare("6") == 0)
		return 6;
	else if (face.compare("7") == 0)
		return 7;
	else if (face.compare("8") == 0)
		return 8;
	else if (face.compare("9") == 0)
		return 9;
	else
		return 10;
}

bool card::is_ace() {
	if (face.compare("Ace") == 0)
		return true;
	else
		return false;
}

/******************************************************************
class deck
Author: Raul Lopez
Date: 3/12/2021
Description: Used to represent a deck. Holds an array of type card.  

Parameters:
	current_card	O/P	int		Used to keep track of index of deck
	shuffle				O/P	void	Shuffles the deck
	deal_card			O/P	card	Returns a card from the deck
******************************************************************/
class deck {
public:
	//default constructor
	deck();
	void shuffle();
	card deal_card();
private:
	card card_deck[SIZE];
	int current_card;
};

deck::deck() {
	string faces[] = { "Ace", "2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King" };
	string suits[] = { "Hearts", "Diamonds", "Clubs", "Spades" };

	current_card = 0;

	// loop to place cards in array
	for (int i = 0; i < SIZE; i++) {
		card_deck[i] = card(faces[i % 13], suits[i / 13]);
	}
}

void deck::shuffle() {
	current_card = 0;

	// loop to shuffle all 52 cards
	for (int first = 0; first < SIZE; first++) {
		int second = (rand() + time(0)) % SIZE;
		// use a temporary card to switch spots of two cards 
		card temp = card_deck[first];
		card_deck[first] = card_deck[second];
		card_deck[second] = temp;
	}

}

card deck::deal_card() {
	// return first card and increment counter for card spot
	return card_deck[current_card++];
}

/******************************************************************
class player
Author: Raul Lopez
Date: 3/12/2021
Description: Used to represent a player. Uses a vector to represent the player/dealer's hand.   

Parameters:
	new_card					I/P	card	card to be added to player's hand
	get_hand_value		O/P	int		return total vlaue of player's cards
	get_ace_count			O/P	int		return the number of aces in player's hand
	reduce_ace_count	O/P	void	reduce the number of aces whose value is 11
	reduce_hand_value	O/P	void	reduce hand value by 10 due to ace
******************************************************************/
class player {
public:
	//default constructor
	player();
	void add_card(card new_card);
	int get_hand_value();
	int get_ace_count();
	void reduce_ace_count();
	void reduce_hand_value();
private:
	vector<card> hand;
	int hand_value = 0;
	int ace_count = 0;
};

player::player() {
	//empty constructor
}

void player::add_card(card new_card) {
	hand.push_back(new_card);
	hand_value += new_card.value();
	if (new_card.is_ace())
		ace_count++;	
}

int player::get_hand_value() {
	return hand_value;
}

int player::get_ace_count() {
	return ace_count;
}

void player::reduce_ace_count() {
	ace_count--;
}

void player::reduce_hand_value() {
	hand_value -= 10;
}

/******************************************************************
int player_one_play(player p1, deck d)
Author: Raul Lopez
Date: 3/13/2021
Description: Used to simulate player one's strategy.  
						 strategy: if hand > 11 stand

Parameters:
	p1					I/P		player	player one
	d						I/P		deck		deck of cards
	hand_value	O/P		int			return hand value after done drawing cards
******************************************************************/
int player_one_play(player p1, deck d) {
	// draw two cards
	card card = d.deal_card();
	p1.add_card(card);
	card = d.deal_card();
	p1.add_card(card);

	// loop to keep hitting if hand < 12
	while (p1.get_hand_value() <= 11) {
		card = d.deal_card();
		p1.add_card(card);
		// if player busted and has an ace change value of ace from 11 to 1
		if (p1.get_hand_value() > 21 && p1.get_ace_count() > 0) {
			p1.reduce_ace_count();
			p1.reduce_hand_value();
		}
	}
	return p1.get_hand_value();
}
 
/******************************************************************
int player_two_play(player p2, deck d)
Author: Raul Lopez
Date: 3/13/2021
Description: Used to simulate player two's strategy.  
						strategy: if hand < 16 hit
Parameters:
	p2					I/P		player	player two
	d						I/P		deck		deck of cards
	hand_value	O/P		int			return hand value after done drawing cards
******************************************************************/
int player_two_play(player p2, deck d) {
	card card = d.deal_card();
	p2.add_card(card);
	card = d.deal_card();
	p2.add_card(card);

	// loop to keep hitting if hand < 16
	while (p2.get_hand_value() < 16) {
		card = d.deal_card();
		p2.add_card(card);
		// if player busted and has an ace change value of ace from 11 to 1
		if (p2.get_hand_value() > 21 && p2.get_ace_count() > 0) {
			p2.reduce_ace_count();
			p2.reduce_hand_value();
		}
	}
	return p2.get_hand_value();
}

/******************************************************************
int dealer_play(player dealer, deck d)
Author: Raul Lopez
Date: 3/13/2021
Description: Used to simulate player dealer's strategy.  
						strategy: rules of dealer
Parameters:
	dealer			I/P		player	dealer
	d						I/P		deck		deck of cards
	hand_value	O/P		int			return hand value after done drawing cards
******************************************************************/
int dealer_play(player dealer, deck d) {
	card card = d.deal_card();
	dealer.add_card(card);
	card = d.deal_card();
	dealer.add_card(card);

	// loop to keep hitting if hand < 16
	while (dealer.get_hand_value() < 17) {
		card = d.deal_card();
		dealer.add_card(card);
		// if player busted and has an ace change value of ace from 11 to 1
		if (dealer.get_hand_value() > 21 && dealer.get_ace_count() > 0) {
			dealer.reduce_ace_count();
			dealer.reduce_hand_value();
		}
	}
	return dealer.get_hand_value();
}

/******************************************************************
int determine_winner(int p, int dealer)
Author: Raul Lopez
Date: 3/13/2021
Description: Used to determine if the player or the dealer has won
Parameters:
	dealer			I/P		int	value of dealer's cards
	p						I/P		int	value of player's cards
	winner			O/P		int	return 0 for player win, 1 for dealer win, and 2
										for tie
******************************************************************/
int determine_winner(int p, int dealer) {
	// if player busted
	if (p > 21)
		return 1;
	// if dealer busted
	else if (dealer > 21)
		return 0;
	// if player won
	else if (p > dealer)
		return 0;
	// if dealer won 
	else if (dealer > p)
		return 1;
	// tie
	else
		return 2;
}
/******************************************************************
int output(int p1, int p2, int d1, int d2)
Author: Raul Lopez
Date: 3/14/2021
Description: Used to print total number of wins for players and dealer
Parameters:
	p1		I/O	int	number of player one wins
	p2		I/O	int	number of player two wins
	d1		I/O	int	number of times dealer won against player one
	d2		I/O	int	number of times dealer won against player two
******************************************************************/
void output(int p1, int p2, int d1, int d2){
	setbuf(stdout,NULL);
	printf("Dealer wins vs player 1 count: %d\n", d1);
	printf("Dealer wins vs player 2 count: %d\n", d2);
	printf("Player 1 win count: %d\n", p1);
	printf("Player 2 win count: %d\n", p2);
}

/******************************************************************
int main()
Author: Raul Lopez
Date: 3/14/2021
Description: runs 1000 twenty-one game trials. creates players, dealer, and deck. 
Parameters:
	main	O/P	int	status code(not used)
******************************************************************/
int main() {
	int dealer_one_wins = 0;
	int dealer_two_wins = 0;
	int player_one_wins = 0;
	int player_two_wins = 0;	

	// loop for trials
	for (int i = 0; i < NUM_TRIALS; i++) {
		// create the deck and shuffle at start of each trial 
		deck card_deck;
		card_deck.shuffle();
		
		//create players and dealer
		player dealer;
		player player_one;
		player player_two;	
		
		//create pipe 
		int fd[2];
		pid_t child_a, child_b;		
		pipe(fd);	
		
		// create first child
		fflush(stdout);
		child_a = fork();
		if(child_a == 0){
			// Child a code
			int score_array[] = {0,0,0};
			int player_one_hand = player_one_play(player_one, card_deck);
			score_array[0] = player_one_hand;
			
			// send score through pipe
			write(fd[1], score_array, sizeof(score_array));
			exit(EXIT_SUCCESS);			
		}
		else{
			fflush(stdout);
			child_b = fork();
			if (child_b == 0){
				// Child b code
				int player_two_hand = player_two_play(player_two, card_deck);
				waitpid(child_a, NULL, 0);
				int score_array[3];
				
				// read in previous score
				read(fd[0], score_array, sizeof(score_array));
				score_array[1] = player_two_hand;
				
				// write player two score
				write(fd[1], score_array, sizeof(score_array));
				exit(EXIT_SUCCESS);
			}
			else{
				setbuf(stdout,NULL);
				// Parent code
				waitpid(child_a, NULL, 0);
				waitpid(child_b, NULL, 0);

				int dealer_hand = dealer_play(dealer, card_deck);
				int score_array[3];
				read(fd[0], score_array, sizeof(score_array));
				score_array[2] = dealer_hand;

				// get all scores from array
				int player_one_hand = score_array[0];
				int player_two_hand = score_array[1];
				
				int player_one_match = determine_winner(player_one_hand, dealer_hand);
				int player_two_match = determine_winner(player_two_hand, dealer_hand);
				// switch case for player one vs dealer
				switch (player_one_match) {
				// player one wins
				case 0:
					player_one_wins++;
					break;
				// dealer wins
				case 1:
					dealer_one_wins++;
				}
				// switch case for player two vs dealer
				switch (player_two_match){
				// player two wins
				case 0:
					player_two_wins++;
					break;
				// dealer wins
				case 1:
					dealer_two_wins++;
				}
			}
		}	
		// close pipe
		close(fd[0]);
		close(fd[1]);	
	}
	// print out dealer and player wins	
	output(player_one_wins, player_two_wins, dealer_one_wins, dealer_two_wins);
	return 0;
}

