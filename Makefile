CC=C:\mingw32\bin\g++.exe
BOOST_PATH=C:\boost_1_58_0
INCLUDES=dependencies/includes/
LIBS=dependencies/libs/
FLAGS= -std=c++11 -I$(BOOST_PATH) -I$(INCLUDES) -L$(LIBS)

all: main

main: LevelLoader.o Camera.o Geometry.o Renderer main.o
	$(CC) -o out/main.exe main.o LevelLoader.o Camera.o Geometry.o RendererEnvironment.o RendererSimpleObstacle.o $(FLAGS) -lhgehelp -lhge -ltmx -ltinyxml2 -lboost_filesystem -lboost_system -lz
	
Renderer: RendererEnvironment.o RendererSimpleObstacle.o

main.o: src/main.cpp
	$(CC) -c src/main.cpp $(FLAGS)

LevelLoader.o: src/utils/LevelLoader.cpp src/utils/LevelLoader.h
	$(CC) -c -std=c++11 src/utils/LevelLoader.cpp $(FLAGS)
	   
RendererEnvironment.o: src/render/RendererEnvironment.cpp
	$(CC) -c -std=c++11 src/render/RendererEnvironment.cpp $(FLAGS)

RendererSimpleObstacle.o:  src/render/RendererSimpleObstacle.cpp
	$(CC) -c -std=c++11 src/render/RendererSimpleObstacle.cpp $(FLAGS)

Camera.o: src/model/Camera.cpp src/model/Camera.h
	$(CC) -c -std=c++11 src/model/Camera.cpp $(FLAGS)

tests: src/geometry/tests.cpp Geometry.o
	$(CC) -o out/tests.exe tests.cpp Geometry.o $(FLAGS)

Geometry.o: src/geometry/Geometry.cpp src/geometry/Geometry.h
	$(CC) -c -std=c++11 src/geometry/Geometry.cpp $(FLAGS)


clean:
	cmd //C del *.o
