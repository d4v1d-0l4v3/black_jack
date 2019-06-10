/******************************************************************************/
/*!
 * @file:					  misc.hpp
 * @author  Original:     David Olave
 * @date    Created:      August 19th, 2017
 *
 *  @Description: Implements miscellaneous common functionality
 *
 *  Copyright David Olave 2017
 ******************************************************************************/

/* C++ Library includes */
#include <string>
#include <iostream>
#include <sstream>
#include <boost/algorithm/string.hpp>

/* Local includes */
#include "misc.hpp"

/*
 * Prints messages for human readability
 * @return: number of characters printed
 *
 * @note: Priority argument are not used currently
 */
unsigned int
	log
	(
	TLogPrio prio,				// Log priority
	const std::string &logStr		// Message to print
	)
	{
	printf("%s", logStr.c_str());
	return logStr.size();
	}

/*
 * Request input from two provided options
 * When input matches exit string, return or max
 * tries
 * @return: - REQ_INPUT_EXIT - User input matches requested exit
 * 			- REQ_INPUT_CONT - User input matches requested continue
 * 			- REQ_INPUT_ERR  - I/O When requested input
 */
TReqInputRets
	reqInput
	(
	const std::string &reqStr,     // Header string. Human readable question
	const std::string &exitStr,	 // String to be matched with input to exit routine
	const std::string &continueStr,// String to be matched with input to continue the routine below
	unsigned int max_tries			 // Max number of attempts before exitting with error
	)
	{
	TReqInputRets ret = REQ_INPUT_ERR;  // Assume error processing input
	bool success = true;  // Assume success entering user input
	//  Tries to allow system to received bad input before exiting
	unsigned int attempts = 0;
	std::string upExitStr = boost::to_upper_copy<std::string>(exitStr);
	std::string upContinStr = boost::to_upper_copy<std::string>(continueStr);
	do
	{
	std::string inStr;

	log(LOG_INFO, reqStr);

	std::cin >> inStr;
	std::string upInStr = boost::to_upper_copy<std::string>(inStr);
	if (upInStr == upExitStr)
		{
		success = true;
		ret = REQ_INPUT_EXIT;
		}
	else if (upInStr == upContinStr)
		{
		success = true;
		ret = REQ_INPUT_CONT;
		}
	else
		{
		success = false;
		log(LOG_INFO, "\nUnrecognized input. Permitted only 'Y' or 'N'\n\n");
		sleep(1);   // Give user time to recognize error
		}
	attempts++;
	} while  (!success && (attempts < max_tries));

	if (attempts >= max_tries)
		{
		printf("max tries = %d\n", max_tries);
		std::stringstream ss;
		ss << "\nError, max attempts (" << attempts << ") to received valid input from "
				"user has been reached.\n" << std::endl;
		log(LOG_ERR, ss.str());
		ret = REQ_INPUT_ERR;
		}
	return ret;
	}
