
output: main.o AccessControlMatrix.o
	g++ main.o AccessControlMatrix.o -o output

main.o: main.cpp
	g++ -c main.cpp

AccessControlMatrix.o: AccessControlMatrix.cpp AccessControlMatrix.h
	g++ -c AccessControlMatrix.cpp

clean:
	rm *.o output