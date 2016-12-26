
#include "glimac/OBJMesh.hpp"

using namespace glm;
using namespace std;


namespace glimac {
			
			void OBJMesh::setupMesh()
			{
				// Create buffers/arrays
				glGenVertexArrays(1, &this->VAO);
				glGenBuffers(1, &this->VBO);
				glGenBuffers(1, &this->EBO);

				glBindVertexArray(this->VAO);
				// Load data into vertex buffers
				glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
				// A great thing about structs is that their memory layout is sequential for all its items.
				// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
				// again translates to 3/2 floats which translates to a byte array.
				glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(ShapeVertex), &this->vertices[0], GL_STATIC_DRAW);  

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);

				// Set the vertex attribute pointers
				glEnableVertexAttribArray(VERTEX_ATTRIB_POSITION);
				glEnableVertexAttribArray(VERTEX_ATTRIB_NORMALS);
				glEnableVertexAttribArray(VERTEX_ATTRIB_TEXTURE);
				glVertexAttribPointer(VERTEX_ATTRIB_POSITION,3,GL_FLOAT,GL_FALSE,sizeof(ShapeVertex), (const GLvoid *) (offsetof(ShapeVertex,position)));
				glVertexAttribPointer(VERTEX_ATTRIB_NORMALS,3,GL_FLOAT,GL_FALSE,sizeof(ShapeVertex), (const GLvoid *) (offsetof(ShapeVertex,normal)));
				glVertexAttribPointer(VERTEX_ATTRIB_TEXTURE,2,GL_FLOAT,GL_FALSE,sizeof(ShapeVertex), (const GLvoid *) (offsetof(ShapeVertex,texCoords)));

				glBindVertexArray(0);
			}
			
			

			
			
			
			
			
			// Render the mesh
			void OBJMesh::Draw(GLuint ID) 
			{
				// Bind appropriate textures
				GLuint diffuseNr = 1;
				GLuint specularNr = 1;
				for(GLuint i = 0; i < this->textures.size(); i++)
				{
					glActiveTexture(GL_TEXTURE0 + i); // Active proper texture unit before binding
					// Retrieve texture number (the N in diffuse_textureN)
					stringstream ss;
					string number;
					string name = this->textures[i].type;
					if(name == "texture_diffuse")
						ss << diffuseNr++; // Transfer GLuint to stream
					else if(name == "texture_specular")
						ss << specularNr++; // Transfer GLuint to stream
					number = ss.str(); 
					// Now set the sampler to the correct texture unit
					glUniform1i(glGetUniformLocation(ID, (name + number).c_str()), i);
					// And finally bind the texture
					glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
				}
				
				// Also set each mesh's shininess property to a default value (if you want you could extend this to another mesh property and possibly change this value)
				glUniform1f(glGetUniformLocation(ID, "material.shininess"), 16.0f);

				// Draw mesh
				glBindVertexArray(this->VAO);
				glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
				glBindVertexArray(0);

				// Always good practice to set everything back to defaults once configured.
				for (GLuint i = 0; i < this->textures.size(); i++)
				{
					glActiveTexture(GL_TEXTURE0 + i);
					glBindTexture(GL_TEXTURE_2D, 0);
				}
			}

	
	
	
}