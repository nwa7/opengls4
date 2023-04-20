#version 330 core

//in vec3 vPosition;
//in vec3 vNormal;
in vec2 vTex;
uniform sampler2D uTex;
uniform sampler2D uTex2;

out vec3 coordText;

void main() {
    vec4 tex1 = texture(uTex, vTex);
    vec4 tex2 = texture(uTex2, vTex);
    vec4 blended = mix(tex1, tex2, 0.5); // blend 50/50
    coordText = blended.xyz;
}
