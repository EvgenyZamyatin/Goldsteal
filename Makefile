CC=C:\mingw32\bin\g++.exe
BOOST_PATH=C:\boost_1_58_0
INCLUDES=dependencies/includes/
LIBS=dependencies/libs/
FLAGS= -std=c++11 -I$(BOOST_PATH) -I$(INCLUDES) -L$(LIBS)

all: main

main: Camera.o Geometry.o Render main.o Environment.o IObject.o SimpleObstacle.o GameState.o
	$(CC) -o out/main.exe GameState.o Camera.o Geometry.o RenderEnvironment.o RenderSimpleObstacle.o main.o Environment.o IObject.o SimpleObstacle.o $(FLAGS) -lhgehelp -lhge -ltmx -ltinyxml2 -lboost_filesystem -lboost_system -lz

GameState.o: src/model/GameState.h src/model/GameState.cpp
	$(CC) -c src/model/GameState.cpp $(FLAGS)
		
Environment.o: src/model/Environment.h src/model/Environment.cpp
	$(CC) -c src/model/Environment.cpp $(FLAGS)
                                           
IObject.o: src/model/IObject.h src/model/IObject.cpp
	$(CC) -c src/model/IObject.cpp $(FLAGS)

SimpleObstacle.o: src/model/SimpleObstacle.h src/model/SimpleObstacle.cpp
	$(CC) -c src/model/SimpleObstacle.cpp $(FLAGS)

Geometry.o: src/geometry/Geometry.cpp src/geometry/Geometry.h
	$(CC) -c -std=c++11 src/geometry/Geometry.cpp $(FLAGS)

Camera.o: src/model/Camera.cpp src/model/Camera.h
	$(CC) -c -std=c++11 src/model/Camera.cpp $(FLAGS)

main.o: src/main.cpp
	$(CC) -c src/main.cpp $(FLAGS)

Render: RenderEnvironment.o RenderSimpleObstacle.o RenderBody.o


RenderBody.o: src/render/RenderBody.cpp
	$(CC) -c -std=c++11 src/render/RenderBody.cpp $(FLAGS)

RenderEnvironment.o: src/render/RenderEnvironment.cpp
	$(CC) -c -std=c++11 src/render/RenderEnvironment.cpp $(FLAGS)

RenderSimpleObstacle.o:  src/render/RenderSimpleObstacle.cpp
	$(CC) -c -std=c++11 src/render/RenderSimpleObstacle.cpp $(FLAGS)

tests: src/geometry/tests.cpp Geometry.o
	$(CC) -o out/tests.exe tests.cpp Geometry.o $(FLAGS)

clean:
	cmd //C del *.o

