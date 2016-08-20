#include "ofApp.h"
#include "ofTest.h"
#include "ofxTimeMeasurements.h"
#include "System/Util/KSUtil.h"
#include "FacehackInclude.h"

using namespace Facehack;
using namespace Kosakasakas;

//--------------------------------------------------------------
void ofApp::setup(){
    
    // フレームレートの設定
    float frameRate = 60.0f;
    ofSetVerticalSync(false);
    ofSetFrameRate(frameRate);
    TIME_SAMPLE_SET_FRAMERATE(frameRate);
    
    // テスト実施
    {
        bool testResult = false;
        
        shared_ptr<ofTest> pTest = make_shared<ofTest>();
        if (pTest->Initialize())
        {
            testResult = pTest->DoTest();
        }
        
        ofASSERT(testResult, "テストに失敗しました。");
    }
    
    // これをしないとUVがピクセル座標になってしまう
    ofDisableArbTex();
    
    // ファクトリ作成
    int width   = 300;
    int height  = 200;
    FacehackFactory factory;
    if (!factory.Initialize(width, height))
    {
        ofLog(OF_LOG_ERROR, "ファクトリの生成に失敗しました");
        return;
    }
    
    // テストモデルの作成
    {
        // Facehackパラメータ作成
        ParamsPtr pParam = make_shared<FacehackParams>();
        {
            // カメラパラメータ
            ofVec3f camPos      = ofVec3f(0.0f, 0.0f, 300.0f);
            ofVec3f camLookAt   = ofVec3f(0.0f, 0.0f, 0.0f);
            float camFov        = 60.0f;
            float camAspectRatio = (float)width/(float)height;
            
            // イルミネーションパラメータ
            GammaCoeffArray gammaR, gammaG, gammaB;
            gammaR.fill(1.0f);
            gammaG.fill(1.0f);
            gammaB.fill(1.0f);
            
            // フェイシャルパラメータ
            AlphaCoeffArray alpha;
            BetaCoeffArray  beta;
            DeltaCoeffArray delta;
            alpha.fill(1.0f);
            beta.fill(2.0f);
            delta.fill(3.0f);
            ofQuaternion faceQ;
            faceQ.makeRotate(0.0f,  ofVec3f(1.0f, 0.0f, 0.0f),
                             30.0f, ofVec3f(0.0f, 1.0f, 0.0f),
                             0.0f,  ofVec3f(0.0f, 0.0f, 1.0f));
            ofVec3f faceTrans = ofVec3f(0.0f, 0.0f, 0.0f);
            
            if (!pParam->Initialize(camPos,
                                    camLookAt,
                                    camFov,
                                    camAspectRatio,
                                    gammaR,
                                    gammaG,
                                    gammaB,
                                    alpha,
                                    beta,
                                    delta,
                                    faceQ,
                                    faceTrans))
            {
                ofLog(OF_LOG_ERROR, "Facehackパラメータの初期化に失敗しました");
                return;
            }
        }
        
        // テストモデルを生成
        m_pTestModel = factory.Create(pParam);
    }
    
    // ソースモデルの作成
    {
        // パラメータは初期値でいい
        ParamsPtr pParam = make_shared<FacehackParams>();
        if (!pParam->Initialize())
        {
            ofLog(OF_LOG_ERROR, "Facehackパラメータの初期化に失敗しました");
            return;
        }
        
        // モデル生成
        m_pSrcModel = factory.Create(pParam);
    }
    
}

//--------------------------------------------------------------
void ofApp::update(){
    m_pSrcModel->Update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    m_pTestModel->Draw();
    //m_pSrcModel->Draw();
    
    // フレームバッファを描画
    m_pTestModel->GetTexture().draw(0, 0);
    //m_pSrcModel->GetTexture().draw(300, 0);
    
    ofDrawBitmapString( "press T : start tracking.", 0, 10);
    ofDrawBitmapString( "test model", 0, 30);
    
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
