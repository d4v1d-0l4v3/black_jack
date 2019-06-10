/******************************************************************************/
/*!
 * @file:					  misc.hpp
 * @author  Original:     David Olave
 * @date    Created:      August 19th, 2017
 *
 *  @Description: The header file miscellaneous common operations
 *
 *  Copyright David Olave 2017
 ******************************************************************************/

#ifndef __MISC__
#define __MISC__

/* C++ Library includes */
#include <string>

/* Defines */

//  Max attempts to received valid input from user has been reached
#define MAX_VALID_INPUT_REQ_TRIES   10

/* Enumerations, type defines */
typedef enum __logPrio__
	{
	LOG_DEBUG,
	LOG_INFO,
	LOG_WARN,
	LOG_ERR
	}TLogPrio;

typedef enum __ReqInputRets__
	{
	REQ_INPUT_EXIT, // User input matches requested exit
	REQ_INPUT_CONT, // User input matches requested continue
	REQ_INPUT_ERR 	 // I/O When requested input
	}TReqInputRets;

/*
 * Prints messages for human readability
 * @return: number of characters printed
 */
unsigned int log(TLogPrio prio, const std::string &logStr);

/*
 * Request input from two provided options
 * When input matches exit string, return or max
 * tries
 * @return: - REQ_INPUT_EXIT - User input matches requested exit
 * 			- REQ_INPUT_CONT - User input matches requested continue
 * 			- REQ_INPUT_ERR  - I/O When requested input
 */
TReqInputRets reqInput (const std::string &reqStr, const std::string &exitStr,
		const std::string &continueStr, unsigned int max_tries);

#endif
