//
//  FacialModel.hpp
//  Facehack
//
//  Created by Takahiro Kosaka on 2016/07/04.
//
//

#ifndef FacialModel_hpp
#define FacialModel_hpp

#include "ofMain.h"
#include <array>
#include "KSMath.h"
#include "ofKsBaselFaceModel.hpp"

namespace Facehack {
    
    typedef std::shared_ptr<Kosakasakas::ofKsBaselFaceModel> BaselModelPtr;
    
    const int ALPHA_COEFF_NUM    = 80;
    const int BETA_COEFF_NUM     = 80;
    const int DELTA_COEFF_NUM    = 46;
    
    typedef Eigen::Matrix<float, ALPHA_COEFF_NUM, 1>    AlphaCoeffArray;
    typedef Eigen::Matrix<float, BETA_COEFF_NUM, 1>     BetaCoeffArray;
    typedef Eigen::Matrix<float, DELTA_COEFF_NUM, 1>    DeltaCoeffArray;
    
    class FacialModel
    {
    public:
        FacialModel();
        virtual ~FacialModel();
        
        bool    Initialize(const float* const alphaCoeffs,
                           const float* const betaCoeffs,
                           const float* const deltaCoeffs,
                           const float* const rotation,
                           const float* const transform);
        bool    Initialize();
        
        void    Finalize();
        
        void    Update(DeltaCoeffArray& deltaCoeffs,
                       float*           rotation,
                       float*           transform);
        
        inline const ofMesh&    GetMesh() const
        {
            return m_pBaselModel->GetMesh();
        }
        
        inline const ofVec3f&    GetTransform() const
        {
            return m_Transform;
        }
        
        inline const ofQuaternion& GetQuaternion() const
        {
            return m_Quat;
        }
        
    private:
        //! バーセルモデル
        BaselModelPtr m_pBaselModel;
        
        //! 回転成分
        ofQuaternion    m_Quat;
        //! 平行移動成分
        ofVec3f     m_Transform;
        
        //! PCAの主成分ベクトル（シェイプ）
        AlphaCoeffArray m_aAlphaCoeffs;
        //! PCAの主成分ベクトル（アルベド）
        BetaCoeffArray  m_aBetaCoeffs;
        //! 表情キーシェイプの重みベクトル
        DeltaCoeffArray m_aDeltaCoeffs;
        
    };
}

#endif /* FacialModel_hpp */
