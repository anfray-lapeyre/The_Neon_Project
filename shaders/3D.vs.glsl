#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normals;
layout(location = 2) in vec2 texCoords;

uniform mat4 uMVPMatrix;
uniform mat4 uMVMatrix;
uniform mat4 uNormalMatrix;
uniform sampler2D uTexture;
uniform sampler2D uTexture2;

out vec3 vPosition_vs;
out vec3 vNormals_vs;
out vec2 vTexCoords_vs;


void main() {

	vPosition_vs=(uMVMatrix * vec4(position,1)).xyz;
	vNormals_vs= (uNormalMatrix* vec4(normals,0)).xyz;
	vTexCoords_vs= texCoords;
	gl_Position= uMVPMatrix*vec4(position,1);


}
