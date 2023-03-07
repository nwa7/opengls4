#version 330 core

in vec3 vFragColor;
in vec2 vFragPosition;

out vec3 fFragColor;

void main() {
  float alpha = 2;
  float beta = 45.;
  float att = alpha * exp(-beta * distance(vec2(0.0), vFragPosition)*distance(vec2(0,0), vFragPosition));

  fFragColor = vFragColor * att;
};