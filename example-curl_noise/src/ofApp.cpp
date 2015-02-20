#include "ofApp.h"

void ofApp::setup(){
	ofSetVerticalSync(false);
	ofSetFrameRate(60);

	curlNoise.setup(1024*256);
	particlesVbo = curlNoise.getParticleBuffer();

	gui.setup(curlNoise.parameters);
	gui.add(fps.setup("Fps:", ""));

}

void ofApp::update(){
	fps = ofToString(ofGetFrameRate());
	curlNoise.update(ofGetMouseX()-ofGetWidth()/2.0, ofGetMouseY()-ofGetHeight()/2.0);
}

void ofApp::draw(){
	ofBackground(ofColor::black);
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	
	ofPushMatrix();
		ofTranslate(ofGetWidth()/2.0, ofGetHeight()/2.0);	
		ofSetColor(ofColor(200, 0, 0, 50));
	// cam.begin();
		particlesVbo.draw(GL_POINTS, 0, particlesVbo.getNumVertices());
	// cam.end();
	ofPopMatrix();
	
	gui.draw();
}
