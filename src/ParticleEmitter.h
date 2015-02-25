#pragma once

#include "ofMain.h"

struct EmitterData{
	ofVec4f pos;
	ofVec4f vel;
	ofVec4f acc;
	ofVec4f prevPos;
	ofQuaternion orientation;
	ofParameter<float> radius;
	ofParameter<float> velocityAngle;
	ofParameter<float> velocityScale;
	ofParameter<float> averageLifespan;
	ofParameter<float> lifespanVariation;
	ofParameter<float> averageVelocity;
	ofParameter<float> velocityVariation;
	ofParameter<bool> bUseEmitterVelocity;
	int id;
};

class ParticleEmitter{

	static uint count;

	EmitterData data;

	ofQuaternion defaultOrientation;
	float prevTime;

	public:
		ofEvent<EmitterData> updated;

		ofParameterGroup parameters;
		ParticleEmitter(){ data.id = count; ++count; }
		// ~ParticleEmitter(){ --count; }
		void setup();
		void update(float x, float y, float z = 0.0);
		void draw();

		// void setOrientation(ofQuaternion q){ defaultOrientation = q; }

		int getId() { return data.id; }
		const ofPoint getPos() const { return data.pos; }

		// ostream& operator<<(ostream& os)
		// {
		// 	Emitter e = getData();
		// 	os << e.pos << '/' << e.vel << '/' << e.acc;
		// 	return os;
		// }
};