#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetVerticalSync(false);
	ofSetFrameRate(60);

	curlNoise.setup(128*128);
	particlesVbo = curlNoise.getParticleBuffer();

	gui.setup(curlNoise.parameters);
	gui.add(fps.setup("Fps:", ""));

}

//--------------------------------------------------------------
void ofApp::update(){
	fps = ofToString(ofGetFrameRate());
	curlNoise.updateEmitter(ofGetMouseX()-ofGetWidth()/2.0, ofGetMouseY()-ofGetHeight()/2.0);
	curlNoise.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackgroundGradient(ofColor(15, 16, 37), ofColor(11, 11, 10));

	ofPushMatrix();
		ofTranslate(ofGetWidth()/2.0, ofGetHeight()/2.0);	
	// cam.begin();
		particlesVbo.draw(GL_POINTS, 0, particlesVbo.getNumVertices());
	// cam.end();
	ofPopMatrix();
	
	gui.draw();
}
