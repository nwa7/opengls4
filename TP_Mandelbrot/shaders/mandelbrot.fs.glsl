#version 330 core

in vec3 vFragColor;
in vec2 vFragPosition;

out vec3 fFragColor;

vec2 ComplexSqr(vec2 z){
	return vec2(z.x*z.x - z.y*z.y, 2*z.x*z.y);
}

void main() {
	vec2 z = vFragPosition;

	int N = 30;
	int i;
	for(i = 0; i < N && length(z)<2; i++){
		z = ComplexSqr(z)+vFragPosition;
	}

	fFragColor= vec3(i/float(N), 0., 0.);
};