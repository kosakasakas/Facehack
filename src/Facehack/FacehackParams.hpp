//
//  FacehackParams.hpp
//  Facehack
//
//  Created by Takahiro Kosaka on 2016/07/04.
//
//

#ifndef FacehackParams_hpp
#define FacehackParams_hpp

#include "ofMain.h"
#include "KSMath.h"
#include "FacialModel.hpp"
#include "IlluminationModel.hpp"

namespace Facehack {
    
    class FacehackParams
    {
    public:
        FacehackParams();
        virtual ~FacehackParams();
        bool    Initialize(const ofVec3f& camPos,
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
                           const ofVec3f& faceTrans);
        bool    Initialize();
        void    Finalize();
        
        // カメラモデル用
        const float* const  GetCameraPosition() const;
        const float* const  GetCameraLookAt() const;
        const float         GetCameraFov() const;
        const float         GetCameraAspectRatio() const;
        
        void    SetCameraPosition(const ofVec3f& pos);
        void    SetCameraLookAt(const ofVec3f& lookAt);
        void    SetCameraFov(float fov);
        void    SetCameraAspectRatio(float ratio);
        
        // イルミネーション用
        const float* const  GetGammaCoeffsR() const;
        const float* const  GetGammaCoeffsG() const;
        const float* const  GetGammaCoeffsB() const;
        
        void    SetGammaCoeffsR(const Kosakasakas::KSVectorXf& coeffs);
        void    SetGammaCoeffsG(const Kosakasakas::KSVectorXf& coeffs);
        void    SetGammaCoeffsB(const Kosakasakas::KSVectorXf& coeffs);
        
        // ファイシャルモデル用
        const float* const  GetAlphaCoeffs() const;
        const float* const  GetBetaCoeffs() const;
        const float* const  GetDeltaCoeffs() const;
        const float* const  GetFacialQuaternion() const;
        const float* const  GetFacialTransform() const;
        
        void    SetAlphaCoeffs(const Kosakasakas::KSVectorXf& coeffs);
        void    SetBetaCoeffs(const Kosakasakas::KSVectorXf& coeffs);
        void    SetDeltaCoeffs(const Kosakasakas::KSVectorXf& coeffs);
        void    SetFacialQuaternion(const ofQuaternion& quat);
        void    SetFacialTransform(const ofVec3f& trans);
        
        const Kosakasakas::KSVectorXf&  GetParams() const
        {
            return m_pParams;
        }
        
    private:
        //! パラメータ配列のレイアウト
        enum DataLayout
        {
            CAM_POS     = 0,
            CAM_LOOKAT  = CAM_POS       + 3,
            CAM_FOV     = CAM_LOOKAT    + 3,
            CAM_ASPECT  = CAM_FOV       + 1,
            GAMMA_R     = CAM_ASPECT    + 1,
            GAMMA_G     = GAMMA_R       + GAMMA_COEFF_NUM,
            GAMMA_B     = GAMMA_G       + GAMMA_COEFF_NUM,
            ALPHA       = GAMMA_B       + GAMMA_COEFF_NUM,
            BETA        = ALPHA         + ALPHA_COEFF_NUM,
            DELTA       = BETA          + BETA_COEFF_NUM,
            FACE_QUAT   = DELTA         + DELTA_COEFF_NUM,
            FACE_TRANS  = FACE_QUAT     + 4,
            
            TOTAL_NUM   = FACE_TRANS    + 3
        };
        
    private:
        //! パラメータ配列
        //Kosakasakas::KSVectorXf  m_pParams;
        typedef Eigen::Matrix<float, TOTAL_NUM, 1> ParamVec;
        ParamVec m_pParams;
        
        // キャッシュしておく（実行時に変更しない）パラメータ
        Kosakasakas::KSVectorXf     m_pAlphaCoeffs;
        Kosakasakas::KSVectorXf     m_pBetaCoeffs;
    };
}

#endif /* FacehackParams_hpp */
