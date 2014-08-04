#include <iostream>
#include <cstdlib>//rand(), srand()
#include <ctime>//time
#include <limits>//max()

using namespace std;
const unsigned int DECKSIZE = 52;

class CDeck
{
private:
    unsigned int m_deck[DECKSIZE]=
    {2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,5,6,6,6,6,7,7,7,7,8,8,8,8,9,9,9,9,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,11,11,11,11};

public:
    void showDeck();
    void shuffleDeck();

    friend class CGame;
};

void CDeck::showDeck()
{
    cout << "This is your deck:" << endl;
    for(int i = 0; i < DECKSIZE; i++)
    {
        cout << m_deck[i] << "\t";
    }
    cout << endl << endl;
}

void CDeck::shuffleDeck()
{
    int _x = 0;
    for(int i = 0; i < DECKSIZE; i++)
    {
        _x = rand() % (DECKSIZE - i) + i;
        swap(*(m_deck + i), *(m_deck + _x));
    }
}

class CPlayer
{
private:
    unsigned int m_winnings;
    unsigned int m_losses;

    friend class CGame;
};

class CGame
{
private:
    CDeck oNewDeck;
    CPlayer oNewPlayer;
    unsigned int m_playerHand[DECKSIZE]={0};
    unsigned int * m_pPlayerHand = m_playerHand;
    unsigned int m_CPUHand[DECKSIZE]={0};
	unsigned int * m_pCPUHand = m_CPUHand;
    unsigned int m_handScore;
    bool m_noCardsInDeck;

    void hitCard(CDeck &oNewDeck, unsigned int *a_pHand);
    void updateDeck(CDeck &oNewDeck);
	unsigned int deal(CDeck &oNewDeck);
    unsigned int countHowManyAcesInHand(unsigned int *a_pHand);
    void game(CPlayer &oNewPlayer, CDeck &oNewDeck, unsigned int a_iPlayerChoice);
    unsigned int countHandScore(unsigned int *a_pHand);
    bool goEmptyHands(unsigned int *a_pHand);
    bool checkHand(unsigned int *a_pHand);
    void showTable(unsigned int *a_pHand);
    void showCurrentScore(unsigned int *a_pHand);
    unsigned int countWhoWin();

public:
	CGame();
    void interfaceOfTheGame();
    bool isThereNoCardsInDeck() { return m_noCardsInDeck; }
    void showStats();
};

CGame::CGame()
{
    cout << "Hello World! This is awesome blackjack game!" << endl << endl;
    oNewDeck.shuffleDeck();
    oNewPlayer.m_losses = 0;
    oNewPlayer.m_winnings = 0;
}

