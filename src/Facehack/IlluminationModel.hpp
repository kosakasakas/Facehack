//
//  IlluminationModel.hpp
//  Facehack
//
//  Created by Takahiro Kosaka on 2016/07/04.
//
//

#ifndef IlluminationModel_hpp
#define IlluminationModel_hpp

//#include <array>
#include "KSMath.h"

namespace Facehack {
    
    const int GAMMA_COEFF_NUM = 12;
    typedef Eigen::Matrix<float, GAMMA_COEFF_NUM, 1>    GammaCoeffArray;
    
    class IlluminationModel
    {
    public:
        IlluminationModel();
        virtual ~IlluminationModel();
        
        bool    Initialize(const float* const coeffsR,
                           const float* const coeffsG,
                           const float* const coeffsB);
        bool    Initialize();
        void    Finalize();
        
        void    Update(GammaCoeffArray& coeffsR,
                       GammaCoeffArray& coeffsG,
                       GammaCoeffArray& coeffsB);
        
        inline const GammaCoeffArray&  GetGammaCoeffsR() const
        {
            return m_aGammaCoeffsR;
        }
        
        inline const GammaCoeffArray&  GetGammaCoeffsG() const
        {
            return m_aGammaCoeffsG;
        }
        
        inline const GammaCoeffArray&  GetGammaCoeffsB() const
        {
            return m_aGammaCoeffsB;
        }
    private:
        GammaCoeffArray m_aGammaCoeffsR;
        GammaCoeffArray m_aGammaCoeffsG;
        GammaCoeffArray m_aGammaCoeffsB;
    };
}

#endif /* IlluminationModel_hpp */
