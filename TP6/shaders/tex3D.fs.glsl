#version 330 core

//in vec3 vPosition;
//in vec3 vNormal;
in vec2 vTex;
uniform sampler2D uTex;

out vec3 coordText;

void main() {
    coordText = texture(uTex, vTex).xyz;
}
