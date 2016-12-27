#version 330

in vec3 vPosition_vs;
in vec3 vNormals_vs;
in vec2 vTexCoords_vs;

uniform sampler2D uTexture;

out vec3 fFragColor;


void main() {

	fFragColor = vec3(1.f);//texture(uTexture,vTexCoords_vs).xyz;


}