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

bool    CameraModel::Initialize(float XClip, float YClip)
{
    m_XClip = XClip;
    m_YClip = YClip;
    
    float fov = ofRadToDeg(2.0f * atanf(m_YClip / m_NearClip));
    float aspect = m_XClip / m_YClip;
    m_Cam.setNearClip(m_NearClip);
    m_Cam.setFarClip(m_FarClip);
    m_Cam.setFov(fov);
    m_Cam.setAspectRatio(aspect);
    m_Cam.setPosition(0.0f, 0.0f, 0.0f); // カメラは常に原点に置いておく。
    
    return true;
}

void    CameraModel::Finalize()
{}

void    CameraModel::Update(float XClip, float YClip)
{
}