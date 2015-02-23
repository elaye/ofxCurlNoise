#pragma once

#include "ofMain.h"

#include "ParticleEmitter.h"

#define WORK_GROUP_SIZE 256
#define STRINGIFY(...) #__VA_ARGS__

struct Particle {
	ofVec4f pos;
	ofVec4f vel;
	ofVec4f acc;
	ofVec4f lifespan;
};

class ParticleManager{

	const vector<ParticleEmitter>& emitters;
	ofBufferObject emittersBuffer;

	vector<Particle> particles;
	ofBufferObject particlesBuffer;

	ofVbo vbo;

	ofShader shader;
	
	public:
		// ParticleManager(): emitters(){}
		ParticleManager(const vector<ParticleEmitter>& _emitters, int n);
		// void setup(vector<ParticleEmitter>* emitters, int n);
		void update();
		// void updateEmitters(vector<ofPoint> newPos);

		ofVbo& getVbo(){ return vbo; }

	private:
		void initParticles();
		void loadShader();
		void updateEmitters();

};