void CGame::interfaceOfTheGame()
{
    unsigned int _playerChoice;

    while((cout << "[1] HIT / [2] STAND ([0] EXIT) ") && (!(cin >> _playerChoice) || _playerChoice < 0 || _playerChoice > 3))
    {
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    game(oNewPlayer, oNewDeck, _playerChoice);
}

void CGame::showStats()
{
    cout << endl << "Tokens (win/loss): " << oNewPlayer.m_winnings << "/" << oNewPlayer.m_losses << endl << endl;
}

unsigned int CGame::countHowManyAcesInHand(unsigned int *a_pHand)
{
    unsigned int _result = 0;
    for(int i = 0; a_pHand[i] != 0; i++)
    {
        if(11 == a_pHand[i])
            _result++;
    }
    return _result;
}

void CGame::game(CPlayer &oNewPlayer, CDeck &oNewDeck, unsigned int a_iPlayerChoice)
{
    if(1 == a_iPlayerChoice)
    {
        hitCard(oNewDeck, m_pPlayerHand);
        showTable(m_pPlayerHand);
        showCurrentScore(m_pPlayerHand);
		
		if(21 == countHandScore(m_pPlayerHand))// when 21
        {
            cout << "Blackjack You Win!" << endl << endl;
        	oNewPlayer.m_winnings+=3;
            goEmptyHands(m_pPlayerHand);
            goEmptyHands(m_pCPUHand);
        }
		
        if(false == checkHand(m_pPlayerHand))//returns false when over 21
        {
            cout << "Over 21! You have lost." << endl << endl;
            oNewPlayer.m_losses+=2;
            goEmptyHands(m_pPlayerHand);
            goEmptyHands(m_pCPUHand);
        }
    }

    if(2 == a_iPlayerChoice)
    {
        cout << endl << "-----------------------------" << endl << "Player STAND. It's CPU turn." << endl << endl;
        while(false == m_noCardsInDeck)
        {
            unsigned int _numberOfAcesInHand = countHowManyAcesInHand(m_pCPUHand);
            unsigned int _CPUScoreWithAces = countHandScore(m_pCPUHand) - (10 * _numberOfAcesInHand);

            if(17 >= _CPUScoreWithAces)
            {
                hitCard(oNewDeck, m_pCPUHand);
                showTable(m_pCPUHand);
                showCurrentScore(m_pCPUHand);
            }

            if(false == checkHand(m_pCPUHand))//returns false when over 21
            {
                cout << "Over 21! Player has won." << endl << endl;
                oNewPlayer.m_winnings+=2;
                break;
            }

            if(17 < _CPUScoreWithAces)
            {
                unsigned int _result = countWhoWin();

                if(0 == _result)
                {
                    cout << "It's a draw!" << endl << endl;
                }
                else if(1 == _result)
                {
                    cout << "Player has won." << endl << endl;
                    oNewPlayer.m_winnings+=2;
                }
                else if(2 == _result)
                {
                    cout << "Player has lost." << endl << endl;
                    oNewPlayer.m_losses+=2;
                }

                break;
            }

        }
        goEmptyHands(m_pPlayerHand);
        goEmptyHands(m_pCPUHand);
    }

    if(0 == a_iPlayerChoice)
    {
        showStats();
        exit(EXIT_SUCCESS);
    }
}

void CGame::hitCard(CDeck &oNewDeck, unsigned int *a_pHand)
{
    for(int i = 0; i < DECKSIZE; i++)
    {
        if(0 == a_pHand[i])
        {
            a_pHand[i] = deal(oNewDeck);
            break;
        }
    }
}

void CGame::updateDeck(CDeck &oNewDeck)
{
    for(int i = 1; i < DECKSIZE; i++)
    {
        oNewDeck.m_deck[i - 1] = oNewDeck.m_deck[i];
    }
    oNewDeck.m_deck[DECKSIZE-1] = 0;
}

unsigned int CGame::deal(CDeck &oNewDeck)
{
    unsigned int _takenCard = oNewDeck.m_deck[0];

    if(_takenCard)
    {
        updateDeck(oNewDeck);
        m_noCardsInDeck = false;
    }
    else
    {
        cerr << "There is no card left in Deck - bets are void" << endl;
        m_noCardsInDeck = true;
    }

    return _takenCard;
}

void CGame::showTable(unsigned int *a_pHand)
{
    for(int i = 0; (DECKSIZE > i && 0 != a_pHand[i]); i++)
    {
        cout << "(" << a_pHand[i] << ")" << "\t";
    }
}

void CGame::showCurrentScore(unsigned int *a_pHand)
{
    unsigned int _numberOfAcesInHand = countHowManyAcesInHand(a_pHand);

    if(0 < _numberOfAcesInHand)
    {
        cout << endl << "Current score: (" << countHandScore(a_pHand) - (10 * _numberOfAcesInHand) << ")/(" << countHandScore(a_pHand) <<  ")" << endl << endl;
    }
    else//there is no ace in hand
    {
        cout << endl << "Current score: (" << countHandScore(a_pHand) <<  ")" << endl << endl;
    }
}

bool CGame::checkHand(unsigned int *a_pHand)
{
    bool _result;
    unsigned int _score = countHandScore(a_pHand);
    unsigned int _numberOfAcesInHand = countHowManyAcesInHand(a_pHand);

    if(21 < (_score - (10 * _numberOfAcesInHand)))
    {
        _result = false;
    }
    else
    {
        _result = true;
    }

    return _result;
}

unsigned int CGame::countHandScore(unsigned int *a_pHand)
{
    unsigned int _handSum = 0;
    for(int i = 0; (DECKSIZE > i && 0 != a_pHand[i]); i++)
    {
        _handSum = _handSum + a_pHand[i];
    }
    return _handSum;
}

unsigned int CGame::countWhoWin()
{
	unsigned int _playersScore = countHandScore(m_pPlayerHand);
    unsigned int _numberOfAcesInHand = countHowManyAcesInHand(m_pCPUHand);
    unsigned int _CPUScoreWithAces = countHandScore(m_pCPUHand) - (10 * _numberOfAcesInHand);
    unsigned int _result;
	
    if(_playersScore == _CPUScoreWithAces)
    {
        _result = 0;
    }

    if(_playersScore > _CPUScoreWithAces)
    {
        _result = 1;
    }

    if(_playersScore < _CPUScoreWithAces)
    {
        _result = 2;
    }

    return _result;
}

bool CGame::goEmptyHands(unsigned int *a_pHand)
{
    bool _result;
    for(int i = 0; a_pHand[i] != 0; i++)
    {
        a_pHand[i] = 0;
    }
    return _result;
}

int main()
{
    srand(time(NULL));
    CGame oNewGame;

    while(false == oNewGame.isThereNoCardsInDeck())
    {
        oNewGame.interfaceOfTheGame();
    }

    cout << "It's over!" << endl;
    oNewGame.showStats();

    return 0;
}
