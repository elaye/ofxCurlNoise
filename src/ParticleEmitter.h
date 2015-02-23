#pragma once

#include "ofMain.h"

struct Emitter{
	ofVec4f pos;
	ofVec4f vel;
	ofVec4f acc;
	ofVec4f prevPos;
	float radius;
	float velocityScale;	
};

class ParticleEmitter{

	static uint count;

	ofVec4f pos, vel, acc, prevPos;
	float prevTime;

	ofParameter<float> emitterRadius, 
						averageLifespan, lifespanVariation, 
						averageVelocity, velocityVariation, velocityScale;

	ofParameter<bool> bUseEmitterVelocity;

	public:
		ofParameterGroup parameters;
		ParticleEmitter(){ ++count; }
		~ParticleEmitter(){ --count; }
		void setup();
		void update(float x, float y);
		void draw();

		const ofPoint getPos() const { return pos; }

		operator const Emitter(void) const{
			Emitter e;
			e.pos = pos;
			e.vel = vel;
			e.acc = acc;
			e.prevPos = prevPos;
			return e;
		}


};