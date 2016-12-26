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
			vector<OBJMesh> meshes;
			string directory;
			vector<Texture> textures_loaded;
			/*  Functions   */
			void loadModel(string path);
			void processNode(aiNode* node, const aiScene* scene);
			OBJMesh processMesh(aiMesh* mesh, const aiScene* scene);
			vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
			
		public:
			/*  Functions   */
			Model(string path)
			{
				this->loadModel(path);
			}
			void Draw(GLuint ID);	

	};
	GLint TextureFromFile(const char* path, string directory);
}

#endif