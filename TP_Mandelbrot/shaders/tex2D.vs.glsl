#version 330

layout(location = 0) in vec2 aVertexPosition;
layout(location = 1) in vec2 aVertexText;

out vec2 vText;

void main() {
    vText = aVertexText;
    gl_Position = vec4(aVertexPosition, 0, 1);
}
