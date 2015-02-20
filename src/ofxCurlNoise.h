#pragma once

#include "ofMain.h"

#include "ParticleEmitter.h"
#include "CurlNoise.h"


class ofxCurlNoise {

	ParticleEmitter particleEmitter;
	CurlNoise curlNoise;

	ofVbo particlesVbo;

	public:
		ofParameterGroup parameters;
		void setup(int n);
		void update(float x, float y);
		void draw();

		ofVbo& getParticleBuffer(){ return particlesVbo; }

};

