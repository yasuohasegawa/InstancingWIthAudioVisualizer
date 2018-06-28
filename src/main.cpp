#include "ofMain.h"
#include "ofApp.h"
#include "ofGLProgrammableRenderer.h"

#define SCREEN_W 1024
#define SCREEN_H 768

//========================================================================
int main( ){

	ofGLWindowSettings glWindowSettings;

	glWindowSettings.setGLVersion(3, 2);
	glWindowSettings.setSize(SCREEN_W, SCREEN_H);
	ofCreateWindow(glWindowSettings);
	
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
