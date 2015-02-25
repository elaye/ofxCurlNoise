#include "ParticleEmitter.h"

uint ParticleEmitter::count = 0;

void ParticleEmitter::setup(){

	prevTime = 0.0;

	parameters.setName("Particle emitter " + ofToString(data.id+1));
	parameters.add(data.averageLifespan.set("Average lifespan", 120.0, 1.0, 3600.0));
	parameters.add(data.lifespanVariation.set("Lifespan variation", 50.0, 0.0, 100.0));
	parameters.add(data.velocityAngle.set("Velocity angle", M_PI/6.0, 0.0, M_PI));
	parameters.add(data.averageVelocity.set("Average vel.", 1.5, 0.0, 10.0));
	parameters.add(data.velocityVariation.set("Vel. variation", 100.0, 0.0, 100.0));
	parameters.add(data.bUseEmitterVelocity.set("Use emitter vel.", true));
	parameters.add(data.velocityScale.set("Emitter Vel. scale", 1.0, -10.0, 10.0));
	parameters.add(data.radius.set("Radius", 3.0, 1.0, 30.0));

	data.pos = ofPoint(ofGetWidth()/2.0, ofGetHeight()/2.0);
	data.prevPos = data.pos;
	data.vel = ofPoint(0, 0);
	data.acc = ofPoint(0, 0);
}

void ParticleEmitter::update(float x, float y, float z){
	float dt = ofGetElapsedTimef() - prevTime;
	ofPoint newVel = (data.pos - ofPoint(x, y, z))*dt;
	data.prevPos = data.pos;
	data.pos = ofPoint(x, y, z);
	data.acc = (data.vel - newVel)*dt;
	data.vel = newVel;
	prevTime = ofGetElapsedTimef();
	// data.velRotFromZ.makeRotate(data.vel.getNormalized(), ofVec3f(0, 0, 1));
	ofQuaternion q;
	q.makeRotate(data.vel.getNormalized(), ofVec3f(0, 0, 1));
	data.orientation = q*defaultOrientation;
	ofNotifyEvent(updated, data, this);
}

void ParticleEmitter::draw(){
	ofSetColor(ofColor::red);
	ofFill();
	ofDrawCircle(data.pos, 10);
}