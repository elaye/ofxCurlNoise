#pragma once

#include "ofMain.h"

#include "ofxGui.h"

#include "ofxCurlNoise.h"

class ofApp : public ofBaseApp{

	vector<ParticleEmitter> emitters;
	ofxCurlNoise curlNoise;

	ofxPanel gui;
	ofxLabel fps;

	ofShader renderShader;
	ofEasyCam cam;

	ofParameterGroup parameters;

	public:
		void setup();
		void update();
		void updateEmitter(int i);
		void draw();
		
};
