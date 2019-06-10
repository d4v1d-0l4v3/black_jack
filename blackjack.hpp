/******************************************************************************/
/*!
 * @file:					  blackjack.hpp
 * @author  Original:     David Olave
 * @date    Created:      August 17th, 2017
 *
 *  @Description: The header file to define the blackjack class and its
 *  					operations.
 *
 *  Copyright David Olave 2017
 ******************************************************************************/

#ifndef __BLACKJACK_HPP__
#define __BLACKJACK_HPP__

/* Local includes */
#include "dealer.hpp"
#include <string.h>

/*
 * The blackjack class
 * Carry out Blackjack Hands with one player and a dealer
 * */
class TBlackjack
	{
	// Typedefs
	typedef struct __TBlackJackStats__ {
		unsigned long successPlyd;   // Successfully played hands
		unsigned long pushes;
		unsigned long userBusts;
		unsigned long dealerBusts;
		unsigned long userWins;
		unsigned long dealerWins;
		unsigned long errors;    // General processing errors
	}TBlackJackStats;

	enum {
		HAND_OUTCOME_CONTINUE= -6,		// Continue playing hand
		HAND_OUTCOME_UWON		= -5,		// User won
		HAND_OUTCOME_DWON		= -4,		// Dealer won
		HAND_OUTCOME_PUSHED 	= -3,
		HAND_OUTCOME_BUSTED 	= -2,
		HAND_OUTCOME_ERROR  	= -1
	};

	// Number of plays per shuffle
	static const unsigned int SHUFFLE_PERIOD_PLAYS   = 6;
	static const unsigned int BLACKJACK_VAL = 21;
	static const unsigned int INIT_CARDS_NR = 2;
	static const unsigned int ACE_MIN_VAL 	 = 1;
	static const unsigned int ACE_MAX_VAL   = 11;
	// Function members
	public:
		TBlackjack(void) {memset((void *)&mStats, 0, sizeof(mStats));};
		~TBlackjack(void){};
		int playHand(TCards &cardDeck);
		/*
		 * Initial black jack checks
		 * Rule: If user and dealer have both a natural (face and Ace), it is a 'push'
		 * 		which means the hand is a tie and no players has won.
		 * @return: - HAND_OUTCOME_PUSH		- Both dealer and user have Blackjack. The hand is a tie
		 * 			- HAND_OUTCOME_CONTINUE - Neither dealer nor user have Blackjack. The game must continue
		 * 			- HAND_OUTCOME_DWIN		- Dealer's hand is a Blackjack but user is not. Dealer wins
		 * 			- HAND_OUTCOME_UWIN		- User's hand is a Blackjack but dealer is not. User wins
		 * 			- HAND_OUTCOME_ERR		- Function could not determining initial conditions.
		 */
		int initBlackjackChks (TCards &dealerCards,	TCards &userCards);
		/*
		 * Prints Blackjack game results stats
		 * @return: - 0 -  Printing stats, otherwise
		 * 			Error
		 */
		int printStats (void);
		// Get successfully played hands
		unsigned int getSuccessPlHandsCount(void) {return mStats.successPlyd;};

	private:
		// Increment successfully played hands
		unsigned int incSuccessPlHandsCount(void) {return ++mStats.successPlyd;};
		/*
		 * Dealer distributes all cards to user and dealer herself/ himself
		 * @return: - 0 - Success ditributing initial cards. Otherwise,
		 * 			Error
		 */
		int drawInitialCards (TDealer &dealer, TCards &cardDeck,
				TCards &dealerCards,TCards	&userCards);
		/*
		 * Function: 		getScore
		 * Description:	Calculates maximum potential cards score value from a hand
		 * 					and print its score if selected
		 * @return:			- User score.
		 * 					- Less than zero if error
		 */
		int getScore (TCards &cards, bool &soft, const bool print = true);
		/*
		 * Print cards for a specific hand
		 * @return: - 0 - Printing cards. Otherwise,
		 * 			Error
		 */
		int printCards (TCards &cards);

		/*
		 * Function: playerReqCards
		 * Description:User requests cards as preferred.
		 * @return:	 User score or
		 * 			- HAND_OUTCOME_BUSTED  - User went over BLACKJACK_VAL
		 * 			- HAND_OUTCOME_ERROR	  - Error processing processing this function
		 */
		int userReqCards (TDealer &dealer, TCards &cardDeck, TCards &userCards);

		/*
		 * Function: dealerReqCards
		 * Description:Dealer requests cards as rules dictate.
		 * Rules: 	- Dealer must hit on soft 17.
		 * 			- Dealer must stand on hard 17 or higher soft or hard hands.
		 * @return:	 User score or
		 * 			- HAND_OUTCOME_BUSTED  - Dealer went over BLACKJACK_VAL
		 * 			- HAND_OUTCOME_STAND	  - Dealer stands with value equal or less than
		 * 			- HAND_OUTCOME_ERROR	  - Error processing processing this function
		 */
		int dealerReqCards (TDealer &dealer, TCards &cardDeck, TCards	 &dealerCards);

		/*
		 * User plays hand with dealer
		 * Rule:	- House (dealer) wins in an event of a score tie even if both
		 * 		dealer and player have an score of 21.
		 *
		 * @return: - 0 - Success playing hand with dealer. Otherwise
		 * 			Error
		 */
		int compareScores (const int userScore, const int dealerScore);

		/*
		 * Function:	verifyNatural
		 * @return:		- True - if cards hand is a natural blackjack (face card and an Ace)
		 */
		bool verifyNatural (TCards &cards);

	private:
		// Variable members
		TBlackJackStats mStats;
	};

#endif /* __BLACKJACK_HPP__ */
