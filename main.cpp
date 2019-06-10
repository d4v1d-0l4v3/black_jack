/******************************************************************************/
/*!
 * @file
 * @author  Original:     David Olave
 * @date    Created:      August 17th, 2017
 *
 *  @Description: The top file where black jack game is launched
 *
 *  Copyright David Olave 2017
 ******************************************************************************/

/* Local includes */
#include "blackjack.hpp"
#include "misc.hpp"

/* Library includes */
#include <stdio.h>
#include <iostream>
#include <getopt.h>
#include <unistd.h>
#include <sstream>

//! Option string for getopt for etrans. See opttab for long options.
char optstr[] = ":h";

//! Option table for getopt_long for etrans.
struct option opttab[] = {
   { "help",   no_argument,         NULL,    'h'   },
   { 0, 0, 0, 0 }
   };

/******************************************************************************/
/*! Print usage then exit the app.
 ******************************************************************************/
   void
usage
   (
   void
   )
   throw ()
   {
   std::cout << "black_jack: A simple blackjack with only one player and a dealer" << std::endl;
   std::cout << "Rules:" << std::endl;
   std::cout << "\t- Dealer must hit on soft 17 or lower." << std::endl;
   std::cout << "\t- Dealer must stand on soft 18, hard 17 or higher." << std::endl;
   std::cout << "\t- If both dealer and user have blackjack (a natural), the hand is a tie (push)."
   		<< std::endl;
   std::cout << "\t- Dealer's blackjack and non-natural 21 hand (no Ace-Face card pair) wins over\n" <<
   				 "\t  user non-natural 21." << std::endl;
   std::cout << "\t- Player's blackjack wins over dealer's non-natural 21." << std::endl;
   std::cout << std::endl;
   std::cout << "Options:" << std::endl;
   std::cout << "   -h, --help" << std::endl;
   std::cout << "      Print this help." << std::endl;

   std::cout << std::endl;
   return;
   }
/* Top level and binary entry point for black jack game
 * @return: 0 - Success exiting black jack game. Otherwise,
 * 			Error
 *
 * */
int main
	(
   int   argc,          // Argument count
   char  **argv         // Argument strings
   )
	{
	int ret = -1; // Assume game exits with error
	bool exit = false;
	signed char    oc;

	while ((oc = getopt_long(argc, argv, optstr, opttab, NULL)) != -1)
		{
		switch (oc)
			{
			case 'h':      // use config file argued
				usage();
				ret = 0;
			case '?':
			default:       // invalid option
				return ret;
				break;
			}
		}

	try
		{
		TBlackjack bljck;;

		log (LOG_INFO, "\n\n****Welcome to virtual blackjack! Get ready to start****\n\n");
		TCards cardDeck;
		do
			{
			// Play hand
			bljck.playHand(cardDeck);
			bljck.printStats();
			std::string reqStr("Continue game?\n Please enter 'Y' to continue or 'N' "
					"to exit game\n");
			TReqInputRets retType = reqInput(reqStr, "N", "Y", MAX_VALID_INPUT_REQ_TRIES);
			if ((retType == REQ_INPUT_ERR) || (retType == REQ_INPUT_EXIT))
				{
				exit = true;
				log(LOG_ERR, "Exiting game\n");
				}
			else if (retType == REQ_INPUT_CONT)
				log(LOG_INFO, "Get ready to play next hand\n");
			else
				{
				std::stringstream ss;
				ss << "Internal error, incorrect value (" << (unsigned int) retType <<
						") returned from processing request input \n" << std::endl;
				log(LOG_ERR,  ss.str());
				}

			}while (!exit);
		ret = 0;

		}
	catch (std::exception& err)
		{
		std::stringstream ss;
		ss << "Virtual Blackjack execution error" << err.what() <<
						". Exiting application\n" << std::endl;
		log(LOG_ERR, ss.str());
		}
	catch (...)
		{
		std::stringstream ss;
		ss << "Virtual Blackjack uknown execution error" <<
				". Exiting application\n" << std::endl;
		log(LOG_INFO, ss.str());
		}

	// Black jack object destructor is called when main exits
	return ret;
	}
