#pragma once

#include "ofMain.h"

struct EmitterData{
	ofVec4f pos;
	ofVec4f vel;
	ofVec4f acc;
	ofVec4f prevPos;
	float radius;
	float velocityScale;
	float averageLifespan;
	float lifespanVariation;
	float averageVelocity;
	float velocityVariation;
	float useEmitterVelocity;
	int id;
	// ofVec4f lifespan;
	// ofVec4f velParam;
	// ofVec4f radius;	
};

class ParticleEmitter{

	static uint count;
	int id;

	// ofVec4f pos, vel, acc, prevPos;
	EmitterData data;
	float prevTime;

	ofParameter<float> radius, 
						averageLifespan, lifespanVariation, 
						averageVelocity, velocityVariation, velocityScale;

	ofParameter<bool> bUseEmitterVelocity;

	public:
		// static ofEvent<ParticleEmitter> updated;
		ofEvent<EmitterData> updated;

		ofParameterGroup parameters;
		ParticleEmitter(){ data.id = count; ++count; }
		// ~ParticleEmitter(){ --count; }
		void setup();
		void update(float x, float y);
		void draw();

		int getId() { return id; }
		const ofPoint getPos() const { return data.pos; }

		// EmitterData getData(){ return data; }

		// operator const Emitter(void) const{
		// 	Emitter e;
		// 	e.pos = pos;
		// 	e.vel = vel;
		// 	e.acc = acc;
		// 	e.prevPos = prevPos;
		// 	return e;
		// }

		// operator const Emitter(void) const{
		// 	Emitter e;
		// 	e.pos = pos;
		// 	e.vel = vel;
		// 	e.acc = acc;
		// 	e.prevPos = prevPos;
		// 	e.radius = emitterRadius;
		// 	e.velocityScale = velocityScale;
		// 	e.averageLifespan = averageLifespan;
		// 	e.lifespanVariation = lifespanVariation;
		// 	e.averageVelocity = averageVelocity;
		// 	e.velocityVariation = velocityVariation;
		// 	e.useEmitterVelocity = (bUseEmitterVelocity)? 1.0 : -1.0;
		// 	return e;			
		// }

		// EmitterData getData(){
		// 	EmitterData e;
		// 	e.pos = pos;
		// 	e.vel = vel;
		// 	e.acc = acc;
		// 	e.prevPos = prevPos;
		// 	e.radius = emitterRadius;
		// 	e.velocityScale = velocityScale;
		// 	e.averageLifespan = averageLifespan;
		// 	e.lifespanVariation = lifespanVariation;
		// 	e.averageVelocity = averageVelocity;
		// 	e.velocityVariation = velocityVariation;
		// 	e.useEmitterVelocity = (bUseEmitterVelocity)? 1.0 : -1.0;
		// 	e.id = id;
		// 	// e.radius = ofVec4f(emitterRadius);
		// 	// e.velParam = ofVec4f(velocityScale, averageVelocity, velocityVariation, (bUseEmitterVelocity)? 1.0 : -1.0);
		// 	// e.lifespan = ofVec4f(averageLifespan, lifespanVariation, 0.0, 0.0);			
		// 	return e;			
		// }


		// ostream& operator<<(ostream& os)
		// {
		// 	Emitter e = getData();
		// 	os << e.pos << '/' << e.vel << '/' << e.acc;
		// 	return os;
		// }
};