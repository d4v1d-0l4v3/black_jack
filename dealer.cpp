/******************************************************************************/
/*!
 * @file:					  dealer.cpp
 * @author  Original:     David Olave
 * @date    Created:      August 17th, 2017
 *
 *  @Description: Implements dealer operations.
 *
 *  Copyright David Olave 2017
 ******************************************************************************/

/*
 * Library includes
 */
#include <random>
#include <iostream>
#include <sstream>

/* Local includes */
#include "misc.hpp"
#include "dealer.hpp"

/* Private defines */
#define MAX_CARD_VALUE  10
#define ACE_VALUE			1

TDealer::TDealer
	(
	void
	)
	{

	}

/*!
 * Get card from the card deck
 */
	TCard
TDealer::dealCard
	(
	TCards &cardDeck
	)
	{

	if (cardDeck.empty()) {
		log  (LOG_ERR, "\nCard deck empty, shuffling\n\n");
		shuffle(cardDeck);
	}

	TCard card = cardDeck.back();
	cardDeck.pop_back();

	return card;
	}

/*
 * Dealer shuffles card
 * @return: - 0 - Success shuffling cards. Otherwise,
 * 			Error
 *
 * @Note: Card deck size must larger than zero when the
 * 		 function returns.
 */
	int
TDealer::shuffle
	(
	TCards &cardDeck
	)
	{
	log (LOG_INFO, "Shuffling...\n\n");
	TCard card;
	// Temporal card storage
	TCards cardDeckTmp;
	// Remove all remaining cards on the deck
	cardDeck.clear();

	// Fill card deck
	for (unsigned int i = 1; i <= MAX_CARD_VALUE; i++)
		{
		card.value = i;
		card.isFace = false;
		if (i == ACE_VALUE)
			{
			card.name = "Ace of spades";
			cardDeckTmp.push_back(card);
			card.name = "Ace of clubs";
			cardDeckTmp.push_back(card);
			card.name = "Ace of diamonds";
			cardDeckTmp.push_back(card);
			card.name = "Ace of hearts";
			cardDeckTmp.push_back(card);
			}
		else
			{
			std::stringstream ss;
			ss << i;    // Extremely unlikely to fail
			card.name = ss.str() + " of spades";
			cardDeckTmp.push_back(card);
			card.name = ss.str() + " of clubs";
			cardDeckTmp.push_back(card);
			card.name = ss.str() + " of diamonds";
			cardDeckTmp.push_back(card);
			card.name = ss.str() + " of hearts";
			cardDeckTmp.push_back(card);
			card.value = i;
			}
		}
	// Insert higher ranks
	// Jacks
	card.isFace = true;
	card.name = "Jack of spades";
	cardDeckTmp.push_back(card);
	card.name = "Jack of clubs";
	cardDeckTmp.push_back(card);
	card.name = "Jack of diamonds";
	cardDeckTmp.push_back(card);
	card.name = "Jack of hearts";
	cardDeckTmp.push_back(card);

	// Queens
	card.name = "Queen of spades";
	cardDeckTmp.push_back(card);
	card.name = "Queen of clubs";
	cardDeckTmp.push_back(card);
	card.name = "Queen of diamonds";
	cardDeckTmp.push_back(card);
	card.name = "Queen of hearts";
	cardDeckTmp.push_back(card);

	// Kings
	card.name = "King of spades";
	cardDeckTmp.push_back(card);
	card.name = "King of clubs";
	cardDeckTmp.push_back(card);
	card.name = "King of diamonds";
	cardDeckTmp.push_back(card);
	card.name = "King of hearts";
	cardDeckTmp.push_back(card);

	// Now shuffle
	try
		{
		std::random_device rd;
		while(!cardDeckTmp.empty())
			{
			/*
			 * Selecting randomly a card from the tmp deck to insert on the function
			 * argued card deck
			 */
			unsigned int sel = rd() % cardDeckTmp.size();
			TCard card = cardDeckTmp[sel];
			cardDeckTmp.erase(cardDeckTmp.begin() + sel);
			cardDeck.push_back(card);
			}
		}
	catch (std::exception &e)
		{
		log (LOG_INFO, "System does not implement true random generator diverting to"
				 " pseudo number generator");
		/*
		 * Selecting randomly a card from the tmp deck to insert on the function
		 * argued card deck. Using a uniform distribution to allow equal probabilities
		 * for card selection from the deck.
		 */
		std::default_random_engine generator;
		while(!cardDeckTmp.empty())
			{
			std::uniform_int_distribution<unsigned int> uniDist(0, cardDeckTmp.size() - 1);
			unsigned int sel = uniDist(generator);
			TCard card = cardDeckTmp[sel];
			cardDeckTmp.erase(cardDeckTmp.begin() + sel);
			cardDeck.push_back(card);
			}
		}
	return 0;
	}


