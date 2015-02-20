#pragma once

#include "ofMain.h"

#define WORK_GROUP_SIZE 256
#define STRINGIFY(...) #__VA_ARGS__

struct Particle {
	ofVec4f pos;
	ofVec4f vel;
	ofVec4f acc;
	ofVec4f lifespan;
};

class ParticleEmitter{

	ofVec4f pos, vel, acc;
	float prevTime;

	ofParameter<float> emitterRadius;

	vector<Particle> particles;
	ofBufferObject particlesBuffer;
	ofVbo vbo;

	ofShader shader;

	public:
		ofParameterGroup parameters;
		void setup(int n);
		void update(float x, float y);
		void initParticles();
		void loadShader();

		ofVbo& getVbo(){ return vbo; }

};