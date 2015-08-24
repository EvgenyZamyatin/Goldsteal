cmake_minimum_required(VERSION 2.8)

project(Goldsteal)
add_definitions(-DDEBUG)
add_definitions(-DHGE_DIRECTX_VER=9)

include_directories("C:/boost_1_58_0")
include_directories("C:/Users/Jeka/GameDev/Goldsteal/dependencies/includes/")
link_directories("C:/Users/Jeka/GameDev/Goldsteal/dependencies/libs/")

set(CMAKE_CXX_FLAGS "-O3 -std=c++11")

set(RENDER src/render/RenderBody.cpp src/render/RenderEnvironment.cpp src/render/RenderGameState.cpp src/render/RenderLightSource.cpp src/render/RenderSimpleObject.cpp)

set(MODEL src/model/Environment.cpp src/model/GameState.cpp src/model/IBody.cpp 
			src/model/IObject.cpp src/model/FlashLight.cpp 
				src/model/LightSource.cpp src/model/SimpleObject.cpp src/model/Camera.cpp )

set(BRAINS src/model/brains/IBrain.cpp src/model/brains/BrainPlayerInput.cpp)

set(MICROPATHER src/micropather.cpp)  

set(SOURCE ${BRAINS} ${MODEL} ${RENDER} ${MICROPATHER})

add_executable(main src/InputData.cpp src/main.cpp ${SOURCE})
#add_executable(geo_test ${GEOMETRY} src/geometry/tests.cpp)

target_link_libraries(main boost_filesystem boost_system hgehelp hge tmx tinyxml2 z)
