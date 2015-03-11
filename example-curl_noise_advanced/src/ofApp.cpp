#include "ofApp.h"

void ofApp::setup(){
	ofSetVerticalSync(false);
	ofSetFrameRate(60);

	// Create 3 particle emitters 
	emitters = {ParticleEmitter(), ParticleEmitter(), ParticleEmitter()};
	// Setup the particle emitters
	for(auto& e : emitters){
		e.setup();
	}

	// Setup the curl noise with the number of particles we want
	curlNoise.setup(1024*256);
	// Set the noise turbulence
	curlNoise.setTurbulence(0.3);

	renderShader.load("shaders/render_vert.glsl", "shaders/render_frag.glsl");
	// Add 'lifespan' and 'emitterId' attributes to the particle vbo
	// (see the shaders for how to use these)
	curlNoise.setAttributes(renderShader);

	// Setup GUI
	parameters.add(curlNoise.parameters);
	for(auto& e : emitters){
		parameters.add(e.parameters);
	}
	
	gui.setup(parameters);
	gui.add(fps.setup("Fps:", ""));
	gui.minimizeAll();
}

void ofApp::update(){
	fps = ofToString(ofGetFrameRate());
	
	// Update emitters' positions
	for(uint i = 0; i < emitters.size(); ++i){
		updateEmitter(i);
	}
	// Update curl noise
	curlNoise.update();
}

void ofApp::updateEmitter(int i){
	float t = ofGetElapsedTimef();
	float r = 150.0;
	float theta = t + 4*M_PI*i/float(emitters.size());
	float x = r*cos(theta) + r/2.0*cos(2*M_PI*i/float(emitters.size()));
	float y = r*sin(theta) + r/2.0*sin(2*M_PI*i/float(emitters.size()));
	emitters[i].update(x, y, 0);
}

void ofApp::draw(){
	ofBackground(ofColor::lightGrey);
	ofEnableAlphaBlending();

	cam.begin();
		// Draw particles
		renderShader.begin();
			curlNoise.draw();
		renderShader.end();
	cam.end();

	// Draw GUI
	gui.draw();
}
