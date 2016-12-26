#pragma once

#include <GL/glew.h>
#include "glm.hpp"
#define VERTEX_ATTRIB_POSITION 0
#define VERTEX_ATTRIB_NORMALS 1
#define VERTEX_ATTRIB_TEXTURE 2

namespace glimac {

struct ShapeVertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

}
