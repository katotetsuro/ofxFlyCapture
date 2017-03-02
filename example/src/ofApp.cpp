#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	auto devices = vg.listDevices();
	for (auto &d : devices) {
		ofLogNotice() << d.serialID;
	}
	vg.setDeviceID(ofToInt(devices.begin()->serialID));
	vg.setup(0, 0, true);
}

//--------------------------------------------------------------
void ofApp::update(){
	vg.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
	vg.draw(0, 0);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

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
