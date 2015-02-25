#include "ofxCurlNoise.h"

void ofxCurlNoise::setup(ParticleEmitter& emitter, int n){
	ofAddListener(emitter.updated, &particleManager, &ParticleManager::updateEmitter);
	setup(1, n);
}

void ofxCurlNoise::setup(vector<ParticleEmitter>& emitters, int n){
	for(auto& e : emitters){
		ofAddListener(e.updated, &particleManager, &ParticleManager::updateEmitter);
	}
	setup(emitters.size(), n);
}

void ofxCurlNoise::setup(int k, int n){
	particleManager.setup(k, n);

	curlNoise.setup(n);

	parameters.setName("Curl noise");
	parameters.add(bCurlNoise.set("Enable", true));
	parameters.add(curlNoise.parameters);

	advancedParameters.setName("Curl noise");
	advancedParameters.add(curlNoise.advancedParameters);
}

void ofxCurlNoise::setAttributes(ofShader& renderShader){
	particleManager.setAttributes(renderShader);
}

void ofxCurlNoise::update(){
	particleManager.update();
	if(bCurlNoise){
		curlNoise.update();
	}
}

void ofxCurlNoise::draw(){
	particleManager.draw();	
}