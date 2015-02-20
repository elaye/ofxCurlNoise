#pragma once

#include "ofMain.h"

#define WORK_GROUP_SIZE 256
#define STRINGIFY(...) #__VA_ARGS__

struct Emitter {
	ofVec4f pos;
	ofVec4f vel;
	ofVec4f acc;		
};

struct Particle {
	ofVec4f pos;
	ofVec4f vel;
	ofVec4f acc;
	ofVec4f lifespan;
};

class ofxCurlNoise {

	ofShader emitterShader, curlNoiseShader;

	vector<Particle> particles;
	ofBufferObject particlesBuffer;
	ofVbo vbo;

	Emitter emitter;

	ofParameter<float> turbulence, noisePositionScale, noiseTimeScale, noiseScale, baseSpeedScale;

	float prevTime;

	public:
		ofParameterGroup parameters;
		void setup(int n);
		void update(float x, float y);
		void updateEmitter(float x, float y);
		void updateCurlNoise();

		ofVbo& getParticleBuffer(){ return vbo; }

	private:
		void loadEmitterShader();
		void loadCurlNoiseShader();
		void initEmitter();
		void initParticles();

};

