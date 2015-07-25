CC=C:\mingw32\bin\g++.exe
BOOST_PATH=C:\boost_1_58_0
INCLUDES=dependencies/includes/
LIBS=dependencies/libs/
FLAGS= -std=c++11 -I$(BOOST_PATH) -I$(INCLUDES) -L$(LIBS)

all: main

main: Camera.o Geometry.o Render main.o Environment.o IObject.o SimpleObstacle.o GameState.o Brains InputData.o IBody.o
	$(CC) -o out/main.exe GameState.o Camera.o Geometry.o RenderEnvironment.o RenderSimpleObstacle.o RenderBody.o main.o Environment.o IObject.o SimpleObstacle.o InputData.o IBrain.o IBody.o BrainPlayerInput.o $(FLAGS) -lhgehelp -lhge -ltmx -ltinyxml2 -lboost_filesystem -lboost_system -lz

IBody.o: src/model/IBody.h src/model/IBody.cpp
	$(CC) -c src/model/IBody.cpp $(FLAGS)

Brains: IBrain.o BrainPlayerInput.o

IBrain.o: src/model/brains/IBrain.h src/model/brains/IBrain.cpp
	$(CC) -c src/model/brains/IBrain.cpp $(FLAGS)

BrainPlayerInput.o: src/model/brains/BrainPlayerInput.h src/model/brains/BrainPlayerInput.cpp
	$(CC) -c src/model/brains/BrainPlayerInput.cpp $(FLAGS)

InputData.o: src/InputData.h src/InputData.cpp
	$(CC) -c src/InputData.cpp $(FLAGS)

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

