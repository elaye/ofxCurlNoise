#pragma once

#include "ofMain.h"

#include "ParticleEmitter.h"

#define WORK_GROUP_SIZE 256
#define STRINGIFY(...) #__VA_ARGS__

struct Particle {
	ofVec4f pos;
	ofVec4f vel;
	ofVec4f acc;
	// ofVec4f lifespan;
};

class ParticleManager{

	vector<EmitterData> emittersData;

	vector<Particle> particles;
	ofBufferObject particlesBuffer, lifespanBuffer, emitterIdBuffer;

	ofVbo vbo;

	ofShader shader;
	
	public:
		void setup(int k, int n);
		void update();
		void updateEmitter(EmitterData& emitterData);
		void draw();

		void setAttributes(ofShader& renderShader);

		ofVbo& getVbo(){ return vbo; }

	private:
		void initParticles();
		void loadShader();
};