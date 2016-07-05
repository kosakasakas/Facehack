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
        bool    Initialize(const float* const pos,
                           const float* const lookAt,
                           const float* const upVec,
                           float fov,
                           float aspectRatio);
        bool    Initialize();
        void    Finalize();
        
        void    Update(float* aPos, float* aLookAt, float fov);
        
        inline const ofCamera&  GetCamera() const
        {
            return m_Cam;
        }
        
        inline const ofVec3f& GetPosition() const
        {
            return m_Position;
        }
        
    private:
        //! カメラ
        ofCamera    m_Cam;
        //!
        ofVec3f     m_Position;
        ofVec3f     m_LookAt;
        ofVec3f     m_UpVec;
        float       m_Fov;
        float       m_AspectRatio;
    };
}

#endif /* CameraModel_hpp */
