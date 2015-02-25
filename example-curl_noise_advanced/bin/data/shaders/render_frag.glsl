#version 430

in float varyingLifespan;
flat in int id;

out vec4 color;

void main(){
	float a = clamp(exp(-100.0/varyingLifespan), 0.1, 1.0);
	float v = clamp(varyingLifespan/200.0, 0.1, 0.9);
	if(id == 0){
		color = vec4(0.3*v, 0.3, v, a);
	}
	else if(id == 1){
		color = vec4(0.3, v, 0.3*v, a);
	}
	else{
		color = vec4(v, 0.3*v, 0.3, a);
	}
}