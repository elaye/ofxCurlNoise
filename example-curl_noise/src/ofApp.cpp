#include "ofApp.h"

void ofApp::setup(){
	ofSetVerticalSync(false);
	ofSetFrameRate(60);

	// particleEmitter.setup();
	// curlNoise.setup(particleEmitter, 1024*256);

	for(int i = 0; i < 2; ++i){
		ParticleEmitter em;
		em.setup();
		emitters.push_back(em);
	}
	curlNoise.setup(emitters, 1024*256);

	gui.setup(curlNoise.parameters);
	gui.add(speedCoeff.setup("Speed", 2.0, 0.1, 5.0));
	gui.add(fps.setup("Fps:", ""));

}

void ofApp::update(){
	fps = ofToString(ofGetFrameRate());

	float t = ofGetElapsedTimef()*speedCoeff;
	float a = 300;
	float x = a*cos(t)/(1+sin(t)*sin(t));
	float y = a*sin(t)*cos(t)/(1+sin(t)*sin(t));
	// float x = 16*sin(t)*sin(t)*sin(t);
	// float y = 13*cos(t)-5*cos(2*t)-2*cos(3*t)-4*cos(4*t);
	// particleEmitter.update(x, y);
	emitters[0].update(x, y);

	x = a*cos(t+M_PI/2.0)/(1+sin(t+M_PI/2.0)*sin(t+M_PI/2.0));
	y = a*sin(t+M_PI/2.0)*cos(t+M_PI/2.0)/(1+sin(t+M_PI/2.0)*sin(t+M_PI/2.0));	
	emitters[1].update(x, y);

	curlNoise.update();
}

void ofApp::draw(){
	ofBackground(ofColor::black);
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	
	cam.begin();
	// ofPushMatrix();
		// ofTranslate(ofGetWidth()/2.0, ofGetHeight()/2.0);	
		ofSetColor(ofColor(100, 1, 1, 50));
		curlNoise.draw();
		// particleEmitter.draw();
		// emitters[0].draw();
		// emitters[1].draw();
	// ofPopMatrix();
	cam.end();
	
	gui.draw();
}
