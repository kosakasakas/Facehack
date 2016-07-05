//
//  FacehackParams.cpp
//  Facehack
//
//  Created by Takahiro Kosaka on 2016/07/04.
//
//

#include "FacehackParams.hpp"

using namespace Kosakasakas;
using namespace Facehack;

FacehackParams::FacehackParams()
{}

FacehackParams::~FacehackParams()
{}

bool    FacehackParams::Initialize(const ofVec3f& camPos,
                                   const ofVec3f& camLookAt,
                                   float camFov,
                                   float camRatio,
                                   const GammaCoeffArray& gammaCoeffsR,
                                   const GammaCoeffArray& gammaCoeffsG,
                                   const GammaCoeffArray& gammaCoeffsB,
                                   const AlphaCoeffArray& alPhaCoeffs,
                                   const BetaCoeffArray& betaCoeffs,
                                   const DeltaCoeffArray& deltaCoeffs,
                                   const ofQuaternion& faceQuat,
                                   const ofVec3f& faceTrans)
{
    // データを格納していく
    m_pParams(CAM_POS)      = camPos.x;
    m_pParams(CAM_POS+1)    = camPos.y;
    m_pParams(CAM_POS+2)    = camPos.z;
    
    m_pParams(CAM_LOOKAT)   = camLookAt.x;
    m_pParams(CAM_LOOKAT+1) = camLookAt.y;
    m_pParams(CAM_LOOKAT+2)   = camLookAt.z;
    
    m_pParams(CAM_FOV)      = camFov;
    
    m_pParams(CAM_ASPECT)   = camRatio;

    
    std::memcpy(m_pParams.data()+GAMMA_R,
                gammaCoeffsR.data(),
                sizeof(float) * gammaCoeffsR.size());
    
    std::memcpy(m_pParams.data()+GAMMA_G,
                gammaCoeffsG.data(),
                sizeof(float) * gammaCoeffsG.size());
    
    std::memcpy(m_pParams.data()+GAMMA_B,
                gammaCoeffsB.data(),
                sizeof(float) * gammaCoeffsB.size());
    
    std::memcpy(m_pParams.data()+ALPHA,
                alPhaCoeffs.data(),
                sizeof(float) * alPhaCoeffs.size());
    
    std::memcpy(m_pParams.data()+BETA,
                betaCoeffs.data(),
                sizeof(float) * betaCoeffs.size());
    
    std::memcpy(m_pParams.data()+DELTA,
                deltaCoeffs.data(),
                sizeof(float) * deltaCoeffs.size());
    
    
    m_pParams(FACE_QUAT)    = faceQuat.x();
    m_pParams(FACE_QUAT+1)  = faceQuat.y();
    m_pParams(FACE_QUAT+2)  = faceQuat.z();
    m_pParams(FACE_QUAT+3)  = faceQuat.w();
    
    m_pParams(FACE_TRANS)   = faceTrans.x;
    m_pParams(FACE_TRANS+1)   = faceTrans.y;
    m_pParams(FACE_TRANS+2)   = faceTrans.z;
    
    return true;
}

bool FacehackParams::Initialize()
{
    // テストデータの格納
    m_pParams(CAM_POS)      = 0.0f;
    m_pParams(CAM_POS+1)    = 0.0f;
    m_pParams(CAM_POS+2)    = 500.0f;
    
    m_pParams(CAM_LOOKAT)      = 0.0f;
    m_pParams(CAM_LOOKAT+1)    = 0.0f;
    m_pParams(CAM_LOOKAT+2)    = 0.0f;
    
    m_pParams(CAM_FOV)      = 60.0f;
    
    m_pParams(CAM_ASPECT)    = 3.0f / 2.0f;
    
    std::fill_n(m_pParams.data()+ALPHA,
                ALPHA_COEFF_NUM,
                0.0f);
    
    std::fill_n(m_pParams.data()+BETA,
                BETA_COEFF_NUM,
                0.0f);
    
    std::fill_n(m_pParams.data()+GAMMA_R,
                GAMMA_COEFF_NUM,
                0.0f);
    
    std::fill_n(m_pParams.data()+GAMMA_G,
                GAMMA_COEFF_NUM,
                0.0f);
    std::fill_n(m_pParams.data()+GAMMA_B,
                GAMMA_COEFF_NUM,
                0.0f);
    
    std::fill_n(m_pParams.data()+DELTA,
                DELTA_COEFF_NUM,
                0.0f);
    
    
    m_pParams(FACE_QUAT)    = 0.0f;
    m_pParams(FACE_QUAT+1)  = 0.0f;
    m_pParams(FACE_QUAT+2)  = 0.0f;
    m_pParams(FACE_QUAT+3)  = 1.0f;
    
    m_pParams(FACE_TRANS)   = 0.0f;
    m_pParams(FACE_TRANS+1)   = 0.0f;
    m_pParams(FACE_TRANS+2)   = 0.0f;
    
    return true;
}

const float* const   FacehackParams::GetCameraPosition() const
{
    return (m_pParams.data() + CAM_POS);
}
const float* const  FacehackParams::GetCameraLookAt() const
{
    return (m_pParams.data() + CAM_LOOKAT);
}
const float         FacehackParams::GetCameraFov() const
{
    return *(m_pParams.data() + CAM_FOV);
}
const float         FacehackParams::GetCameraAspectRatio() const
{
    return *(m_pParams.data() + CAM_ASPECT);
}


const float* const  FacehackParams::GetGammaCoeffsR() const
{
    return (m_pParams.data() + GAMMA_R);
}
const float* const  FacehackParams::GetGammaCoeffsG() const
{
    return (m_pParams.data() + GAMMA_G);
}
const float* const  FacehackParams::GetGammaCoeffsB() const
{
    return (m_pParams.data() + GAMMA_B);
}


const float* const  FacehackParams::GetAlphaCoeffs() const
{
    return (m_pParams.data() + ALPHA);
}
const float* const  FacehackParams::GetBetaCoeffs() const
{
    return (m_pParams.data() + BETA);
}
const float* const  FacehackParams::GetDeltaCoeffs() const
{
    return (m_pParams.data() + DELTA);
}
const float* const  FacehackParams::GetFacialQuaternion() const
{
    return (m_pParams.data() + FACE_QUAT);
}
const float* const  FacehackParams::GetFacialTransform() const
{
    return (m_pParams.data() + FACE_TRANS);
}

