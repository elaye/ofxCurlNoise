#include "ofApp.h"

void ofApp::setup(){
	ofSetVerticalSync(false);
	ofSetFrameRate(60);

	// Create 3 particle emitters 
	emitters = {ParticleEmitter(), ParticleEmitter(), ParticleEmitter()};
	ofQuaternion q;
	q.makeRotate(-90, 0, 0, 1);
	// Setup the particle emitters
	for(auto& e : emitters){
		e.setup();
		e.setOrientation(q);		
	}

	// Setup the curl noise, pass the emitter and 
	// the number of particles we want
	curlNoise.setup(emitters, 1024*256);
	curlNoise.setTurbulence(0.3);

	renderShader.load("shaders/render_vert.glsl", "shaders/render_frag.glsl");
	curlNoise.setAttributes(renderShader);
	// Add 'lifespan' and 'emitterId' attributes to the particle vbo
	// See the shaders for how to use these

	// Setup gui
	// emitterPanel.setup(emitter.parameters);
	// emitterPanel.add(bMouse.setup("Mouse", false));

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
	float phi = M_PI/2.0;
	float theta = t + 4*M_PI*i/float(emitters.size());
	float x = r*cos(theta)*sin(phi) + r/2.0*cos(2*M_PI*i/float(emitters.size()));
	float y = r*sin(theta)*sin(phi) + r/2.0*sin(2*M_PI*i/float(emitters.size()));
	float z = r*cos(phi);
	emitters[i].update(x, y, z);
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
