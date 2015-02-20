#pragma once

#include "ofMain.h"

#include "ParticleEmitter.h"
#include "CurlNoise.h"


class ofxCurlNoise {

	ParticleEmitter particleEmitter;
	CurlNoise curlNoise;

	public:
		ofParameterGroup parameters;
		void setup(int n);
		void update(float x, float y);

		ofVbo& getParticleBuffer(){ return particleEmitter.getVbo(); }

};

