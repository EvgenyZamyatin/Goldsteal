CC=C:\mingw32\bin\g++.exe
BOOST_PATH=C:\boost_1_58_0
INCLUDES=dependencies/includes/
LIBS=dependencies/libs/
FLAGS= -std=c++11 -I$(BOOST_PATH) -I$(INCLUDES) -L$(LIBS)

all: main

main: LevelLoader.o Camera.o Geometry.o Render main.o Hero.o
	$(CC) -o out/main.exe main.o LevelLoader.o Camera.o Geometry.o RenderEnvironment.o RenderSimpleObstacle.o $(FLAGS) -lhgehelp -lhge -ltmx -ltinyxml2 -lboost_filesystem -lboost_system -lz
	
Render: RenderEnvironment.o RenderSimpleObstacle.o RenderBody.o

main.o: src/main.cpp
	$(CC) -c src/main.cpp $(FLAGS)

LevelLoader.o: src/utils/LevelLoader.cpp src/utils/LevelLoader.h
	$(CC) -c -std=c++11 src/utils/LevelLoader.cpp $(FLAGS)

Hero.o: src/model/Hero.h src/model/Hero.cpp
	$(CC) -c -std=c++11 src/model/Hero.cpp $(FLAGS)

RenderBody.o: src/render/RenderBody.cpp
	$(CC) -c -std=c++11 src/render/RenderBody.cpp $(FLAGS)

RenderEnvironment.o: src/render/RenderEnvironment.cpp
	$(CC) -c -std=c++11 src/render/RenderEnvironment.cpp $(FLAGS)

RenderSimpleObstacle.o:  src/render/RenderSimpleObstacle.cpp
	$(CC) -c -std=c++11 src/render/RenderSimpleObstacle.cpp $(FLAGS)

Camera.o: src/model/Camera.cpp src/model/Camera.h
	$(CC) -c -std=c++11 src/model/Camera.cpp $(FLAGS)

tests: src/geometry/tests.cpp Geometry.o
	$(CC) -o out/tests.exe tests.cpp Geometry.o $(FLAGS)

Geometry.o: src/geometry/Geometry.cpp src/geometry/Geometry.h
	$(CC) -c -std=c++11 src/geometry/Geometry.cpp $(FLAGS)


clean:
	cmd //C del *.o
