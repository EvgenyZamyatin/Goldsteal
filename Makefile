CC=C:\mingw32\bin\g++.exe
BOOST_PATH=C:\boost_1_58_0
INCLUDES=dependencies/includes/
LIBS=dependencies/libs/
FLAGS= -std=c++11 -I$(BOOST_PATH) -I$(INCLUDES) -L$(LIBS)

all: main                                                 

main: LevelLoader.o main.o
	$(CC) -o out/main.exe main.o LevelLoader.o $(FLAGS) -lhgehelp -lhge -ltmx -ltinyxml2 -lboost_filesystem -lboost_system -lz

main.o: src/main.cpp
	$(CC) -c src/main.cpp $(FLAGS)

LevelLoader.o: src/utils/LevelLoader.cpp
	$(CC) -c -std=c++11 src/utils/LevelLoader.cpp $(FLAGS)

tests: src/geometry/tests.cpp Geometry.o
	$(CC) -o out/tests.exe tests.cpp Geometry.o $(FLAGS)

Geometry.o: src/geometry/Geometry.cpp
	$(CC) -c -std=c++11 src/geometry/Geometry.cpp $(FLAGS)


clean:
	cmd //C del *.exe
	cmd //C del *.o
