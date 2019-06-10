/******************************************************************************/
/*!
 * @file:					  blackjack.hpp
 * @author  Original:     David Olave
 * @date    Created:      August 17th, 2017
 *
 *  @Description: File that implements the blackjack class and its
 *  					operations.
 *
 *  Copyright David Olave 2017
 ******************************************************************************/


/* Library includes */
#include <sstream>
#include <unistd.h>

/* Local includes */
#include "misc.hpp"
#include "blackjack.hpp"

/*
 * User plays hand with dealer
 * @return: - 0 - Success playing hand with dealer. Otherwise
 * 			Error
 */
	int
TBlackjack::playHand
	(
	TCards &cardDeck
	)
	{
	TDealer dealer;
	TCards userCards, dealerCards;   // User hand
	int ret = -1; // Assumes failure playing hand
	// Get successfully play hands
	unsigned int HandsPlayed = getSuccessPlHandsCount();
	// Check amount of hands played. Every six hands, cards are shuffle
	if (!(HandsPlayed % SHUFFLE_PERIOD_PLAYS))
		{
		if (HandsPlayed)
			{
			std::stringstream ss;
			ss << "Next count of " << HandsPlayed << " games (total games "
					"played count=" << HandsPlayed  << ") has been reached\n" << std::endl;
			log(LOG_INFO, ss.str());
			}
		dealer.shuffle(cardDeck);
		}


	// Dealer provides initial cards
	ret = drawInitialCards(dealer, cardDeck, dealerCards, userCards);
	if (ret)
		{
		log(LOG_INFO, "Error, when feeding first cards on the game\n\n");
		return ret;
		}

	int iniRet = initBlackjackChks(dealerCards, userCards);
	if (iniRet != HAND_OUTCOME_CONTINUE)
		{
		if (iniRet == HAND_OUTCOME_ERROR)
			return iniRet;
		else
			{
			incSuccessPlHandsCount();  // Do not increment if error playing a hand
			return 0;
			}
		}

	// Player requests for cards
	int userScore = userReqCards(dealer, cardDeck, userCards);
	if (userScore == HAND_OUTCOME_BUSTED)
		{
		incSuccessPlHandsCount();  // Do not increment if error playing a hand
		mStats.dealerWins++;
		mStats.userBusts++;
		return 0;
		}
	else if ((userScore == HAND_OUTCOME_ERROR) || (userScore <= 0))
		{
		std::stringstream ss;
		ss << "Error, when user requqesting cards. User score is (" << userScore <<
				"\n" << std::endl;
		mStats.errors++;
		return ret;
		}

	// Dealer draw cards until lower threshold has been reached
	unsigned int dealerScore = dealerReqCards(dealer, cardDeck, dealerCards);
	if (dealerScore == HAND_OUTCOME_BUSTED)
		{
		mStats.dealerBusts++;
		mStats.userWins++;
		incSuccessPlHandsCount();  // Do not increment if error playing a hand
		return 0;
		}
	else if ((dealerScore == HAND_OUTCOME_ERROR) || (userScore <= 0))
		{
		std::stringstream ss;
		ss << "Error, when dealer withdrawing cards. Dealer score is (" << userScore <<
				"\n" << std::endl;
		mStats.errors++;
		return ret;
		}

	// Scores are compared and winning side is computed. Stats are updated
	compareScores(userScore, dealerScore);
	incSuccessPlHandsCount();  // Do not increment if error playing a hand

	ret = 0;
	return ret;
	}

/*
 * User plays hand with dealer
 * Rule:	- House (dealer) wins in an event of a score tie even if both
 * 		dealer and player have an score of 21.
 *
 * @return: - 0 - Success playing hand with dealer. Otherwise
 * 			Error
 */
	int
TBlackjack::compareScores
	(
	const int  userScore,    // User hand score
	const int  dealerScore	 // Dealer hand score
	)
	{
	int ret = -1;
	if ((userScore < 0) || (dealerScore < 0) || (userScore > BLACKJACK_VAL) ||
			(dealerScore > BLACKJACK_VAL))
		{
		std::stringstream ss;
		ss << __FUNCTION__ << ": Error, invalid user (" << userScore << ") or dealer (" <<
				dealerScore << ") score. Could not determine hand winner\n" << std::endl;
		log(LOG_ERR, ss.str());
		return ret;
		}

	if (dealerScore >= userScore)
		{
		std::stringstream ss;
		ss << "Dealer WINS. Dealer has higher or equivalent score than user.\n"
				<< "Dealer score:" << dealerScore << ". User score:" << userScore <<
				"\n" << std::endl;
		mStats.dealerWins++;
		log(LOG_INFO, ss.str());
		}
	else
		{
		std::stringstream ss;
		ss << "User WINS. User has higher score than dealer.\n"
				<< "User score:" << userScore << ". Dealer score:" << dealerScore <<
				"\n" << std::endl;
		mStats.userWins++;
		log(LOG_INFO, ss.str());
		}

	ret = 0;
	return ret;
	}

