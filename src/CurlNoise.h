#pragma once

#include "ofMain.h"

#define WORK_GROUP_SIZE 256
#define STRINGIFY(...) #__VA_ARGS__

class CurlNoise{
	ofShader curlNoiseShader;

	ofParameter<float> turbulence, noisePositionScale, noiseTimeScale, noiseScale;

	int particlesNumber;

	public:
		ofParameterGroup parameters;
		ofParameterGroup advancedParameters;
		void setup(int n);
		void update();

		void setTurbulence(float t){ turbulence = t; }

	private:
		void loadShader();
		string getNoiseShaderFunctions();
};