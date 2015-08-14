CC=C:\mingw\bin\g++.exe
BOOST_PATH=C:\boost_1_58_0
INCLUDES=dependencies/includes/
LIBS=dependencies/libs/
FLAGS= -DHGE_DIRECTX_VER=9 -std=c++11 -O3 -D DEBUG -I$(BOOST_PATH) -I$(INCLUDES) -L$(LIBS)

all: main

main: Camera.o GGeometry Render main.o Environment.o IObject.o SimpleObject.o GameState.o Brains InputData.o IBody.o LightSource.o
	$(CC) -o out/main.exe GameState.o Camera.o Geometry.o VisibilityPolygon.o RenderEnvironment.o RenderSimpleObject.o RenderBody.o RenderGameState.o RenderLightSource.o LightSource.o main.o Environment.o IObject.o SimpleObject.o InputData.o IBrain.o IBody.o BrainPlayerInput.o $(FLAGS) -lhgehelp -lhge -ltmx -ltinyxml2 -lboost_filesystem -lboost_system -lz

LightSource.o: src/model/LightSource.cpp src/model/LightSource.h
	$(CC) -c src/model/LightSource.cpp $(FLAGS)

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

SimpleObject.o: src/model/SimpleObject.h src/model/SimpleObject.cpp
	$(CC) -c src/model/SimpleObject.cpp $(FLAGS)

Camera.o: src/model/Camera.cpp src/model/Camera.h
	$(CC) -c -std=c++11 src/model/Camera.cpp $(FLAGS)

main.o: src/main.cpp
	$(CC) -c src/main.cpp $(FLAGS)

Render: RenderEnvironment.o RenderSimpleObject.o RenderBody.o RenderGameState.o RenderLightSource.o


RenderBody.o: src/render/RenderBody.cpp
	$(CC) -c -std=c++11 src/render/RenderBody.cpp $(FLAGS)

RenderEnvironment.o: src/render/RenderEnvironment.cpp
	$(CC) -c -std=c++11 src/render/RenderEnvironment.cpp $(FLAGS)

RenderSimpleObject.o:  src/render/RenderSimpleObject.cpp
	$(CC) -c -std=c++11 src/render/RenderSimpleObject.cpp $(FLAGS)

RenderGameState.o:  src/render/RenderGameState.cpp
	$(CC) -c -std=c++11 src/render/RenderGameState.cpp $(FLAGS)

RenderLightSource.o: src/render/RenderLightSource.cpp
	$(CC) -c -std=c++11 src/render/RenderLightSource.cpp $(FLAGS)

GGeometry: src/geometry/* Geometry.o VisibilityPolygon.o

Geometry.o: src/geometry/Geometry.cpp src/geometry/Geometry.h
	$(CC) -c -std=c++11 src/geometry/Geometry.cpp $(FLAGS)

VisibilityPolygon.o: src/geometry/VisibilityPolygon.cpp src/geometry/Geometry.h
	$(CC) -c -std=c++11 src/geometry/VisibilityPolygon.cpp $(FLAGS)

tests: src/geometry/tests.cpp GGeometry
	$(CC) -o out/tests.exe src/geometry/tests.cpp VisibilityPolygon.o Geometry.o $(FLAGS)




clean:
	cmd //C del *.o

