/******************************************************************************/
/*!
 * @file:					  dealer.hpp
 * @author  Original:     David Olave
 * @date    Created:      August 17th, 2017
 *
 *  @Description: The header file that defines dealer operations.
 *
 *  Copyright David Olave 2017
 ******************************************************************************/

#ifndef __DEALER_HPP__
#define __DEALER_HPP__

/* Library includes */
#include <list>
#include <string>
#include <vector>

/* Typedefines */

/* Card Structure */
typedef struct __Card__
	{
	unsigned int value;
	std::string name;
	bool isFace;       // The card is a high Face  value card
	}TCard;

/* Card deck container */
typedef std::vector<TCard> TCards;

/*
 * The dealer class
 * Implements dealer options
 * */
class TDealer
	{

	public:
		TDealer(void);
		~TDealer(void){};
		TCard dealCard(TCards &cardDeck);
		int shuffle(TCards &cardDeck);

	private:

	};

#endif /* __DEALER_HPP__ */
