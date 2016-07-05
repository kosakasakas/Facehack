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
    if(0)
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
    
    // 画像描画用のFBO確保
    {
        ofFbo::Settings setting;
        setting.width           = 300;
        setting.height          = 200;
        setting.internalformat  = GL_RGBA8;
        setting.numColorbuffers = 1;
        setting.useDepth        = true;
        setting.useStencil      = true;
        setting.depthStencilInternalFormat  = GL_DEPTH24_STENCIL8;
        setting.depthStencilAsTexture       = true;
        m_FboSource.allocate(setting);
        m_FboOutput.allocate(setting);
    }
    
    // ソースモデルの作成
    {
        // ファクトリ作成
        FacehackFactory factory;
        if (!factory.Initialize(300, 200))
        {
            ofLog(OF_LOG_ERROR, "ファクトリの生成に失敗しました");
            return;
        }
        
        // Facehackパラメータ作成
        ParamsPtr pParam = make_shared<FacehackParams>();
        {
            // カメラパラメータ
            ofVec3f camPos      = ofVec3f(0.0f, 0.0f, 300.0f);
            ofVec3f camLookAt   = ofVec3f(0.0f, 0.0f, 0.0f);
            float camFov        = 60.0f;
            float camAspectRatio = (float)m_FboSource.getWidth() / (float)m_FboSource.getHeight();
            
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
            faceQ.makeRotate(30.0f, ofVec3f(0.0f, 1.0f, 0.0f));
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
    
        // モデルを生成
        m_pSourceModel = factory.Create(pParam);
        m_pTargetModel = factory.Create();
        m_pTargetModel->SetSourceTexture(m_pSourceModel->GetTexture());
    }
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    m_pSourceModel->Draw();
    
    // フレームバッファを描画
    m_pSourceModel->GetTexture().draw(0, 0);
    
    ofDrawBitmapString( "press T : start tracking.", 0, 10);
    ofDrawBitmapString( "source model", 0, 30);
    
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
