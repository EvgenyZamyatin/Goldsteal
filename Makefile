CC=C:\mingw32\bin\g++.exe
BOOST_PATH=C:\boost_1_55_0
HGE_PATH=C:\hge181\include
HGE_LIB=C:\hge181\lib\gcc
FLAGS= -std=c++11 -I$(BOOST_PATH) -L$(BOOST_PATH) -I$(HGE_PATH) -L$(HGE_LIB) -lhge -lhgehelp

all: tests main

main:
	$(CC) -o out/main.exe src/main.cpp $(FLAGS)

tests: Geometry.o tests.o
	$(CC) -o out/tests.exe tests.o Geometry.o $(FLAGS)

tests.o:
	$(CC) -c src/geometry/tests.cpp $(FLAGS)

Geometry.o:
	$(CC) -c src/geometry/Geometry.cpp $(FLAGS)

clean:
	cmd //C del *.o