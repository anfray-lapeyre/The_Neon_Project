#ifndef _OBJMESH_HPP
#define _OBJMESH_HPP

#define GLEW_STATIC
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
// GL Includes
#include <GL/glew.h>
#include <GL/glu.h>

#include <glimac/common.hpp>
#include <glimac/glm.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace std;

namespace glimac {
	
	struct Texture {
		GLuint id;
		string type;
		aiString path;
	};

	class OBJMesh
	{
		/*
		Classe OBJMesh : définition d'un modèle statique.
		*/
		private: 
			GLuint VAO, VBO, EBO;
			void setupMesh();
		public:
			vector<glimac::ShapeVertex> vertices;
			vector<GLuint> indices;
			vector<Texture> textures;
			
			// Constructeur, prend en argument le nom du modèle à charger 

			//OBJMesh(std::string);
			//Vrai constructeur copie
			OBJMesh(OBJMesh& other){
				this->VAO=other.getVAO();
				this->VBO=other.getVBO();
				this->EBO=other.getEBO();
				this->vertices = other.vertices;
				this->indices = other.indices;
				this->textures = other.textures;
			};
			
			
			OBJMesh(OBJMesh* other){
				this->VAO=other->getVAO();
				this->VBO=other->getVBO();
				this->EBO=other->getEBO();
				this->vertices = other->vertices;
				this->indices = other->indices;
				this->textures = other->textures;
			};
			
			// Constructeur copie
			OBJMesh(vector<ShapeVertex> vertices, vector<GLuint> indices, vector<Texture> textures)
			{
				this->vertices = vertices;
				this->indices = indices;
				this->textures = textures;

				// Now that we have all the required data, set the vertex buffers and its attribute pointers.
				this->setupMesh();
			}
			
			
			
			GLuint getVAO(){return VAO;}
			GLuint getVBO(){return VBO;}
			GLuint getEBO(){return EBO;}
			// Render the mesh
			void Draw(GLuint ID);
			
	};


}

#endif