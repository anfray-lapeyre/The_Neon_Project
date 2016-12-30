#include <cmath>
#include <vector>
#include <iostream>
#include "glimac/common.hpp"
#include "glimac/Plane.hpp"
#include <glimac/glm.hpp>

using namespace std;
using namespace glm;

namespace glimac {


    Plane::Plane() {
        this->m_nVertexCount=36;
        ShapeVertex vertex;

        vertex.position=vec3(-1.0f,-1.0f,0.f);
        vertex.normal=vec3(1.0f,1.0f,1.0f);
        vertex.texCoords=vec2(0.f,1.f);
        this->m_Vertices.push_back(vertex);
        vertex.position=vec3(1.0f,-1.0f,0.f);
        vertex.texCoords=vec2(1.f,1.f);
        this->m_Vertices.push_back(vertex);
        vertex.position=vec3(-1.0f, 1.0f, 0.f);
        vertex.texCoords=vec2(0.f,0.f);
        this->m_Vertices.push_back(vertex);


        vertex.position=vec3( 1.0f,-1.0f,0.f);
        vertex.texCoords=vec2(1.f,1.f);
        this->m_Vertices.push_back(vertex);
        vertex.position=vec3(-1.0f, 1.0f, 0.f);
        vertex.texCoords=vec2(0.f,0.f);
        this->m_Vertices.push_back(vertex);
        vertex.position=vec3(1.0f, 1.0f,0.f);
        vertex.texCoords=vec2(1.f,0.f);
        this->m_Vertices.push_back(vertex);

        /***/
       

    }


}

