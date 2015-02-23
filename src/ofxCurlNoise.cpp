#include "ofxCurlNoise.h"

void ofxCurlNoise::setup(ParticleEmitter& emitter, int n){
	vector<ParticleEmitter> emitters;
	emitters.push_back(emitter);
	setup(emitters, n);
}

void ofxCurlNoise::setup(const vector<ParticleEmitter>& emitters, int n){
	particleManager = shared_ptr<ParticleManager> (new ParticleManager(emitters, n));
	// particleManager->setup(emitters, n);
	curlNoise.setup(n);

	particlesVbo = ofVbo(particleManager->getVbo());

	parameters.add(curlNoise.parameters);
	// parameters.add(particleManager.parameters);
}

void ofxCurlNoise::update(){
	particleManager->update();
	curlNoise.update();
}

void ofxCurlNoise::draw(){
	particlesVbo.draw(GL_POINTS, 0, particlesVbo.getNumVertices());	
}