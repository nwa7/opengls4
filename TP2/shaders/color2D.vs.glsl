#version 330 core

layout(location = 0) in vec2 aVertexPosition;
layout(location = 1) in vec3 aVertexColor;

out vec3 vFragColor;
out vec2 vFragPosition; // frag position


  mat3 translate(float tx, float ty){
    return (mat3(vec3(1,0,0), vec3(0,1,0), vec3(tx,ty,1)));
  };

  mat3 scale(float sx, float sy){
    return (mat3(vec3(sx, 0, 0), vec3(0,sy,0), vec3(0,0,1)));
  };

  mat3 rotate(float a){
    return (mat3(vec3(cos(a), sin(a),0), vec3(-sin(a), cos(a), 0), vec3(0,0,1)));
  };

void main() {
  vFragColor = aVertexColor;
  vFragPosition= aVertexPosition;
  gl_Position = vec4(aVertexPosition, 0, 1);
  // gl_Position = (vec4(aVertexPosition, 0, 1) + vec4(0.5,0.5,0,0)); //translation of 0.5,0.5
  // gl_Position = vec4(vFragColor.x * 2, vFragColor.y * 0.5, 0,1); //* 2 sur les x et * 0.5 sur les y
  //gl_Position = vec4((translate(1.,1.) * vec3(aVertexPosition, 1)).xy, 0, 1);
  };
