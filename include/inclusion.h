#ifndef INCLUSION_H_
#define INCLUSION_H_

#define GLEW_STATIC
#define M_PI 3.14159265358979323846
#define VITESSE_DEPLACEMENT 0.1
#define VITESSE_ROTATION 0.015

#define MIX_DEFAULT_FREQUENCY 22050
#define MIX_MAX_FREQUENCY 48000
//#define MIX_DEFAULT_CHANNELS 2 //2 = stereo 1= mono
#define MIX_DEFAULT_CHUNKSIZE 1024 //nombre de bits utilises pour les sons



#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <vector>
#include <string>
#include <memory>
#include <cstddef>
#include <math.h>


#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>
#include <glimac/glm.hpp>
#include <glimac/Image.hpp>
#include <glimac/OBJMesh.hpp>
#include <glimac/Cube.hpp>
#include <glimac/FreeflyCamera.hpp>
#include <glimac/SDLWindowManager.hpp>
#include <glimac/Model.hpp>


#define aisgl_min(x,y) (x<y?x:y)
#define aisgl_max(x,y) (y>x?y:x)


#endif