
#include "glimac/OBJMesh.hpp"

using namespace glm;
using namespace std;


namespace glimac {
			
			void OBJMesh::setupMesh()
			{
				// Create buffers/arrays
				glGenVertexArrays(1, &this->VAO);
				glGenBuffers(1, &this->VBO);
				
				// Load data into vertex buffers
				glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
				// A great thing about structs is that their memory layout is sequential for all its items.
				// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
				// again translates to 3/2 floats which translates to a byte array.
				glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(ShapeVertex), &this->vertices[0], GL_STATIC_DRAW);  
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
			
			

			
			
			
			
			
			// Render the mesh
			void OBJMesh::Draw(GLuint ID) 
			{
				// Bind appropriate textures

				// Draw mesh
				glBindVertexArray(this->VAO);
				glDrawArrays(GL_LINES,0, this->vertices.size());
				glBindVertexArray(0);
			}

	
	
	
}
