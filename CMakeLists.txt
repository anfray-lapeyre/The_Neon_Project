project(The_Neon_Project) #name of the project
add_definitions(-std=c++11)

cmake_minimum_required(VERSION 2.8)
INCLUDE_DIRECTORIES(include)
file(GLOB_RECURSE SRC_FILES *.cpp *.h)
FILE(GLOB_RECURSE shaderFiles "./*.glsl")
add_custom_target(shaders SOURCES ${shaderFiles})
INCLUDE_DIRECTORIES(third-party/include)
INCLUDE_DIRECTORIES(glimac/include)
#find_package(ASSIMP REQUIRED)

add_subdirectory(glimac)


#FIND_LIBRARY(ASSIMP_LIB assimp PATHS third-party/assimp/lib)
#message(${ASSIMP_LIB})

aux_source_directory(./src SRC_LIST) #add all source file from current directory in the SRC_LIST variable

if (WIN32)
    #do something
	#set(LIB_LIST  glimac SDLmain SDL ${ASSIMP_LIB} SDL_mixer glew32s opengl32 glu32 freeglut ) #add the libraries options in the LIB_LIST variable
	set(LIB_LIST  glimac SDLmain SDL SDL_mixer glew32s opengl32 glu32 freeglut ) #add the libraries options in the LIB_LIST variable
endif (WIN32)

if (UNIX)
    #do something
	# set(LIB_LIST glimac SDL SDL_mixer ${ASSIMP_LIB} GLEW GL GLU glut) #add the libraries options in the LIB_LIST variable
	set(LIB_LIST glimac SDL SDL_mixer GLEW GL GLU glut) #add the libraries options in the LIB_LIST variable
endif (UNIX)


add_executable(${PROJECT_NAME} ${SRC_LIST}) #set an executable from the file of SRC_LIST
target_link_libraries(${PROJECT_NAME} ${LIB_LIST}) #link the libraries with the executable
