#include "ParticleEmitter.h"

uint ParticleEmitter::count = 0;

void ParticleEmitter::setup(){

	prevTime = 0.0;

	parameters.setName("Particle emitter " + ofToString(count));
	parameters.add(averageLifespan.set("Average lifespan", 120.0, 1.0, 3600.0));
	parameters.add(lifespanVariation.set("Lifespan variation", 50.0, 0.0, 100.0));
	parameters.add(averageVelocity.set("Average vel.", 1.3, 0.0, 2.5));
	parameters.add(velocityVariation.set("Vel. variation", 100.0, 0.0, 100.0));
	parameters.add(bUseEmitterVelocity.set("Use emitter vel.", true));
	parameters.add(velocityScale.set("Velocity scale", -10.0, -100.0, 100.0));
	parameters.add(radius.set("Radius", 3.0, 1.0, 30.0));

	data.pos = ofPoint(ofGetWidth()/2.0, ofGetHeight()/2.0);
	data.prevPos = data.pos;
	data.vel = ofPoint(0, 0);
	data.acc = ofPoint(0, 0);

	data.radius = radius;
	data.velocityScale = velocityScale;
	data.averageLifespan = averageLifespan;
	data.lifespanVariation = lifespanVariation;
	data.averageVelocity = averageVelocity;
	data.velocityVariation = velocityVariation;
	data.useEmitterVelocity = bUseEmitterVelocity;
}

void ParticleEmitter::update(float x, float y){
	float dt = ofGetElapsedTimef() - prevTime;
	ofPoint newVel = ofPoint(data.pos.x - x, data.pos.y - y)*dt;
	data.prevPos = data.pos;
	data.pos = ofPoint(x, y);
	data.acc.x = (data.vel.x - newVel.x)*dt;
	data.acc.y = (data.vel.y - newVel.y)*dt;
	data.vel = newVel;
	prevTime = ofGetElapsedTimef();
	ofNotifyEvent(updated, data, this);
}

void ParticleEmitter::draw(){
	ofSetColor(ofColor::red);
	ofFill();
	ofDrawCircle(data.pos, 10);
}