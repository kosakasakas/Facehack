//
//  FacehackOptimizer.hpp
//  Facehack
//
//  Created by Takahiro Kosaka on 2016/07/05.
//
//

#ifndef FacehackOptimizer_hpp
#define FacehackOptimizer_hpp

#include "ofMain.h"
#include "FacialModel.hpp"
#include "FacehackParams.hpp"
#include "FacehackFactory.hpp"

namespace Facehack {
    
    class FacehackOptimizer
    {
    public:
        FacehackOptimizer();
        virtual ~FacehackOptimizer();
        
        bool    Initialize(const ParamsPtr& pParam,
                           const ofTexture& inputTex,
                           const ofTexture& synthesizedTex,
                           const float* const alphaVariance,
                           const float* const betaVariance,
                           const float* const deltaVarinace);
        void    Finalize();
        void    Update(const ofTexture& inputTex);
        bool    Solve();
        
    private:
        float   GetPhotoConsistency();
        float   GetFeatureAlignment();
        float   GetStatisticalRegularization();
        
    private:
        const float W_col   = 1.0f;
        const float W_lan   = 10.0f;
        const float W_reg   = 0.000025f;
        
        ParamsPtr   m_pParam;
        ofTexture   m_InputTex;
        ofTexture   m_SynthesizedTex;
        AlphaCoeffArray m_AlphaValiance;
        BetaCoeffArray  m_BetaValiance;
        DeltaCoeffArray m_DeltaValiance;
    };
}

#endif /* FacehackOptimizer_hpp */
