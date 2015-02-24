#include "ofxCurlNoise.h"

void ofxCurlNoise::setup(ParticleEmitter& emitter, int n){
	// particleManager.addEmitter(&emitter);
	ofAddListener(emitter.updated, &particleManager, &ParticleManager::updateEmitter);
	setup(1, n);
}

void ofxCurlNoise::setup(vector<ParticleEmitter>& emitters, int n){
	for(auto& e : emitters){
	// 	particleManager.addEmitter(&e);
		ofAddListener(e.updated, &particleManager, &ParticleManager::updateEmitter);
	}
	setup(emitters.size(), n);
}

void ofxCurlNoise::setup(int k, int n){
	particleManager.setup(k, n);
	// ofAddListener(ParticleEmitter::updated, &particleManager, &ParticleManager::updateEmitter);

	curlNoise.setup(n);

	particlesVbo = ofVbo(particleManager.getVbo());

	parameters.add(curlNoise.parameters);
	// parameters.add(particleManager.parameters);
}

void ofxCurlNoise::update(){
	particleManager.update();
	curlNoise.update();
}

void ofxCurlNoise::draw(){
	particlesVbo.draw(GL_POINTS, 0, particlesVbo.getNumVertices());	
}