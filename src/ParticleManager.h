#pragma once

#include "ofMain.h"

#define WORK_GROUP_SIZE 256
#define STRINGIFY(...) #__VA_ARGS__

struct Particle {
	ofVec4f pos;
	ofVec4f vel;
	ofVec4f acc;
};

class ParticleManager{

	vector<Particle> particles;
	ofBufferObject particlesBuffer, lifespanBuffer, emitterIdBuffer;

	ofVbo vbo;

	ofShader shader;

	static int particleNumber;
	
	public:
		void setup(int n);
		void update();
		void draw();

		void setAttributes(ofShader& renderShader);

		ofVbo& getVbo(){ return vbo; }

		static int getParticleNumber(){ return particleNumber; }

	private:
		void initParticles();
		void loadShader();
};