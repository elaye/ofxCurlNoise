#include "ofApp.h"

void ofApp::setup(){
	ofSetVerticalSync(false);
	ofSetFrameRate(60);

	curlNoise.setup(1024*256);
	// For render shader example
	// particlesVbo = curlNoise.getParticleBuffer();

	gui.setup(curlNoise.parameters);
	gui.add(speedCoeff.setup("Speed", 2.0, 0.1, 5.0));
	gui.add(fps.setup("Fps:", ""));

}

void ofApp::update(){
	fps = ofToString(ofGetFrameRate());
	// curlNoise.update(ofGetMouseX()-ofGetWidth()/2.0, ofGetMouseY()-ofGetHeight()/2.0);
	float t = ofGetElapsedTimef()*speedCoeff;
	float a = 300;
	float x = a*cos(t)/(1+sin(t)*sin(t));
	float y = a*sin(t)*cos(t)/(1+sin(t)*sin(t));
	// float x = 16*sin(t)*sin(t)*sin(t);
	// float y = 13*cos(t)-5*cos(2*t)-2*cos(3*t)-4*cos(4*t);
	curlNoise.update(x, y);
}

void ofApp::draw(){
	ofBackground(ofColor::black);
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	
	ofPushMatrix();
		ofTranslate(ofGetWidth()/2.0, ofGetHeight()/2.0);	
		ofSetColor(ofColor(100, 1, 1, 50));
	// cam.begin();
		// For render shader example
		// particlesVbo.draw(GL_POINTS, 0, particlesVbo.getNumVertices());
		curlNoise.draw();
	// cam.end();
	ofPopMatrix();
	
	gui.draw();
}
