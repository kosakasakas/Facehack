//
//  FacialModel.cpp
//  Facehack
//
//  Created by Takahiro Kosaka on 2016/07/04.
//
//

#include "FacialModel.hpp"

using namespace Facehack;
using namespace Kosakasakas;
using namespace Eigen;

FacialModel::FacialModel()
: m_pBaselModel(nullptr)
{}

FacialModel::~FacialModel()
{
    m_pBaselModel   = nullptr;
}

bool    FacialModel::Initialize(const float* const alphaCoeffs,
                                const float* const betaCoeffs,
                                const float* const deltaCoeffs,
                                const float* const rotation,
                                const float* const transform)
{
    if (m_pBaselModel)
    {
        ofLog(OF_LOG_ERROR, "すでに初期化されています.");
        return false;
    }
    
    // バーセルPCAモデルのセットアップ
    {
        m_pBaselModel   = make_shared<ofKsBaselFaceModel>();
        
        // モデルのロード
        // 権利上、モデルは配布できないので自分で取得してunpublicディレクトリに置いてください.
        if(!m_pBaselModel->Initialize("unpublic", "model2009-publicmm1-bfm.h5"))
        {
            ofLog(OF_LOG_ERROR, "バーセルモデルの初期化に失敗しました.");
            return false;
        }
        
        // float配列からKSVectorXf形式に変換
        KSVectorXf shapeCoeff = Map<const AlphaCoeffArray>(alphaCoeffs);
        KSVectorXf albedoCoeff = Map<const AlphaCoeffArray>(betaCoeffs);
        
        // PCAモデルの書き出し
        if(!m_pBaselModel->DrawSample(shapeCoeff, albedoCoeff))
        {
            ofLog(OF_LOG_ERROR, "バーセルモデルのサンプリングに失敗しました.");
            return false;
        }
    }
    
    m_Quat      = ofQuaternion(*rotation, *(rotation+1), *(rotation+2), *(rotation+3));
    m_Transform = ofVec3f(*transform, *(transform+1), *(transform+2));
    
    return true;
}

bool    FacialModel::Initialize()
{
    if (m_pBaselModel)
    {
        ofLog(OF_LOG_ERROR, "すでに初期化されています.");
        return false;
    }
    
    // バーセルPCAモデルのセットアップ
    {
        m_pBaselModel   = make_shared<ofKsBaselFaceModel>();
        
        // モデルのロード
        // 権利上、モデルは配布できないので自分で取得してunpublicディレクトリに置いてください.
        if(!m_pBaselModel->Initialize("unpublic", "model2009-publicmm1-bfm.h5"))
        {
            ofLog(OF_LOG_ERROR, "バーセルモデルの初期化に失敗しました.");
            return false;
        }
        
        // MeanShapeの書き出し
        if(!m_pBaselModel->DrawMean())
        {
            ofLog(OF_LOG_ERROR, "バーセルモデルのサンプリングに失敗しました.");
            return false;
        }
    }
    
    m_Quat      = ofQuaternion(0.0f, 0.0f, 0.0f, 1.0f);
    m_Transform = ofVec3f(0.0f, 0.0f, 0.0f);
    
    return true;
}

void    FacialModel::Finalize()
{
}

void    FacialModel::Update(DeltaCoeffArray& deltaCoeffs,
                            float*           rotation,
                            float*           transform)
{}