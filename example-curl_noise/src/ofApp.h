#pragma once

#include "ofMain.h"

#include "ofxGui.h"

#include "ofxCurlNoise.h"

class ofApp : public ofBaseApp{

	ParticleEmitter emitter;
	ofxCurlNoise curlNoise;

	ofxPanel curlNoisePanel;
	ofxPanel emitterPanel;
	ofxToggle bMouse;
	ofxLabel fps;

	public:
		void setup();
		void update();
		void draw();
		
};
