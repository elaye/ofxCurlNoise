#pragma once

#include "ofMain.h"

#include "ofxGui.h"

#include "ofxCurlNoise.h"

class ofApp : public ofBaseApp{

	vector<ParticleEmitter> emitters;
	ofxCurlNoise curlNoise;

	ofxPanel curlNoisePanel;
	ofxPanel emitterPanel;
	ofxLabel fps;

	ofShader renderShader;
	ofEasyCam cam;

	public:
		void setup();
		void update();
		void updateEmitter(int i);
		void draw();
		
};
