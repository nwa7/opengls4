#version 330 core

in vec3 vFragColor;
in vec2 vFragPosition;

out vec3 fFragColor;

vec2 complexSqr(vec2 z){
	return vec2(z.x*z.x - z.y*z.y, 2*z.x*z.y);
}

void main() {
  	//fFragColor =  vec3(vFragPosition.x, vFragPosition.y, 0.);	

	vec2 z = vFragPosition;

	int n_max = 30;
	int i;
	for(i=0; i<n_max && length(z)<2; i++){
		z = complexSqr(z)+vFragPosition;
	}

	fFragColor= vec3(i/float(n_max), 0., 0.);

	//fFragColor =  vec3(1., 0., 0.);
	//fFragColor = vFragColor;
};