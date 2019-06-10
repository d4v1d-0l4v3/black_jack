# black_jack
Basic implementation of Black Jack game,

Date:		August 24th 2017
Author:		David Olave

Virtual Blackjack is a software simulation of the game Blackjack.

Formulated conventions to read this document:
	- User commands are enclosed by single quotes (ex: './command').
	
1. How to install and run virtual blackjack:

	a. Install boost library if not previously installed. Ex: For Ubuntu (linux) distribution, type:

		'sudo apt-get install libboost-all-dev'
		
	b. Clone black_jack from Github repository (git tool must be installed):
	
		'git clone https://github.com/d4v1d-0l4v3/black_jack.git'

	c. At the top level directory, type the utility command
	
		'make'
	
	d. 'make' will compile and generate the binary 'blackjack'.
	
	e. Run virtual blackjack by typing at the same top folder directory the command:
	
		'./blackjack'
		
	f. And follow the game instructions.
		
	g. For information about Virtual Blackjack specific rules type:
		
		'./blackjack -h'
		
2. Compatibility:

	a. The project software has been compiled and run only on an Ubuntu 14.04 LTS 
	distribution hosted by an Intel Core i7 chip set (x86_64) with a 64 bit cpu architecture.
	
	b. The software must be compiled with a C++11 or newer compatible C++ standard compilers.
	
	c. The C++11 compatible compiler must include the "boost" library.
	
Any questions, comments or discovered bugs, please contact the author at david.olave@gmail.com.
