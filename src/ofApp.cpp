#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	snapCounter = 0;
	bSnapshot = false;

	ofSetWindowShape(SCREEN_W, SCREEN_H);
	ofSetFrameRate(0);
	ofSetVerticalSync(true);

	beat.loadSound("bgm.mp3");
	beat.play();

	ofBoxPrimitive tmpBox;
	tmpBox.set(8);
	tmpBox.setDepth(200);

	mVboBox = tmpBox.getMesh();
	
	ofDisableArbTex();
	
	ofLoadImage(mTexA, "k.png");
	ofLoadImage(mTexB, "i.png");
	ofLoadImage(mTexC, "m.png");
	ofLoadImage(mTexD, "o.png");
	ofLoadImage(mTexE, "c.png");

	ofEnableArbTex();

	loadShader();

	mCamMain.setupPerspective(false,60,0,10000);
}

//--------------------------------------------------------------
void ofApp::update(){
	ofSetWindowTitle(ofToString(ofGetFrameRate(), 2));
}

void ofApp::loadShader() {
	ofLogNotice() << "Reloading Shader.";
	mShdInstanced = shared_ptr<ofShader>(new ofShader());

	mShdInstanced->load("shaders/instanced.vert", "shaders/instanced.frag");

	GLint err = glGetError();
	if (err != GL_NO_ERROR) {
		ofLogNotice() << "Load Shader came back with GL error:	" << err;
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofEnableDepthTest();
	ofDisableAlphaBlending();
	
	ofBackgroundGradient(ofColor(200, 200, 200), ofColor(100, 100, 100));
	
	ofSetColor(ofColor::white);
	mCamMain.begin();
	
	float rot = 0.5 * ofGetFrameNum();
	ofTranslate(0, 0, -1000);
	ofRotateX(-60);
	ofRotateZ(rot);

	float * val = ofSoundGetSpectrum(nBands);
	for (int i = 0; i < nBands; i++) {
		fftSmoothed[i] *= 0.975f;
		if (fftSmoothed[i] < val[i]) fftSmoothed[i] = val[i];
	}

	random_shuffle(std::begin(fftSmoothed), std::end(fftSmoothed));

	mShdInstanced->begin();
	mShdInstanced->setUniformTexture("tex0", mTexA, 0);
	mShdInstanced->setUniformTexture("tex1", mTexB, 1);
	mShdInstanced->setUniformTexture("tex2", mTexC, 2);
	mShdInstanced->setUniformTexture("tex3", mTexD, 3);
	mShdInstanced->setUniformTexture("tex4", mTexE, 4);
	mShdInstanced->setUniform1f("timeValue", ofGetElapsedTimef());
	mShdInstanced->setUniform1fv("fftvalues", &fftSmoothed[0], nBands);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// drawing 16384 boxes
	mVboBox.drawInstanced(OF_MESH_FILL, NUM_POSITIONS);

	glDisable(GL_CULL_FACE);
	mShdInstanced->end();
	
	mCamMain.end();

	ofDisableDepthTest();

	//ofSetColor(ofColor::white);
	//ofDrawBitmapString("Use mouse to move camera.\nPress 'f' to toggle fullscreen;\nSPACEBAR to reload shader.", 10, 20);
	
	ofEnableAlphaBlending();

	if (bSnapshot == true) {
		img.grabScreen(0, 0, SCREEN_W, SCREEN_H);

		string fileName = "snapshot_" + ofToString(snapCounter, 5, '0') + ".png";
		img.save(fileName);

		snapCounter++;
	}

	if (bSnapshot) {
		if (snapCounter > 0) {
			img.draw(0, 0, SCREEN_W, SCREEN_H);
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	//bSnapshot = true;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	//bSnapshot = false;
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
	
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
	
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
	
}
