#include "ofApp.h"
#include "ofTest.h"
#include "ofxTimeMeasurements.h"
#include "System/Util/KSUtil.h"

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
    
    // シェーダのロード
    m_Shader.load("shader/test.vert", "shader/test.frag");
    
    // これをしないとUVがピクセル座標になってしまう
    ofDisableArbTex();
    
    // 画像ロード
    //m_Image.load("image/alice.jpg");
    
    // オフスクリーンレンダリング用のFBO確保
    {
        ofFbo::Settings setting;
        setting.width   = 512;
        setting.height  = 512;
        setting.internalformat  = GL_RGBA8;
        setting.numColorbuffers = 1;
        setting.useDepth    = true;
        setting.useStencil  = true;
        setting.depthStencilInternalFormat  = GL_DEPTH24_STENCIL8;
        setting.depthStencilAsTexture   = true;
        m_Fbo.allocate(setting);
    }
    
    // バーセルPCAモデルのセットアップ
    {
        // モデルのロード
        // 権利上、モデルは配布できないので自分で取得してunpublicディレクトリに置いてください.
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
        m_Camera.setPosition(0.0f, 0.0f, 500.0f);
        m_Camera.lookAt(ofVec3f(0.0f, 0.0f, 0.0f), ofVec3f(0.0, 1.0, 0.0));
        m_Camera.setFov(120.0f);
        m_Camera.setAspectRatio(1.0);
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    // デプステスト有効化
    ofEnableDepthTest();
    
    // トランスフォーム処理
    //float spinX = sin(ofGetElapsedTimef()*.35f);
    //float spinY = cos(ofGetElapsedTimef()*.075f);
    
    // 自前で行列を計算しておく
    const ofMatrix4x4 modelMat      = m_BaselFace.GetModelMatrixConst();
    const ofMatrix4x4 viewProjMat   = m_Camera.getModelViewProjectionMatrix();
    ofMatrix4x4 m;
    ofQuaternion q;
    q.makeRotate(120.0f, ofVec3f(1.0,0.0,0.0));
    q.makeRotate(90.0f, ofVec3f(0.0,0.0,1.0));
    m.rotate(q);
    ofMatrix4x4 mvp = viewProjMat * m;
    
    // カメラを使う
    {
        // オフスクリーン描画
        m_Fbo.begin();
        {
            glEnable(GL_CULL_FACE);
            glCullFace(GL_FRONT);
            // カラー初期化
            ofClear(255,255,255, 255);
            // シェーダ有効化
            m_Shader.begin();
            {
                // 行列をシェーダに渡す
                m_Shader.setUniformMatrix4f("u_mvp", mvp);
                
                // 行列をプッシュ
                ofPushMatrix();
                {
                    m_Camera.begin();
                    // メッシュを描画
                    m_BaselFace.GetMesh().draw();
                    m_Camera.end();
                    
                }
                ofPopMatrix();
            }
            m_Shader.end();
        }
        m_Fbo.end();
    }
    
    // フレームバッファを描画
    m_Fbo.draw(0, 0);
    
    // デプスバッファの表示
    m_Fbo.getDepthTexture().draw(512, 0, 256, 256);
    ofDrawBitmapString( "depth buffer", 512, 10);
    
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
