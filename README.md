# black_jack
Basic implementation of Black Jack game,

Date:		August 24th 2017
Author:		David Olave

Virtual Blackjack is a software simulation of the game Blackjack.

Formulated conventions to read this document:
	- User commands are enclosed by single quotes (ex: './command').
	
1. How to install and run virtual blackjack:

	a. After extracting the compressed package, simply type at the extracted top level directory the utility command
	
		'make'
	
	b. 'make' will compile and generate the binary 'blackjack'.
	c. Run virtual blackjack by typing at the same top folder directory the command:
	
		'./blackjack'
		
	d. And follow the game instructions.
		
	e. For information about Virtual Blackjack specific rules type:
		
		'./blackjack -h'
		
2. Compatibility:

	a. The project software has been compiled and run only on an Ubuntu 14.04 LTS 
	distribution hosted by an Intel Core i7 chip set (x86_64) with a 64 bit cpu architecture.
	b. The software must be compiled with a C++11 or newer compatible C++ standard compilers.
	c. The C++11 compatible compiler must include the "boost" library.
	
Any questions, comments or discovered bugs, please contact the author at david.olave@gmail.com.
