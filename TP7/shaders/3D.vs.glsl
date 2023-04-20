#version 330 core

layout(location = 0) in vec3 aVertexPosition;
layout(location = 1) in vec3 aVertexNormal;
layout(location = 2) in vec2 aVertexTex;

uniform mat4 uMVPMatrix;
uniform mat4 uMVMatrix;
uniform mat4 uNormalMatrix;

out vec3 vPosition;
out vec3 vNormal;
out vec2 vTex;

void main() {    

  vPosition = aVertexPosition;
  vec4 vertexNormal = vec4(aVertexNormal, 0);
  vec4 vertexPosition = vec4(aVertexPosition, 1);
  

  vPosition = vec3(uMVMatrix * vertexPosition);
  vNormal = vec3(uNormalMatrix * vertexNormal);
  vTex = aVertexTex;

  gl_Position = uMVPMatrix * vertexPosition;
}
