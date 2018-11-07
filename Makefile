
output: main.o AccessControlMatrix.o
	g++ main.o AccessControlMatrix.o -o output

main.o: main.cpp
	g++ -c main.cpp

AccessControlMatrix.o: subject.o AccessControlMatrix.cpp AccessControlMatrix.h
	g++ -c AccessControlMatrix.cpp

subject.o: subject.cpp subject.h
	g++ -c subject.cpp

clean:
	rm *.o output