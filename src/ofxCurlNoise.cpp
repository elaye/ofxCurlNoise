#include "ofxCurlNoise.h"

// void ofxCurlNoise::addEmitter(ParticleEmitter& emitter){
// 	ofAddListener(emitter.updated, &particleManager, &ParticleManager::updateEmitter);
// 	particleManager.addEmitter();
// }

// void ofxCurlNoise::addEmitters(vector<ParticleEmitter>& emitters){
// 	for(auto& e : emitters){
// 		ofAddListener(e.updated, &particleManager, &ParticleManager::updateEmitter);
// 	}
// 	particleManager.addEmitters(emitters.size());
// }

void ofxCurlNoise::setup(int n){
	particleManager.setup(n);

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