/*
 * Dealer provides all cards to user and dealer herself/ himself
 * @return: - 0 - Success ditributing initial cards. Otherwise,
 * 			Error
 */
	int
TBlackjack::drawInitialCards
	(
	TDealer 	 &dealer,			// In
	TCards	 &cardDeck,			// In
	TCards	 &dealerCards,		//  Out argument
	TCards	 &userCards			//  Out argument
	)
	{
	if (cardDeck.empty())
		dealer.shuffle(cardDeck);

	// Deal cards to user
	TCard card = dealer.dealCard(cardDeck);
	std::stringstream ss;
	ss << "User has a(n) " << card.name << " and ";
	userCards.push_back(card);
	card = dealer.dealCard(cardDeck);
	ss << "a(n) " << card.name << "\n" << std::endl;
	userCards.push_back(card);

	log(LOG_INFO, ss.str());

	// Dealer feeds cards to him/herself
	//First card is facing down
	card = dealer.dealCard(cardDeck);
	dealerCards.push_back(card);
	// The other is facing up
	card = dealer.dealCard(cardDeck);
	dealerCards.push_back(card);

	ss.str(std::string());

	ss << "Dealer has one card facing down. The other is a(n) " <<
			card.name << "\n" << std::endl;

	log(LOG_INFO, ss.str());
	return 0;
	}

/*
 * Function:	verifyNatural
 * @return:		- True - if cards hand is a natural blackjack (face card and an Ace)
 */
	bool
TBlackjack::verifyNatural
	(
	TCards &cards
	)
	{
	if (cards.size() != INIT_CARDS_NR)
		return true;

	bool faceFound = false;  // Assume hand does not have a face card
	bool aceFound = false;   // Assume hand does not have a ace card
	for (TCards::iterator it = cards.begin(); it != cards.end(); it++)
		{
		if (it->isFace)
			faceFound = true;
		else if (it->value == ACE_MIN_VAL)
			aceFound = true;
		}
	return (faceFound && aceFound);

	}

/*
 * Initial black jack checks
 * Rule: If user and dealer have both a natural (face and Ace), it is a 'push'
 * 		which means the hand is a tie and no players has won.
 * @return: - HAND_OUTCOME_PUSHED	- Both dealer and user have Blackjack. The hand is a tie
 * 			- HAND_OUTCOME_CONTINUE - Neither dealer nor user have Blackjack. The game must continue
 * 			- HAND_OUTCOME_DWON		- Dealer's hand is a Blackjack but user is not. Dealer wins
 * 			- HAND_OUTCOME_UWON		- User's hand is a Blackjack but dealer is not. User wins
 * 			- HAND_OUTCOME_ERROR		- Function could not determining initial conditions.
 */
	int
TBlackjack::initBlackjackChks
	(
	TCards	 &dealerCards,		//  Out argument
	TCards	 &userCards			//  Out argument
	)
	{
	int ret = HAND_OUTCOME_ERROR;  // Assume function could not determining initial conditions.
	// Check for hands value sanity
	if ((dealerCards.size() != INIT_CARDS_NR) ||
			(userCards.size() != INIT_CARDS_NR))
		{
		std::stringstream ss;
		ss << "\nIncorrect number of user cards (expected = " << INIT_CARDS_NR <<
				" counted = " << userCards.size() << ") or dealer cards (expected = " << INIT_CARDS_NR <<
				" counted = " << dealerCards.size() << ")\n\n";
		log(LOG_ERR, ss.str());
		return ret;
		}


	bool dealerHasNat = verifyNatural(dealerCards);
	bool userHasNat = verifyNatural(userCards);

	if (dealerHasNat && userHasNat)
		{
		std::string str("\n -- Both Dealer and Player have Blackjack -- \n"
				"The hand is a tie (aka 'push') \n");
		log(LOG_INFO, str);

		str = "Dealer has: \n";
		log(LOG_INFO, str);
		printCards(dealerCards);

		str = "User has: \n";
		log(LOG_INFO, str);
		printCards(userCards);
		std::stringstream  ss;
		ss << std::endl;

		log(LOG_INFO, ss.str());
		mStats.pushes++;
		ret = HAND_OUTCOME_PUSHED;
		}
	else if(dealerHasNat)
		{
		std::string str("\n  -- Dealer wins with a natural (Blackjack) --\n");
		log(LOG_INFO, str);
		str = "Dealer has won with: \n";
		printCards(dealerCards);
		std::stringstream  ss;
		ss << std::endl;
		log(LOG_INFO, ss.str());
		mStats.dealerWins++;
		ret = HAND_OUTCOME_DWON;
		}
	else if(userHasNat)
		{
		std::string str("\n  --  Player wins with a Blackjack!!! --\n");
		log(LOG_INFO, str);
		str = "Player has won with: \n";
		printCards(userCards);
		std::stringstream  ss;
		ss << std::endl;
		log(LOG_INFO, ss.str());
		mStats.userWins++;
		ret = HAND_OUTCOME_UWON;
		}
	else
		{
		bool soft;
		bool print = false;
		if (getScore(dealerCards, soft, print) == BLACKJACK_VAL)
			{
			std::stringstream ss;
			ss << "\n -- Dealer wins with " << BLACKJACK_VAL << " and user has a non " <<
					"natural (Blackjack)\n";
			log(LOG_INFO, ss.str());
			mStats.dealerWins++;
			ret = HAND_OUTCOME_UWON;
			}
		else
			ret = HAND_OUTCOME_CONTINUE;
		}

	return ret;
	}

