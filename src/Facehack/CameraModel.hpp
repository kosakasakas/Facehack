//
//  CameraModel.hpp
//  Facehack
//
//  Created by Takahiro Kosaka on 2016/07/04.
//
//

#ifndef CameraModel_hpp
#define CameraModel_hpp

#include "ofMain.h"
#include "KSMath.h"

namespace Facehack {
    class CameraModel
    {
    public:
        CameraModel();
        virtual ~CameraModel();
        bool    Initialize(float XClip, float YClip);
        bool    Initialize();
        void    Finalize();
        
        void    Update(float XClip, float YClip);
        
        inline const ofCamera&  GetCamera() const
        {
            return m_Cam;
        }
        
        inline float GetXClip() const
        {
            return m_XClip;
        }
        
        inline float GetYClip() const
        {
            return m_YClip;
        }
    private:
        //! カメラ
        ofCamera    m_Cam;
        //! カメラのクリップのX軸との交点
        float m_XClip;
        //! カメラのクリップのY軸との交点
        float m_YClip;
        //! ニアクリップ（固定）
        const float m_NearClip  = 0.15f;
        //! ファークリップ（固定）
        const float m_FarClip   = 1000.0f;
    };
}

#endif /* CameraModel_hpp */
