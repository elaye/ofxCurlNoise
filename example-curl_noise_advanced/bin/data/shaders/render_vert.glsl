#version 430

uniform mat4 modelViewProjectionMatrix;

in vec4 position;

in float lifespan;
in int emitterId;

out float varyingLifespan;
flat out int id;

void main(){
	gl_Position = modelViewProjectionMatrix * position;
	// gl_Position.y = float(emitterId)*10;
	varyingLifespan = lifespan;
	id = emitterId;
	gl_PointSize = 3.0*(1.0-exp(-30.0/lifespan));
}