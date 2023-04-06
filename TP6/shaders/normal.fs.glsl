#version 330 core

in vec3 vPosition;
in vec3 vNormal;
in vec2 VTex;

out vec4 fFragColor;

void main() {
    fFragColor = vec4(normalize(vNormal), 1.0f);

}
