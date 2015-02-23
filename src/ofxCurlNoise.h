#pragma once

#include "ofMain.h"

#include "ParticleManager.h"
#include "CurlNoise.h"


class ofxCurlNoise {

	shared_ptr<ParticleManager> particleManager;
	CurlNoise curlNoise;

	ofVbo particlesVbo;

	public:
		ofParameterGroup parameters;
		void setup(ParticleEmitter& emitter, int n);
		void setup(const vector<ParticleEmitter>& emitters, int n);
		void update();
		void draw();

		ofVbo& getParticleBuffer(){ return particlesVbo; }

	private:
		void setup(int n);

};

