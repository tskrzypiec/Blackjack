#include <iostream>
#include <algorithm>//swap()
#include <cstdlib>//rand(), srand()
#include <ctime>//time
using namespace std;

class Deck
{
	
private:
	
	static const unsigned int deckSize = 52;
	int deck[deckSize] = {2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,5,6,6,6,6,7,7,7,7,8,8,8,8,9,9,9,9,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,11,11,11,11};			
	int croupier[deckSize] = {0};
	int player[deckSize] = {0};			
	int lostToken = 0;
	int winToken = 0;
	
	int playersScore = 0;
	int croupiersScore = 0;
	bool hit = true;	
	bool playerStand = false;
	bool croupierPass = false; 
	bool playerLost = false;
	bool croupierLost = false;
			
public:
	
	void shuffle();
	void deal(); 
	void playersCards(); 
	void playersHit();
	void cardDelete(); //delete card from deck array
	void croupiersCards();
	void croupiersHit();
	void checkPlayersCards(); //check if we are over 21 or have blackjack
	void checkCroupiersCards();
	int croupiersTotal();
	int playersTotal();	 
	void results();
	bool cardsEnough(); //check if we have cards to play
	void reset(); 
	void emptyHands();
	void totalToken(); 
	
};

void Deck::shuffle()
{
	int random = 0;		
	for(int i = 0; i < 52; i++)
	{
		random = rand()%52;
		swap(deck[i], deck[random]);
	}
		
	return;	
}

void Deck::deal()
{	
    int firstIterator = 0;
    int secondIterator = 1;
	  
    for(int i = 0; i < 2; i++)
    {	
		player[i] = deck[firstIterator + firstIterator];
       	croupier[i] = deck[i + secondIterator];
       	
        firstIterator++;
        secondIterator++; 	
    } 
  	    
    for(int i = 0; i < 4; i++)//delete 4 cards we got from deck
    {
        cardDelete(); 		 
    } 
	
	playersTotal();
		
	checkPlayersCards();
					
	if(cardsEnough() == false)
    {	
        cout<< "\n No cards, game over. " <<endl;	
    }
			    
	return;
}

void Deck::playersCards()
{
	
	cout <<" \n\n Player's cards value: ";
	
	for(int i = 0; player[i]!= 0; i++)
	{
		cout<< player[i] << " ";		
	}
			
	return;
}

void Deck::croupiersCards()
{
	if(cardsEnough() == true)
	{
		cout<< "\n Croupier's card value: " << croupier[0] <<endl<<endl;
	}
		
	return;	
}

void Deck::checkPlayersCards()
{
	
	if(playersScore > 21)
	{
		playersCards();
		cout<<"\n It's over 21, You Loose!"<<endl;
		playerStand = true;
		croupierPass = true;
		playerLost = true;
		lostToken += 2;
	}
			
	if(playersScore == 21)
	{
		playersCards();
		cout<< "\n Blackjack! You Win!" <<endl;
		playerStand = true;
		croupierPass = true;
		croupierLost = true;
		winToken += 2;
	}	
	
	if(playersScore < 21)
	{
		playersCards();
	}
		
	return;	
	
}

void Deck::checkCroupiersCards()
{
	
	if(croupiersScore > 21)
	{
		cout<< " \n\n Croupier's cards value:   ";
	
		for(int i = 0; croupier[i]!= 0; i++)
		{
			cout<< croupier[i] << " ";		
		}
	
		cout<< " \n Croupier's cards value is over 21. You Win! " <<endl;
		
		croupierPass = true;
		croupierLost = true;
		winToken+=2;	
	}
			
	if(croupiersScore > 16 && croupiersScore < 21)
	{
		cout<< " \n\n Croupier's cards value:   ";
	
		for(int i = 0; croupier[i]!= 0; i++)
		{
			cout<< croupier[i] << " ";		
		}
		
		croupierPass = true;	
	}
	
	if(croupiersScore == 21)
	{
		
		cout<< " \n\n Croupier's cards value:   ";
	
		for(int i = 0; croupier[i]!= 0; i++)
		{
			cout<< croupier[i] << " ";		
		}
		
		playerLost = true;
		croupierPass = true;
		
		cout<< " \n Croupier has Blackjack. You Loose! " <<endl;
			
	}
				
	return;
}

