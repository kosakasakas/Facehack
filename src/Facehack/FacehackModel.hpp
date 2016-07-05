//
//  FacehackModel.hpp
//  Facehack
//
//  Created by Takahiro Kosaka on 2016/07/04.
//
//

#ifndef FacehackModel_hpp
#define FacehackModel_hpp

#include "CameraModel.hpp"
#include "IlluminationModel.hpp"
#include "FacialModel.hpp"
#include <vector>
#include <memory>
#include "FacehackParams.hpp"

namespace Facehack {
    
    typedef std::shared_ptr<FacialModel> FacialModelPtr;
    typedef std::shared_ptr<CameraModel> CameraModelPtr;
    typedef std::shared_ptr<IlluminationModel> IlluminationModelPtr;
    typedef std::shared_ptr<FacehackParams> ParamsPtr;
    
    class FacehackModel
    {
    public:
        FacehackModel();
        virtual ~FacehackModel();
        bool    Initialize(int width,
                           int height,
                           const CameraModelPtr& pCam,
                           const IlluminationModelPtr& pIllumination,
                           const FacialModelPtr& pFacial);
        void    Finalize();
        
        void    Update(ParamsPtr& pParam);
        void    Draw();
        
        inline const ofTexture& GetTexture() const
        {
            return m_Fbo.getTexture();
        }
        
        void    SetSourceTexture(const ofTexture& srcTex);
        
    private:
        float   GetPhotoConsistency();
        float   GetFeatureAlignment();
        float   GetStatisticalRegularization();
        
    private:
        FacialModelPtr  m_pFacialModel;
        CameraModelPtr  m_pCameraModel;
        IlluminationModelPtr    m_pIlluminationModel;
        ofShader        m_Shader;
        ofFbo           m_Fbo;
        ofTexture       m_SrcTex;
    };
}

#endif /* FacehackModel_hpp */
