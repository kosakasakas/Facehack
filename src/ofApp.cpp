#include "ofApp.h"
#include "ofTest.h"
#include "ofxTimeMeasurements.h"
#include "System/Util/KSUtil.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
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
    
    // シェーダのロード
    m_Shader.load("shader/test.vert", "shader/test.frag");
    
    // これをしないとUVがピクセル座標になってしまう
    ofDisableArbTex();
    
    // 画像ロード
    m_Image.load("image/alice.jpg");
    
    // メッシュを手動生成するテスト
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
    
    // オフスクリーンレンダリング用のFBO確保
    m_Fbo.allocate(512, 512, GL_RGBA8);
}

//--------------------------------------------------------------
void ofApp::update(){
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    // デプステスト有効化
    ofEnableDepthTest();
    
    // トランスフォーム処理
    float spinX = sin(ofGetElapsedTimef()*.35f);
    float spinY = cos(ofGetElapsedTimef()*.075f);
    
    // シェーダのUniformセット
    m_Shader.setUniformTexture("u_sampleTex", m_Image.getTexture(), 0);
    
    // オフスクリーン描画
    m_Fbo.begin();
    {
        // カラー初期化
        ofClear(255,255,255, 255);
        // シェーダ有効化
        m_Shader.begin();
        {
            // メッシュを描画
            m_Mesh.draw();
        }
        m_Shader.end();
    }
    m_Fbo.end();
    
    // フレームバッファを描画
    m_Fbo.draw(0, 0);
    
    // UI用にデプステストは切っておく
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
