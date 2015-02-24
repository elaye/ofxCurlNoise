#pragma once

#include "ofMain.h"

#include "ParticleManager.h"
#include "CurlNoise.h"


class ofxCurlNoise {

	ParticleManager particleManager;
	CurlNoise curlNoise;

	ofVbo particlesVbo;

	ofParameter<bool> bCurlNoise;

	public:
		ofParameterGroup parameters;
		void setup(ParticleEmitter& emitter, int n);
		void setup(vector<ParticleEmitter>& emitters, int n);
		void update();
		void draw();

		ofVbo& getParticleBuffer(){ return particlesVbo; }

	private:
		void setup(int k, int n);

};

