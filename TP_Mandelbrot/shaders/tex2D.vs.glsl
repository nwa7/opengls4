#version 330

layout(location = 0) in vec2 aVertexPosition;
layout(location = 1) in vec2 aVertexText;
//uniform mat3 uModelMatrix;
uniform float uTime;

out vec2 vText;

mat3 rotate(float a){
    return (mat3(vec3(cos(a), sin(a),0), vec3(-sin(a), cos(a), 0), vec3(0,0,1)));
};


void main() {
    vText = aVertexText;
    //mat3 rot = uModelMatrix;
    //gl_Position = vec4((rot * vec3(aVertexPosition, 1)).xy, 0, 1);
    gl_Position = vec4((rotate(uTime) * vec3(aVertexPosition, 1)).xy, 0, 1);
}