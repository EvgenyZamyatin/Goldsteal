CC=C:\mingw32\bin\g++.exe
BOOST_PATH=C:\boost_1_55_0
HGE_PATH=C:\hge181\include
HGE_LIB=C:\hge181\lib\gcc
FLAGS= -std=c++11 -O2 -I$(BOOST_PATH) -L$(BOOST_PATH) -I$(HGE_PATH) -L$(HGE_LIB) -lhgehelp -lhge 

all: main
                     
main: Geometry.o
	$(CC) -o out/main.exe src/main.cpp Geometry.o $(FLAGS)

tests: Geometry.o tests.o
	$(CC) -o out/tests.exe tests.o Geometry.o $(FLAGS)

tests.o:
	$(CC) -c src/geometry/tests.cpp -std=c++11 $(FLAGS)

Geometry.o:
	$(CC) -c src/geometry/Geometry.cpp $(FLAGS)

clean:
	cmd //C del *.exe
	cmd //C del *.o
