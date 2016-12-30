#ifndef _MODEL_HPP
#define _MODEL_HPP

#define GLEW_STATIC
#include <GL/glew.h>
#include <cstdlib>
#include <cstring>
#include <cassert>

#include <string>
#include <vector>
#include <map>

#include <fstream>
#include <sstream>
#include <iostream>
#include <GL/glu.h>

/* assimp include files. These three are usually needed. */
#include <assimp/Importer.hpp>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glimac/FilePath.hpp>
#include <glimac/common.hpp>
#include <glimac/glm.hpp>
#include <glimac/OBJMesh.hpp>
#include <glimac/Image.hpp>
using namespace std;

namespace glimac {
	
	class Model 
	{
		
		private:
			/*  Model Data  */
			vector<vector<ShapeVertex>> meshes;
			string directory;
			GLuint VAO, VBO;
			/*  Functions   */
			void loadModel(string path);
			void processNode(aiNode* node, const aiScene* scene);
			vector<ShapeVertex> vertexFromBlankMesh(aiMesh* mesh);
			vector<ShapeVertex> vertexFromColoredMesh(aiMesh* mesh);
			vector<ShapeVertex> vertexFromTexturedMesh(aiMesh* mesh);
			void InitMesh();
		public:
			/*  Functions   */
			Model(string path)
			{
				this->loadModel(path);
			}
			Model(){};
			void Draw();	
			
	};
}

#endif