/*
 * Function: userReqCards
 * Description:User requests cards as preferred.
 * @return:	 User score or
 * 			- HAND_OUTCOME_BUSTED  - User went over BLACKJACK_VAL
 * 			- HAND_OUTCOME_ERROR	  - Error processing processing this function
 */
	int
TBlackjack::userReqCards
	(
	TDealer 	 &dealer,			// In
	TCards	 &cardDeck,			// In
	TCards	 &userCards			//  In/Out argument
	)
	{
	int ret = HAND_OUTCOME_ERROR;  // Assume system could not process user card requests
	// Print initial player score
	log(LOG_INFO, "Currently,");

	bool exit = false;  // Assume player would like to continue receiving cards
	do
		{
		bool dummy;   // Place holder
		log(LOG_INFO, " the user has:\n");
		printCards(userCards);
		log(LOG_INFO, "with a score of:\n");
		int score = getScore(userCards, dummy);  // Get and print score
		if (score <= 0)
			return HAND_OUTCOME_ERROR;

		if (score > BLACKJACK_VAL)
			{
			log(LOG_INFO, " Oooops, Player busted\n\n");
			return HAND_OUTCOME_BUSTED;
			}

		std::string reqStr("Do you want a Card?\n Please enter 'Y' to receive or 'N' "
						"to stop receiving cards from dealer\n");
		TReqInputRets retType = reqInput(reqStr, "N", "Y", MAX_VALID_INPUT_REQ_TRIES);
		if ((retType == REQ_INPUT_ERR) || (retType == REQ_INPUT_EXIT))
			{
			exit = true;
			ret = score;
			log(LOG_ERR, "Player Stands\n\n");
			}
		else if (retType == REQ_INPUT_CONT)
			{
			// Get card from dealer
			TCard card = dealer.dealCard(cardDeck);
			userCards.push_back(card);
			log(LOG_INFO, "Now, ");
			}
		else
			{
			std::stringstream ss;
			ss << "Internal error, incorrect value (" << (unsigned int) retType <<
					") returned from processing request input \n" << std::endl;
			log(LOG_ERR,  ss.str());
			}
		}while(!exit);

	return ret;
	}

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
	int
TBlackjack::dealerReqCards
	(
	TDealer 	 &dealer,			// In
	TCards	 &cardDeck,			// In
	TCards	 &dealerCards		//  In/Out argument
	)
	{
#define DEALER_HIT_LIMIT			17
#define DEALER_HIT_SOFT_SCORE    DEALER_HIT_LIMIT
	int ret = HAND_OUTCOME_ERROR;  // Assume system could not process user card requests
	// Print initial player score
	log(LOG_INFO, "Currently,");

	bool exit = false;  // Assume player would like to continue receiving cards
	do
		{
		bool isSoft = false;   // Assume the hand score is not soft
		log(LOG_INFO, " the dealer has:\n");
		printCards(dealerCards);
		log(LOG_INFO, " with a score of:\n");
		int score = getScore(dealerCards, isSoft);
		if (score <= 0)
			return HAND_OUTCOME_ERROR;

		// Rules processing section
		if (score > BLACKJACK_VAL)
			{
			log(LOG_INFO, " Oooops, Dealer busted\n\n");
			return HAND_OUTCOME_BUSTED;
			}
		else if ((score > (DEALER_HIT_LIMIT - 1)) && (score < (BLACKJACK_VAL + 1) ))
			{
			if (!(isSoft && (score == DEALER_HIT_SOFT_SCORE)))
				{// If not soft rule
				log(LOG_INFO, " Dealer Stands\n\n");
				return (int)score;
				}
			}

		sleep(1);  // Delay to let user read dealer drawing card execution
		// Get card from dealer
		TCard card = dealer.dealCard(cardDeck);
		dealerCards.push_back(card);
		log(LOG_INFO, "Now, ");
		} while(!exit);
	return ret;
	}

