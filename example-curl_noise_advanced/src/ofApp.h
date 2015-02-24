#pragma once

#include "ofMain.h"

#include "ofxGui.h"

#include "ofxCurlNoise.h"

class ofApp : public ofBaseApp{

	vector<ParticleEmitter> particleEmitters;
	ofxCurlNoise curlNoise;
	ofVbo particlesVbo;

	ofShader renderShader;

	ofEasyCam cam;

	ofxFloatSlider speedCoeff;
	ofxLabel fps;
	ofxPanel gui;

	public:
		void setup();
		void update();
		void draw();
		
};
