#pragma once

#include "ofMain.h"

#include "ParticleManager.h"
#include "ParticleEmitter.h"
#include "CurlNoise.h"


class ofxCurlNoise {

	ParticleManager particleManager;
	CurlNoise curlNoise;

	ofParameter<bool> bCurlNoise;

	public:
		ofParameterGroup parameters;
		ofParameterGroup advancedParameters;
		void setup(int n);
		// void addEmitter(ParticleEmitter& emitter);
		// void addEmitters(vector<ParticleEmitter>& emitters);
		void update();
		void draw();

		void setAttributes(ofShader& renderShader);

		ofVbo& getParticleBuffer(){ return particleManager.getVbo(); }

		void setTurbulence(float t){ curlNoise.setTurbulence(t); }

};

