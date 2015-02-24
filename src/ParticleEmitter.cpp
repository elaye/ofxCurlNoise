#include "ParticleEmitter.h"

// ofEvent<ParticleEmitter> ParticleEmitter::updated = ofEvent<ParticleEmitter>();
uint ParticleEmitter::count = 0;

void ParticleEmitter::setup(){
	pos = ofPoint(ofGetWidth()/2.0, ofGetHeight()/2.0);
	prevPos = pos;
	vel = ofPoint(0, 0);
	acc = ofPoint(0, 0);

	prevTime = 0.0;

	parameters.setName("Particle emitter " + ofToString(count));
	parameters.add(averageLifespan.set("Average lifespan", 120.0, 1.0, 3600.0));
	parameters.add(lifespanVariation.set("Lifespan variation", 50.0, 0.0, 100.0));
	parameters.add(averageVelocity.set("Average vel.", 1.3, 0.0, 2.5));
	parameters.add(velocityVariation.set("Vel. variation", 100.0, 0.0, 100.0));
	parameters.add(bUseEmitterVelocity.set("Use emitter vel.", true));
	parameters.add(velocityScale.set("Velocity scale", -10.0, -100.0, 100.0));
	parameters.add(emitterRadius.set("Radius", 3.0, 1.0, 30.0));

}

void ParticleEmitter::update(float x, float y){
	float dt = ofGetElapsedTimef() - prevTime;
	ofPoint newVel = ofPoint(pos.x - x, pos.y - y)*dt;
	prevPos = pos;
	pos = ofPoint(x, y);
	acc.x = (vel.x - newVel.x)*dt;
	acc.y = (vel.y - newVel.y)*dt;
	vel = newVel;
	prevTime = ofGetElapsedTimef();
	// ofLog() << "id:" << id;
	ofNotifyEvent(updated, *this, this);
}

void ParticleEmitter::draw(){
	ofSetColor(ofColor::red);
	ofFill();
	ofDrawCircle(pos, 10);
}