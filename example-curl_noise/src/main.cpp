#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
	ofGLWindowSettings settings;
	settings.setGLVersion(4,3);
	settings.width = 1024;
	settings.height = 768;
	settings.windowMode = OF_WINDOW;
	ofCreateWindow(settings);

	ofRunApp(new ofApp());
}