void Deck::playersHit()
{	
	while(playerStand == false)
	{
		
		if(cardsEnough() == false)
		{
			cout<< " \n No cards, game over." <<endl;
			break;
		}
		
		cout<< "\n\n Hit? (yes - '1', no - '0') ";
	
		cin>> hit;
				
		if(hit == true)
		{	
		
			int NumberOfcards = 0;
	 
    		for(int i = 0; player[i]!= 0; i++) //check how many cards are in my "hand"
    		{
      			player[i];  
	  			NumberOfcards++;    
    		}
    
    		player[NumberOfcards] = deck[0]; //hit another card from deck
				
			cardDelete();
			
			playersTotal();	
			
			checkPlayersCards();
								
		}
		
		if(hit == false)
		{	
			playersTotal();
			playerStand = true;
		}
				
	}
		
	return;	
}	

void Deck::croupiersHit()
{
	while(croupierPass == false)
	{
		croupiersTotal();	
		
		while(croupiersScore <= 16)
		{
			
			if(cardsEnough() == false)
			{
				cout<< " \nNo cards, game over." <<endl;
				break;
			}
			
			int NumberOfcards = 0;
			
			for(int i = 0; croupier[i]!= 0; i++)
    		{
      			croupier[i];  
	  			NumberOfcards++;    
    		}
    		   	
			croupier[NumberOfcards] = deck[0];
			
    		cardDelete();
    		
			croupiersTotal();
							
		}
		
		checkCroupiersCards();
	
	}
	
	return;	
}

void Deck::cardDelete()
{
	for(int k = 0; k < 52; k++)//moving elements in my array
	{
		deck[k] = deck[k+1];
	}
	
	int lastCard = 51;	//I put 0 in the last cell which has junk after moving elements	
	deck[lastCard] = 0;
		
	return;	
}	

int Deck::playersTotal()
{
	playersScore = 0;
	
	for(int i = 0; player[i]!= 0; i++)
	{
		playersScore = playersScore + player[i];		
	}
			
	return playersScore;
}

int Deck::croupiersTotal()
{	
	croupiersScore = 0;
	
	for(int i = 0; croupier[i]!= 0; i++)
	{
		croupiersScore = croupiersScore + croupier[i];		
	}
	
	return croupiersScore;
}

void Deck::results()
{	

	if(croupierLost == false && playerLost == false)
	{
		
		cout<< " \n Player's score: " << playersScore <<endl;
		cout<< " Croupier's score: " << croupiersScore <<endl;

		if(playersScore > croupiersScore)
		{
			cout<< " You Win! Congratulations!" <<endl;
			winToken += 2;
		}
		
		if(playersScore < croupiersScore)
		{
			cout<< " You Lost with Croupier!" <<endl;
			lostToken += 2;
		}
		
		if(playersScore == croupiersScore)
		{
			cout<< " Draw!" <<endl;
		}
		
	}
		
	return;	
}

bool Deck::cardsEnough()
{	
	if(deck[0] == 0)
	{
		croupierLost = true;
		playerLost = true;
		croupierPass = true;
		playerStand = true;
			
		return false;
	}

	return true;				
}	

void Deck::emptyHands()
{
	for(int i = 0; player[i]!= 0; i++)
	{
		player[i] = 0;
	}
	
	for(int i = 0; croupier[i]!= 0; i++)
	{
		croupier[i] = 0;
	}	
	
	return;	
}

void Deck::reset()
{
	if(cardsEnough() == true)
	{
		croupierPass = false;
		croupierLost = false;
		playerStand = false;
		playerLost = false;
		hit = true;
		playersScore = 0;
		croupiersScore = 0;		
	}
		
	return;	
}
	
void Deck::totalToken()
{
	cout<< " \n Win Tokens score (2 Tokens bet per game): " << winToken <<endl;
	cout<< " Lost Tokens score (2 Tokens bet per game): " << lostToken <<endl;
	
	return;	
}	
	
int main()
{
	
	srand(time(NULL));	
	Deck blackjack;	

	blackjack.shuffle();
				
	while(blackjack.cardsEnough() == true)
	{	

		blackjack.deal();

		blackjack.croupiersCards();
				
		blackjack.playersHit();
		
		blackjack.croupiersHit();

		blackjack.results();
		
		blackjack.emptyHands();
		
		blackjack.reset();
				
	}
		blackjack.totalToken();
			
	return 0;
}
