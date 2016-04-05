#include "ofApp.h"
#include "ofxTimeMeasurements.h"

#include "../extAddons/Eigen/Dense"

//--------------------------------------------------------------
void ofApp::setup(){
    // フレームレートの設定。とりあえず上限なし
    float frameRate = 0.0f;
    ofSetVerticalSync(false);
    ofSetFrameRate(frameRate);
    TIME_SAMPLE_SET_FRAMERATE(frameRate);
}

//--------------------------------------------------------------
void ofApp::update(){
    Eigen::Vector4f v1; v1 << 1.0f, 1.0f, 1.0f, 1.0f;
    Eigen::Vector4f v2; v2 << 0.2f, 0.2f, 0.1f, 0.1f;
    float res   = v1.dot(v2);
    ofLog(OF_LOG_NOTICE, "dot: %f", res);
}

//--------------------------------------------------------------
void ofApp::draw(){

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
