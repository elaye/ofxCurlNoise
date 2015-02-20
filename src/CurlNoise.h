#pragma once

#include "ofMain.h"

#define WORK_GROUP_SIZE 256
#define STRINGIFY(...) #__VA_ARGS__

class CurlNoise{
	ofShader curlNoiseShader;

	ofParameter<float> turbulence, noisePositionScale, noiseTimeScale, noiseScale, baseSpeedScale;

	int particlesNumber;

	public:
		ofParameterGroup parameters;
		void setup(int n);
		void update();

	private:
		void loadShader();
};