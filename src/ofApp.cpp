#include "ofApp.h"
#include "ofTest.h"
#include "ofxTimeMeasurements.h"
#include "System/Util/KSUtil.h"

using namespace Kosakasakas;

//--------------------------------------------------------------
void ofApp::setup(){
    
    // フレームレートの設定
    float frameRate = 0.0f;
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
    
    // シェーダのロード
    m_Shader.load("shader/test.vert", "shader/test.frag");
    
    // これをしないとUVがピクセル座標になってしまう
    ofDisableArbTex();
    
    // 画像ロード
    //m_Image.load("image/alice.jpg");
    
    // オフスクリーンレンダリング用のFBO確保
    m_Fbo.allocate(512, 512, GL_RGBA8);
    
    // バーセルPCAモデルのセットアップ
    {
        // モデルのロード
        m_BaselFace.Initialize("unpublic", "model2009-publicmm1-bfm.h5");
        
        // 適当に主成分を突っ込んでメッシュ構築してみる
        KSVectorXf shapeCoeff(3);
        KSVectorXf albedoCoeff(3);
        shapeCoeff  << 2.0f, 3.0f, 1.0f;
        albedoCoeff << 0.0f, 1.0f, 1.0f;
        m_BaselFace.DrawSample(shapeCoeff, albedoCoeff);
        
        // 初期位置をセット
        m_BaselFace.GetModelMatrix().translate(0.5f * m_Fbo.getWidth(), 0.5f * m_Fbo.getHeight(), 0.0f);
    }
    
    // カメラのセットアップ
    {
        m_Camera.setPosition(0.0f, 0.0f, 0.0f);
        m_Camera.lookAt(ofVec3f(0.0f, 0.0f, 1.0f));
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    // デプステスト有効化
    ofEnableDepthTest();
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    
    // トランスフォーム処理
    //float spinX = sin(ofGetElapsedTimef()*.35f);
    //float spinY = cos(ofGetElapsedTimef()*.075f);
    
    // シェーダのUniformセット
    //m_Shader.setUniformTexture("u_sampleTex", m_Image.getTexture(), 0);
    
    // カメラを使う
    m_Camera.begin();
    {
        // オフスクリーン描画
        m_Fbo.begin();
        {
            // カラー初期化
            ofClear(255,255,255, 255);
            // シェーダ有効化
            m_Shader.begin();
            {
                // 行列をプッシュ
                ofPushMatrix();
                {
                    // 行列取得
                    const ofMatrix4x4 modelMat  = m_BaselFace.GetModelMatrixConst();
                    // 移動
                    ofTranslate(modelMat.getTranslation());
                    // メッシュを描画
                    m_BaselFace.GetMesh().draw();
                    
                }
                ofPopMatrix();
            }
            m_Shader.end();
        }
        m_Fbo.end();
    }
    m_Camera.end();
    
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

    // ランダムでPCAモデル生成
    m_BaselFace.DrawRandomSample();
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
