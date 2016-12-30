
#include "glimac/Model.hpp"

using namespace glm;
using namespace std;


namespace glimac {
			
	void Model::Draw()
	{
		int size=0;
		for(int i=0;i<this->meshes.size();i++){
			size+=this->meshes[i].size();
		}
		glBindVertexArray(this->VAO);
		glDrawArrays(GL_LINES,0, size);
		glBindVertexArray(0);
	}  

	
	void Model::InitMesh(){
		// Create buffers/arrays
		glGenVertexArrays(1, &this->VAO);
		glGenBuffers(1, &this->VBO);
				
		// Load data into vertex buffers
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		// A great thing about structs is that their memory layout is sequential for all its items.
		// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
		// again translates to 3/2 floats which translates to a byte array.
		int size=0;
		for(int i=0;i<this->meshes.size();i++){
			size+=this->meshes[i].size()*sizeof(ShapeVertex);
		}
		glBufferData(GL_ARRAY_BUFFER, size, &this->meshes[0], GL_STATIC_DRAW);  
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(this->VAO);
				// Set the vertex attribute pointers
		glEnableVertexAttribArray(VERTEX_ATTRIB_POSITION);
		glEnableVertexAttribArray(VERTEX_ATTRIB_NORMALS);
		glEnableVertexAttribArray(VERTEX_ATTRIB_TEXTURE);
				
				
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glVertexAttribPointer(VERTEX_ATTRIB_POSITION,3,GL_FLOAT,GL_FALSE,sizeof(ShapeVertex), (const GLvoid *) (offsetof(ShapeVertex,position)));
		glVertexAttribPointer(VERTEX_ATTRIB_NORMALS,3,GL_FLOAT,GL_FALSE,sizeof(ShapeVertex), (const GLvoid *) (offsetof(ShapeVertex,normal)));
		glVertexAttribPointer(VERTEX_ATTRIB_TEXTURE,2,GL_FLOAT,GL_FALSE,sizeof(ShapeVertex), (const GLvoid *) (offsetof(ShapeVertex,texCoords)));

		glBindVertexArray(0);
		
	}



	
	/*  Functions   */
	void Model::loadModel(string path){
		// Read file via ASSIMP
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate |aiProcess_GenNormals | aiProcess_GenUVCoords);
        // Check for errors
        if(!scene) // if is Not Zero
        {
            cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
            return;
        }

        // Retrieve the directory path of the filepath
        this->directory = path.substr(0, path.find_last_of('/'));

        // Process ASSIMP's root node recursively
        this->processNode(scene->mRootNode, scene);

	}
	
	
	void Model::processNode(aiNode* node, const aiScene* scene){

		// Process each mesh located at the current node

        for(GLuint i = 0; i < scene->mNumMeshes; i++)
        {
            // The node object only contains indices to index the actual objects in the scene. 
            // The scene contains all the data, node is just to keep stuff organized (like relations between nodes).
			cout << "test0.5" << endl;
            aiMesh* mesh = scene->mMeshes[i]; 
cout << "test0.7" << endl;			
			//aiMaterial* m = scene->mMaterials[mesh->mMaterialIndex];
			// OBJMesh* test= this->processMesh(mesh, scene);
            // this->meshes.push_back(test);	
			cout << "test" << endl;
			if(mesh->mTextureCoords[0]==NULL)
				meshes.push_back(vertexFromBlankMesh(mesh));

			else
				meshes.push_back(vertexFromTexturedMesh(mesh));	
			cout << "test2" << endl;
        }

	}
	
			
	vector<ShapeVertex> Model::vertexFromTexturedMesh(aiMesh* mesh){
		vector<ShapeVertex> vertex;
		ShapeVertex vx;
		cout << "Ultime test" <<endl;
		for(unsigned j=0;j<mesh->mNumFaces;j++){
			aiFace* face = mesh->mFaces + j;
			for(unsigned k=0;k<3;k++){
				int indice = face->mIndices[k];
				vx.position =vec3(mesh->mVertices[indice].x,mesh->mVertices[indice].y,mesh->mVertices[indice].z);
				
				vx.texCoords=vec2(mesh->mTextureCoords[0][indice].x,mesh->mTextureCoords[0][indice].y);
				vx.normal=vec3(mesh->mNormals[indice].x,mesh->mNormals[indice].y,mesh->mNormals[indice].z);
				vertex.push_back(vx);
			}
		}
		cout << "Ultime test" <<endl;
		return vertex;
	}

	vector<ShapeVertex> Model::vertexFromBlankMesh(aiMesh* mesh){
		vector<ShapeVertex> vertex;
		ShapeVertex vx;
				cout << "Blanc test" <<endl;

		for(unsigned j=0;j<mesh->mNumFaces;j++){
			aiFace face = mesh->mFaces[j];
			
			for(unsigned k=0;k<3;k++){
				int indice = face.mIndices[k];
				cout << "Blanc test2" <<endl;
				vx.position =vec3(mesh->mVertices[indice].x,mesh->mVertices[indice].y,mesh->mVertices[indice].z);
				vx.texCoords=vec2(0,0);
				vx.normal=vec3(mesh->mNormals[indice].x,mesh->mNormals[indice].y,mesh->mNormals[indice].z);
				vertex.push_back(vx);
			}
		}
		cout << "Blanc test3" <<endl;
		return vertex;
	}

}
