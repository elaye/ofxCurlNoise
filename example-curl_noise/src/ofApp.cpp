#include "ofApp.h"

void ofApp::setup(){
	ofSetVerticalSync(false);
	ofSetFrameRate(60);

	// Setup the particle emitter
	// emitter.setup(n);
	emitter.setup();
	// Setup the curl noise with 1024*256 particles
	curlNoise.setup(1024*256);
	// Add the particle emitter
	// curlNoise.addEmitter(emitter);

	// Setup gui
	emitterPanel.setup(emitter.parameters);
	emitterPanel.add(bMouse.setup("Mouse", false));
	
	curlNoisePanel.setup(curlNoise.parameters);
	curlNoisePanel.add(fps.setup("Fps:", ""));
	curlNoisePanel.setPosition(emitterPanel.getWidth()+20, emitterPanel.getPosition().y);
}

void ofApp::update(){
	fps = ofToString(ofGetFrameRate());
	float x, y;
	if(bMouse){
		// Update emitter with mouse position
		x = ofGetMouseX()-ofGetWidth()/2.0;
		y = ofGetMouseY()-ofGetHeight()/2.0;
	}
	else{
		// Update emitter with a '8' curve
		float t = ofGetElapsedTimef();
		float a = 300;
		x = a*cos(t)/(1+sin(t)*sin(t));
		y = a*sin(t)*cos(t)/(1+sin(t)*sin(t));
	}
	emitter.update(x, y);
	// Update curl noise
	curlNoise.update();
}

void ofApp::draw(){
	ofBackground(ofColor::black);
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	ofSetColor(ofColor(100, 1, 1, 50));
	
	ofPushMatrix();
		// Center scene
		ofTranslate(ofGetWidth()/2.0, ofGetHeight()/2.0);
		// Draw particles
		curlNoise.draw();
		ofSetColor(ofColor::white);
		emitter.draw();
	ofPopMatrix();
	
	// Draw GUI
	curlNoisePanel.draw();
	emitterPanel.draw();
}
