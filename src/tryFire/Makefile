all: main.o
	g++ -o main.exe main.o -L../../dependencies/libs/ -lhgehelp -lhge

main.o: main.cpp
	g++ -c -O2 -std=c++11 -I../../dependencies/includes/ main.cpp 