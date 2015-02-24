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
};

class ParticleEmitter{

	static uint count;
	int id;

	EmitterData data;

	float prevTime;

	ofParameter<float> radius, 
						averageLifespan, lifespanVariation, 
						averageVelocity, velocityVariation, velocityScale;

	ofParameter<bool> bUseEmitterVelocity;

	public:
		ofEvent<EmitterData> updated;

		ofParameterGroup parameters;
		ParticleEmitter(){ data.id = count; ++count; }
		// ~ParticleEmitter(){ --count; }
		void setup();
		void update(float x, float y);
		void draw();

		int getId() { return id; }
		const ofPoint getPos() const { return data.pos; }

		// ostream& operator<<(ostream& os)
		// {
		// 	Emitter e = getData();
		// 	os << e.pos << '/' << e.vel << '/' << e.acc;
		// 	return os;
		// }
};