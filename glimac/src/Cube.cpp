#include <cmath>
#include <vector>
#include <iostream>
#include "glimac/common.hpp"
#include "glimac/Cube.hpp"
#include <glimac/glm.hpp>

using namespace std;
using namespace glm;

namespace glimac {


    Cube::Cube() {
        this->m_nVertexCount=36;
        ShapeVertex vertex;

        vertex.position=vec3(-1.0f,-1.0f,-1.0f);
        vertex.normal=vec3(1.0f,1.0f,1.0f);
        vertex.texCoords=vec2(0.f,1.f);
        this->m_Vertices.push_back(vertex);
        vertex.position=vec3(-1.0f,-1.0f, 1.0f);
        vertex.texCoords=vec2(0.f,0.f);
        this->m_Vertices.push_back(vertex);
        vertex.position=vec3(-1.0f, 1.0f, 1.0f);
        vertex.texCoords=vec2(1.f,0.f);
        this->m_Vertices.push_back(vertex);


        vertex.position=vec3( -1.0f,-1.0f,-1.0f);
        vertex.normal=vec3(0.0f,0.f,1.0f);
        vertex.texCoords=vec2(0.f,1.f);
        this->m_Vertices.push_back(vertex);
        vertex.position=vec3(-1.0f, 1.0f, 1.0f);
        vertex.texCoords=vec2(1.f,0.f);
        this->m_Vertices.push_back(vertex);
        vertex.position=vec3(-1.0f, 1.0f,-1.0f);
        vertex.texCoords=vec2(1.f,1.f);
        this->m_Vertices.push_back(vertex);

        /***/

        vertex.position=vec3(1.0f, 1.0f,-1.0f);
        vertex.normal=vec3(1.0f,0.0f,0.0f);
        vertex.texCoords=vec2(1.f,0.f);
        this->m_Vertices.push_back(vertex);
        vertex.position=vec3(-1.0f,-1.0f,-1.0f);
        vertex.texCoords=vec2(0.f,1.f);
        this->m_Vertices.push_back(vertex);
        vertex.position=vec3(-1.0f, 1.0f,-1.0f);
        vertex.texCoords=vec2(1.f,1.f);
        this->m_Vertices.push_back(vertex);

        vertex.position=vec3(1.0f, 1.0f,-1.0f);
        vertex.normal=vec3(1.0f,0.f,1.0f);
        vertex.texCoords=vec2(1.f,0.f);
        this->m_Vertices.push_back(vertex);
        vertex.position=vec3(1.0f,-1.0f,-1.0f);
        vertex.texCoords=vec2(0.f,0.f);
        this->m_Vertices.push_back(vertex);
        vertex.position=vec3(-1.0f,-1.0f,-1.0f);
        vertex.texCoords=vec2(0.f,1.f);
        this->m_Vertices.push_back(vertex);

        /***/

        vertex.position=vec3(1.0f,-1.0f, 1.0f);
        vertex.normal=vec3(1.0f,1.0f,0.0f);
        vertex.texCoords=vec2(1.f,0.f);
        this->m_Vertices.push_back(vertex);
        vertex.position=vec3(-1.0f,-1.0f,-1.0f);
        vertex.texCoords=vec2(0.f,1.f);
        this->m_Vertices.push_back(vertex);
        vertex.position=vec3(1.0f,-1.0f,-1.0f);
        vertex.texCoords=vec2(0.f,0.f);
        this->m_Vertices.push_back(vertex);

        vertex.position=vec3( 1.0f,-1.0f, 1.0f);
        vertex.normal=vec3(0.0f,1.f,1.0f);
        vertex.texCoords=vec2(1.f,0.f);
        this->m_Vertices.push_back(vertex);
        vertex.position=vec3(-1.0f,-1.0f, 1.0f);
        vertex.texCoords=vec2(1.f,1.f);
        this->m_Vertices.push_back(vertex);
        vertex.position=vec3(-1.0f,-1.0f,-1.0f);
        vertex.texCoords=vec2(0.f,1.f);
        this->m_Vertices.push_back(vertex);

        /***/

        vertex.position=vec3(-1.0f, 1.0f, 1.0f);
        vertex.normal=vec3(0.5f,0.2f,0.5f);
        vertex.texCoords=vec2(1.f,0.f);
        this->m_Vertices.push_back(vertex);
        vertex.position=vec3(-1.0f,-1.0f, 1.0f);
        vertex.texCoords=vec2(0.f,0.f);
        this->m_Vertices.push_back(vertex);
        vertex.position=vec3(1.0f,-1.0f, 1.0f);
        vertex.texCoords=vec2(0.f,1.f);
        this->m_Vertices.push_back(vertex);

        vertex.position=vec3(1.0f, 1.0f, 1.0f);
        vertex.normal=vec3(0.f,0.3f,0.9f);
        vertex.texCoords=vec2(1.f,1.f);
        this->m_Vertices.push_back(vertex);
        vertex.position=vec3(-1.0f, 1.0f, 1.0f);
        vertex.texCoords=vec2(1.f,0.f);
        this->m_Vertices.push_back(vertex);
        vertex.position=vec3(1.0f,-1.0f, 1.0f);
        vertex.texCoords=vec2(0.f,1.f);
        this->m_Vertices.push_back(vertex);

        /***/

        vertex.position=vec3(1.0f,-1.0f,-1.0f);
        vertex.normal=vec3(0.f,1.f,0.5f);
        vertex.texCoords=vec2(0.f,1.f);
        this->m_Vertices.push_back(vertex);
        vertex.position=vec3(1.0f, 1.0f, 1.0f);
        vertex.texCoords=vec2(1.f,0.f);
        this->m_Vertices.push_back(vertex);
        vertex.position=vec3(1.0f,-1.0f, 1.0f);
        vertex.texCoords=vec2(0.f,0.f);
        this->m_Vertices.push_back(vertex);


        vertex.position=vec3( 1.0f, 1.0f, 1.0f);
        vertex.normal=vec3(0.f,1.f,0.f);
        vertex.texCoords=vec2(1.f,0.f);
        this->m_Vertices.push_back(vertex);
        vertex.position=vec3( 1.0f,-1.0f,-1.0f);
        vertex.texCoords=vec2(0.f,1.f);
        this->m_Vertices.push_back(vertex);
        vertex.position=vec3( 1.0f, 1.0f,-1.0f);
        vertex.texCoords=vec2(1.f,1.f);
        this->m_Vertices.push_back(vertex);


        vertex.position=vec3(1.0f, 1.0f, 1.0f);
        vertex.normal=vec3(0.8f,0.f,0.5f);
        vertex.texCoords=vec2(1.f,0.f);
        this->m_Vertices.push_back(vertex);
        vertex.position=vec3(1.0f, 1.0f,-1.0f);
        vertex.texCoords=vec2(0.f,0.f);
        this->m_Vertices.push_back(vertex);
        vertex.position=vec3(-1.0f, 1.0f,-1.0f);
        vertex.texCoords=vec2(0.f,1.f);
        this->m_Vertices.push_back(vertex);


        
        vertex.position=vec3(1.0f, 1.0f, 1.0f);
        vertex.normal=vec3(0.2f,0.7f,0.5f);
        vertex.texCoords=vec2(1.f,0.f);
        this->m_Vertices.push_back(vertex);
        vertex.position=vec3(-1.0f, 1.0f,-1.0f);
        vertex.texCoords=vec2(0.f,1.f);
        this->m_Vertices.push_back(vertex);
        vertex.position=vec3(-1.0f, 1.0f, 1.0f);
        vertex.texCoords=vec2(1.f,1.f);
        this->m_Vertices.push_back(vertex);

        


        

    }


}

//void Cube::build(GLfloat r, GLsizei discLat, GLsizei discLong) {
//}

