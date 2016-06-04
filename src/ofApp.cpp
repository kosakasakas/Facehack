#include "ofApp.h"
#include "ofTest.h"
#include "ofxTimeMeasurements.h"
#include "System/Util/KSUtil.h"

//--------------------------------------------------------------
void ofApp::setup(){

    // 球体のセットアップ
    //m_Sphere.setRadius(ofGetHeight() * 0.3f);
    //m_Sphere.setResolution(2);
    
    // ==================================
    // フレームレートの設定。
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
        
        ofASSERT(testResult, "テストに失敗しました。");
    }
    
    m_Model.loadModel("model/shape_0.obj");
    // シェーダのロード
    m_Shader.load("shader/test.vert", "shader/test.frag");
    
    // これをしないとUVがピクセル座標になってしまう
    ofDisableArbTex();
    
    m_Image.load("image/alice.jpg");
    
    m_Mesh.addVertex(ofVec3f(100,100,0));
    m_Mesh.addVertex(ofVec3f(100,200,0));
    m_Mesh.addVertex(ofVec3f(200,200,0));
    m_Mesh.addVertex(ofVec3f(200,100,0));
    
    m_Mesh.addTexCoord(ofVec2f(0.0, 0.0));
    m_Mesh.addTexCoord(ofVec2f(0.0, 1.0));
    m_Mesh.addTexCoord(ofVec2f(1.0, 1.0));
    m_Mesh.addTexCoord(ofVec2f(1.0, 0.0));
    
    m_Mesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
    m_Mesh.addIndex(0);
    m_Mesh.addIndex(1);
    m_Mesh.addIndex(1);
    m_Mesh.addIndex(2);
    m_Mesh.addIndex(2);
    m_Mesh.addIndex(3);
    m_Mesh.addIndex(3);
    m_Mesh.addIndex(0);
    
    m_Fbo.allocate(512, 512, GL_RGBA8);
}

//--------------------------------------------------------------
void ofApp::update(){
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofEnableDepthTest();
    
    float spinX = sin(ofGetElapsedTimef()*.35f);
    float spinY = cos(ofGetElapsedTimef()*.075f);
    
    m_Model.setPosition(ofGetWidth()*0.5f, (float)ofGetHeight() * 0.5f , 0);
    //m_Sphere.setPosition(ofGetWidth()*0.5f, (float)ofGetHeight() * 0.5f , 0);
    
    ofxAssimpMeshHelper & meshHelper = m_Model.getMeshHelper(0);
    ofMaterial & material = meshHelper.material;
    material.setColors(ofFloatColor(0.5,0.5,0.5),
                       ofFloatColor(0.5, 0.5, 0.5),
                       ofFloatColor(0.5, 0.5, 0.5),
                       ofFloatColor(0.5,0.5,0.5));
    //auto mesh = m_Model.getMesh(0);
    
    m_Shader.setUniformTexture("u_sampleTex", m_Image.getTexture(), 0);
    
    m_Fbo.begin();
    ofClear(255,255,255, 255);
    m_Shader.begin();
    m_Mesh.draw();
    m_Shader.end();
    m_Fbo.end();
    
    m_Fbo.draw(0, 0);
    
    //m_Shader.begin();
    //material.begin();
    //m_Model.draw(OF_MESH_FILL);
    //m_Sphere.draw();
    //mesh.draw();
    //m_box.draw();
    
    //m_Mesh.draw();
    //material.end();
    //m_Shader.end();
    
    ofDisableDepthTest();
    
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
