#include "ofxCurlNoise.h"

void ofxCurlNoise::setup(int n){
	particleEmitter.setup(n);
	curlNoise.setup(n);

	parameters.add(curlNoise.parameters);
	parameters.add(particleEmitter.parameters);
}

void ofxCurlNoise::update(float x, float y){
	particleEmitter.update(x, y);
	curlNoise.update();
}