/*
 * Function: 		getScore
 * Description:	Calculates maximum potential cards score value from a hand.
 * @return:	- User score.
 * 			- Less than zero if error
 */
	int
TBlackjack::getScore
	(
	TCards	 	 &cards,			//  In argument
	bool			 &soft,			//  Return value is soft
	const bool	print				//	 Print score(s) according to card contents
	)
	{
	unsigned int ret = 0;
	bool aceFound = false;  // Assume Ace has not been found
	unsigned int accum = 0;
	for (TCards::iterator it = cards.begin(); it != cards.end(); it++)
		{
		if (it->value == ACE_MIN_VAL)
			aceFound = true;

		accum +=(int)it->value;
		}

	if (accum <= 0)
		{

		std::stringstream ss;
		ss << __FUNCTION__ << ": Error, invalid card score = " << accum
				<< "\n" << std::endl;
		log(LOG_ERR, ss.str());
		return accum;
		}

	/*
	 * Choose the largest number close to BLACKJACK_VAL. Ace value can be
	 * used as 1 or 11. 11 Can be selected unles it causes the hand to bust
	 */
	if ((accum > BLACKJACK_VAL)  || !aceFound)
		{
		//  Cards are a bust or no Ace found. Return cards score without conversions
		if (print)
			{
			std::stringstream ss;
			ss << accum << "\n" << std::endl;
			log(LOG_INFO, ss.str());
			}
		ret = accum;
		}
	else
		{ // Ace found return max value close to BLACKJACK_VAL
		if (accum <= ACE_MAX_VAL)
			{// Value is soft
			ret = accum - ACE_MIN_VAL + ACE_MAX_VAL;
			if (print)
				{
				std::stringstream ss;
				ss << "soft " << ret << " or hard " << accum << "\n" << std::endl;
				log(LOG_INFO, ss.str());
				}
			}
		else
			{ // Cannot use Ace as soft
			if (print)
				{
				std::stringstream ss;
				ss << accum << "\n" << std::endl;
				log(LOG_INFO, ss.str());
				}
			ret = accum;
			}
		}

	return ret;
	}

/*
 * Print cards for a specific hand
 * @return: - 0 - Printing cards. Otherwise,
 * 			Error
 */
	int
TBlackjack::printCards
	(
	TCards &cards
	)
	{
	for (TCards::iterator it = cards.begin(); it != cards.end(); it++)
		{
		log(LOG_INFO, it->name);
		if ((it + 1) == cards.end())
			log(LOG_INFO, " ");
		else
			log(LOG_INFO, ", ");
		}
	log(LOG_INFO, "\n");
	return 0;
	}

/*
 * Prints Blackjack game results stats
 * @return: - 0 -  Printing stats, otherwise
 * 			Error
 */
	int
TBlackjack::printStats
	(
	void
	)
	{
	printf(" printing stats\n");
	int ret = -1;   // Assume error printing stats
	std::stringstream ss;
	unsigned long handsPlayed = getSuccessPlHandsCount();
	ss << "*******Virtual Blackjack Statistics*******\n\n" <<
			"Pushes (ties):\t\t" << mStats.pushes << "( %" <<
			(float)((float)mStats.pushes/(float)handsPlayed) << ") \n" <<

			"Wins:\n" <<

			"Player wins:\t\t" << mStats.userWins << "( %" <<
			(float)((float)mStats.userWins/(float)handsPlayed) * 100 << ") \n" <<

			"Dealer wins:\t\t" << mStats.dealerWins << "( %" <<
			(float)((float)mStats.dealerWins/(float)handsPlayed) * 100 << ") \n" <<

			"Hands Played:\t\t" << handsPlayed << "\n" <<

			"Other stats:\n" <<

			"Player busts (over "<< BLACKJACK_VAL << "):\t\t" << mStats.userBusts << "( %" <<
			(float)((float)mStats.userBusts/(float)handsPlayed) * 100 << ") \n" <<

			"Dealer Busts (over "<< BLACKJACK_VAL << "):\t\t" << mStats.dealerBusts << "( %" <<
			(float)((float)mStats.dealerBusts/(float)handsPlayed) * 100 << ") \n" <<

			"\nErrors executing game:" << mStats.errors  << "\n\n" <<

			 std::endl;
	log (LOG_INFO, ss.str());
	return ret;
	}
