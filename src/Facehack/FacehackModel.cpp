//
//  FacehackModel.cpp
//  Facehack
//
//  Created by Takahiro Kosaka on 2016/07/04.
//
//

#include "FacehackModel.hpp"

using namespace Facehack;
using namespace Kosakasakas;

FacehackModel::FacehackModel()
{}

FacehackModel::~FacehackModel()
{}

bool    FacehackModel::Initialize(int width,
                                  int height,
                                  const CameraModelPtr& pCam,
                                  const IlluminationModelPtr& pIllumination,
                                  const FacialModelPtr& pFacial)
{
    m_pCameraModel  = pCam;
    m_pIlluminationModel    = pIllumination;
    m_pFacialModel       = pFacial;
    
    m_Shader.load("shader/test.vert", "shader/test.frag");
    
    // 画像描画用のFBO確保
    {
        ofFbo::Settings setting;
        setting.width           = width;
        setting.height          = height;
        setting.internalformat  = GL_RGBA8;
        setting.numColorbuffers = 1;
        setting.useDepth        = true;
        setting.useStencil      = true;
        setting.depthStencilInternalFormat  = GL_DEPTH24_STENCIL8;
        setting.depthStencilAsTexture       = true;
        m_Fbo.allocate(setting);
    }

    return true;
}

void    FacehackModel::Finalize()
{}

void    FacehackModel::Update()
{
}

void    FacehackModel::Update(ParamsPtr& pParams)
{
}

void    FacehackModel::Draw()
{
    ofCamera cam  = m_pCameraModel->GetCamera();
    
    // デプステストを有効化
    ofEnableDepthTest();
    
    // フレームバッファを有効化
    m_Fbo.begin();
    {
        // カラー初期化
        ofClear(0, 0, 0, 255);
        // シェーダ有効化
        m_Shader.begin();
        {
            // 行列をプッシュ
            ofPushMatrix();
            {
                cam.begin();
                {
                    // 平行移動
                    ofTranslate(m_pFacialModel->GetTransform());
                    
                    // 回転
                    float angle, x, y, z;
                    m_pFacialModel->GetQuaternion().getRotate(angle, x, y, z);
                    ofRotate(angle, x, y, z);
                    
                    // メッシュを描画
                    m_pFacialModel->GetMesh().draw();
                }
                cam.end();
            }
            ofPopMatrix();
        }
        m_Shader.end();
    }
    m_Fbo.end();

    ofDisableDepthTest();
}

void    FacehackModel::SetSourceTexture(const ofTexture& srcTex)
{
    m_SrcTex = srcTex;
}
