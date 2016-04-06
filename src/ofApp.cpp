#include "ofApp.h"
#include "ofTest.h"
#include "ofxTimeMeasurements.h"
#include "System/Util/KSUtil.h"

using namespace std;

//--------------------------------------------------------------
void ofApp::setup(){
    // ==================================
    // フレームレートの設定。とりあえず上限なし
    // ==================================
    float frameRate = 0.0f;
    ofSetVerticalSync(false);
    ofSetFrameRate(frameRate);
    TIME_SAMPLE_SET_FRAMERATE(frameRate);
    
    // ==================================
    // テスト実施
    // ==================================
    {
        bool testResult = false;
        
        shared_ptr<ofTest> pTest = make_shared<ofTest>();
        if (pTest->Initialize())
        {
            testResult = pTest->DoTest();
        }
        
        KSUtil::ofASSERT(testResult, "テストに失敗しました。");
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    
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
