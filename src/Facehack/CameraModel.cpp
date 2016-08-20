//
//  CameraModel.cpp
//  Facehack
//
//  Created by Takahiro Kosaka on 2016/07/04.
//
//

#include "CameraModel.hpp"

using namespace Facehack;

CameraModel::CameraModel()
{}

CameraModel::~CameraModel()
{}

bool    CameraModel::Initialize(const float* const pos,
                                const float* const lookAt,
                                const float* const upVec,
                                float fov,
                                float aspectRatio)
{
    m_Position      = ofVec3f(*pos, *(pos+1), *(pos+2));
    m_LookAt        = ofVec3f(*lookAt, *(lookAt+1), *(lookAt+2));
    m_UpVec         = ofVec3f(*upVec, *(upVec+1), *(upVec+2));
    m_Fov           = fov;
    m_AspectRatio   = aspectRatio;
    
    m_Cam.setPosition(m_Position);
    m_Cam.lookAt(m_LookAt, m_UpVec);
    m_Cam.setFov(m_Fov);
    m_Cam.setAspectRatio(m_AspectRatio);
    m_Cam.setNearClip(0.1f);
    m_Cam.setFarClip(1000.0f);
    
    return true;
}


bool    CameraModel::Initialize()
{
    m_Position      = ofVec3f(0.0, 0.0, 500.0);
    m_LookAt        = ofVec3f(0.0, 0.0, 0.0);
    m_UpVec         = ofVec3f(0.0, 1.0, 0.0);
    m_Fov           = 60.0f;
    m_AspectRatio   = 3.0 / 2.0;
    
    m_Cam.setPosition(m_Position);
    m_Cam.lookAt(m_LookAt, m_UpVec);
    m_Cam.setFov(m_Fov);
    m_Cam.setAspectRatio(m_AspectRatio);
    m_Cam.setNearClip(0.1f);
    m_Cam.setFarClip(1000.0f);
    
    return true;
}

void    CameraModel::Finalize()
{}

void    CameraModel::Update(float* pos,
                            float* lookAt,
                            float fov)
{
}