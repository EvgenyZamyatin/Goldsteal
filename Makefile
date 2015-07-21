CC=C:\mingw32\bin\g++.exe
BOOST_PATH=C:\boost_1_58_0
INCLUDES=dependencies/includes/
LIBS=dependencies/libs/
FLAGS= -std=c++11 -I$(BOOST_PATH) -I$(INCLUDES) -L$(LIBS) -lhgehelp -lhge -ltinyxml 

all: main

try: 
	$(CC) -o out/try.exe try.cpp $(FLAGS)
                                          
main1: Geometry.o
	$(CC) -o out/main.exe src/main.cpp Geometry.o $(FLAGS)

main: Geometry.o
	$(CC) -o out/main.exe src/main.cpp src/utils/LevelLoader.cpp src/model/Environment.cpp src/model/Camera.cpp Geometry.o $(FLAGS)

tests: Geometry.o tests.o
	$(CC) -o out/tests.exe tests.o Geometry.o $(FLAGS)

tests.o:
	$(CC) -c src/geometry/tests.cpp -std=c++11 $(FLAGS)

Geometry.o:
	$(CC) -c src/geometry/Geometry.cpp $(FLAGS)

clean:
	cmd //C del *.exe
	cmd //C del *.o
