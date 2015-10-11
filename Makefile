# the compiler is g++
CC	= g++

# compiler flags:
# -O2	optimization flag
# -Wall	displays compiler warnings
FLAGS	= -O2 -Wall -w

TARGET	= sish

default: sish

sish: main.o Commands.o History.o SimpleShell.o SupportingFunctions.o
	$(CC) $(FLAGS) -o $(TARGET) main.o Commands.o History.o SimpleShell.o

main.o: main.cpp
	$(CC) $(FLAGS) -c main.cpp

Commands.o: Commands.cpp Commands.h
	$(CC) $(FLAGS) -c Commands.cpp Commands.h

History.o: History.cpp History.h
	$(CC) $(FLAGS) -c History.cpp History.h

SimpleShell.o: SimpleShell.cpp SimpleShell.h
	$(CC) $(FLAGS) -c SimpleShell.cpp SimpleShell.h

SupportingFunctions.o: SupportingFunctions.h
	$(CC) $(FLAGS) -c SupportingFunctions.h
clean:
	rm -f $(TARGET) *.o *~ *.gch
