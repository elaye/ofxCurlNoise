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

	curlNoise.setup(n);

	// particlesVbo = ofVbo(particleManager.getVbo());

	parameters.add(bCurlNoise.set("Curl noise", true));
	parameters.add(curlNoise.parameters);
}

void ofxCurlNoise::setAttributes(ofShader& renderShader){
	particleManager.setAttributes(renderShader);
	// particlesVbo = ofVbo(particleManager.getVbo());
}

void ofxCurlNoise::update(){
	particleManager.update();
	if(bCurlNoise){
		curlNoise.update();
	}
}

void ofxCurlNoise::draw(){
	// ofVbo particlesVbo(particleManager.getVbo());
	// particlesVbo.draw(GL_POINTS, 0, particlesVbo.getNumVertices());
	particleManager.draw();	
}