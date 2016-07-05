//
//  FacehackFactory.cpp
//  Facehack
//
//  Created by Takahiro Kosaka on 2016/07/04.
//
//

#include "FacehackFactory.hpp"

using namespace Facehack;
using namespace Kosakasakas;

FacehackFactory::FacehackFactory()
{}

FacehackFactory::~FacehackFactory()
{}

bool    FacehackFactory::Initialize(int width, int height)
{
    m_Width     = width;
    m_Height    = height;
    return true;
}

void    FacehackFactory::Finalize()
{}

FacehackModelPtr   FacehackFactory::Create(const ParamsPtr& pParam)
{
    // Facialモデル構築
    FacialModelPtr  pFacial = make_shared<FacialModel>();
    if (!pFacial->Initialize(pParam->GetAlphaCoeffs(),
                             pParam->GetBetaCoeffs(),
                             pParam->GetDeltaCoeffs(),
                             pParam->GetFacialQuaternion(),
                             pParam->GetFacialTransform()))
    {
        ofLog(OF_LOG_ERROR, "フェイシャルモデルの構築に失敗しました.");
        return nullptr;
    }
    
    // イルミネーションモデル構築
    IlluminationModelPtr pIllumination  = make_shared<IlluminationModel>();
    if (!pIllumination->Initialize(pParam->GetGammaCoeffsR(),
                                   pParam->GetGammaCoeffsG(),
                                   pParam->GetGammaCoeffsB()))
    {
        ofLog(OF_LOG_ERROR, "イルミネーションモデルの構築に失敗しました.");
        return nullptr;
    }
    
    // カメラモデルの構築
    const float upVec[] = {0.0f, 1.0f, 0.0f};
    CameraModelPtr pCamera  = make_shared<CameraModel>();
    if (!pCamera->Initialize(pParam->GetCameraPosition(),
                             pParam->GetCameraLookAt(),
                             upVec,
                             pParam->GetCameraFov(),
                             pParam->GetCameraAspectRatio()))
    {
        ofLog(OF_LOG_ERROR, "カメラモデルの構築に失敗しました.");
        return nullptr;
    }
    
    // Facehackモデルの構築
    FacehackModelPtr pFacehack  = make_shared<FacehackModel>();
    if (!pFacehack->Initialize(m_Width,
                               m_Height,
                               pCamera,
                               pIllumination,
                               pFacial))
    {
        ofLog(OF_LOG_ERROR, "Facehackモデルの構築に失敗しました.");
        return nullptr;
    }
    
    return pFacehack;
}

FacehackModelPtr   FacehackFactory::Create()
{
    // Facialモデル構築
    FacialModelPtr  pFacial = make_shared<FacialModel>();
    if (!pFacial->Initialize())
    {
        ofLog(OF_LOG_ERROR, "フェイシャルモデルの構築に失敗しました.");
        return nullptr;
    }
    
    // イルミネーションモデル構築
    IlluminationModelPtr pIllumination  = make_shared<IlluminationModel>();
    if (!pIllumination->Initialize())
    {
        ofLog(OF_LOG_ERROR, "イルミネーションモデルの構築に失敗しました.");
        return nullptr;
    }
    
    // カメラモデルの構築
    const float upVec[] = {0.0f, 1.0f, 0.0f};
    CameraModelPtr pCamera  = make_shared<CameraModel>();
    if (!pCamera->Initialize())
    {
        ofLog(OF_LOG_ERROR, "カメラモデルの構築に失敗しました.");
        return nullptr;
    }
    
    // Facehackモデルの構築
    FacehackModelPtr pFacehack  = make_shared<FacehackModel>();
    if (!pFacehack->Initialize(m_Width,
                               m_Height,
                               pCamera,
                               pIllumination,
                               pFacial))
    {
        ofLog(OF_LOG_ERROR, "Facehackモデルの構築に失敗しました.");
        return nullptr;
    }
    
    return pFacehack;
}
