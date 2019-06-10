# This file is the compilation rule master file

CPP=g++
LD=g++
CFLAGS=-std=c++11
LFLAGS=
OBJS=main.o dealer.o blackjack.o misc.o
BIN=blackjack

all : $(BIN)

# Link the objects and libraries into the final program.
$(BIN) : $(OBJS)
	$(LD) $(LFLAGS) -o $@ $(OBJS)
	@echo
	
clean:
	rm *.o
	@echo

# Build the object files (*.o) from the source files (*.c).
%.o : %.c
	$(CPP) $(CFLAGS) -c -o $@ $<
	@echo
%.o : %.cpp
	$(CPP) $(CFLAGS) -c -o $@ $<
	@echo
