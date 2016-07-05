#pragma once

#include "ofMain.h"
//#include "ofxAssimpModelLoader.h"
//#include "ofKsBaselFaceModel.hpp"
#include "FacehackInclude.h"

class ofApp : public ofBaseApp
{
	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
private:
    //! シェーダオブジェクト
    //ofShader    m_Shader;
    
    //! カメラオブジェクト
    //ofCamera    m_Camera;
    
    //! バーセルモデル
    //Kosakasakas::ofKsBaselFaceModel m_BaselFace;
    
    Facehack::FacehackModelPtr m_pSourceModel;
    Facehack::FacehackModelPtr m_pTargetModel;
    
    //! テクスチャ
    ofImage m_Image;
    
    //! オフスクリーンレンダリング用のフレームバッファ
    ofFbo   m_FboSource;
    ofFbo   m_FboOutput;
};
