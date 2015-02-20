#include "ofxCurlNoise.h"

void ofxCurlNoise::setup(int n){
	particleEmitter.setup(n);
	curlNoise.setup(n);

	particlesVbo = ofVbo(particleEmitter.getVbo());

	parameters.add(curlNoise.parameters);
	parameters.add(particleEmitter.parameters);
}

void ofxCurlNoise::update(float x, float y){
	particleEmitter.update(x, y);
	curlNoise.update();
}

void ofxCurlNoise::draw(){
	particlesVbo.draw(GL_POINTS, 0, particlesVbo.getNumVertices());	
}