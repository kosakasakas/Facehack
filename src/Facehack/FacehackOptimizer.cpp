//
//  FacehackOptimizer.cpp
//  Facehack
//
//  Created by Takahiro Kosaka on 2016/07/05.
//
//

#include "FacehackOptimizer.hpp"
#include "KSMath.h"

using namespace Kosakasakas;
using namespace Facehack;
using namespace Eigen;

FacehackOptimizer::FacehackOptimizer()
{}

FacehackOptimizer::~FacehackOptimizer()
{}

bool FacehackOptimizer::Initialize(const ParamsPtr& pParam,
                                   const ofTexture& inputTex,
                                   const ofTexture& synthesizedTex,
                                   const float* const alphaVariance,
                                   const float* const betaVariance,
                                   const float* const deltaVarinace)
{
    m_pParam            = pParam;
    m_InputTex          = inputTex;
    m_SynthesizedTex    = synthesizedTex;
    m_AlphaValiance     = Map<const AlphaCoeffArray>(alphaVariance);
    m_BetaValiance      = Map<const BetaCoeffArray>(betaVariance);
    m_DeltaValiance     = Map<const DeltaCoeffArray>(deltaVarinace);
    return true;
}

void    FacehackOptimizer::Finalize()
{}

void    FacehackOptimizer::Update(const ofTexture& inputTex)
{
    m_InputTex  = inputTex;
}

bool    FacehackOptimizer::Solve()
{
    return true;
}

float   FacehackOptimizer::GetPhotoConsistency()
{
    ofPixels inPixs, synthesizedPixs;
    m_InputTex.readToPixels(inPixs);
    m_SynthesizedTex.readToPixels(synthesizedPixs);
    
    float total = 0.0f;
    int num     = inPixs.size();
    for (int i=0; i<num; ++i)
    {
        char diff   = inPixs[i] - synthesizedPixs[i];
        total += abs((float)diff/ 255.0f);
    }
    total /= (float)num;
    return total;
}

float   FacehackOptimizer::GetFeatureAlignment()
{
    return 0.0;
}

float   FacehackOptimizer::GetStatisticalRegularization()
{
    return 0.0;
}
