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
    // =========================================================================
    // E(P) = w_col * E_col(P) + w_lan * E_lan(P) + w_reg * E_reg(P) を最適化する.
    // =========================================================================
    
    int paramNum = FacehackParams::DataLayout::TOTAL_NUM;
    
    KSMatrixXf data(2, paramNum);
    for (int i = 0; i < paramNum; ++i)
    {
        //data.insert(0, i) = m_pParam.get
    }
    data.row(0) = m_pParam->GetParams();
    
    /*
    // オプティマイザの宣言
    KSDenseOptimizer  optimizer;
    
    // 残差関数
    KSFunction  residual    = [&optimizer](const KSMatrixXd &x)->KSMatrixXd
    {
        const KSMatrixXd& data = optimizer.GetDataMat();
        KSMatrixXd r(data.cols(), 1);
        
        for(int i=0, n=r.rows(); i<n; ++i)
        {
            // r = y-(ax+b)
            r(i)    = data(1,i) - (x(0) * data(0, i) + x(1));
        }
        
        return r;
    };
    
    // 残差のヤコビアン
    KSFunction jacobian     = [&optimizer](const KSMatrixXd &x)->KSMatrixXd
    {
        const KSMatrixXd& data = optimizer.GetDataMat();
        KSMatrixXd d(data.cols(), x.rows());
        
        for(int i=0,n=d.rows(); i<n; ++i)
        {
            // d0 = -x
            // d1 = -1
            d(i, 0)         = -data(0,i);
            d(i, 1)         = -1;
        }
        
        return d;
    };
    
    // パラメータ行列の初期値を設定
    KSMatrixXd param0(2,1);
    param0 << 5.0, 5.0;
    
    // オプティマイザの初期化
    optimizer.Initialize(residual, jacobian, param0, data);
    
    // 計算ステップ5回
    int numStep = 5;
    
    // 計算開始(通常計算)
    TS_START("optimization exmple 1-1");
    for (int i = 0; i < numStep; ++i)
    {
        if (!optimizer.DoGaussNewtonStep())
        {
            ofLog(OF_LOG_ERROR, "ガウス-ニュートン計算ステップに失敗しました。");
            return false;
        }
    }
    TS_STOP("optimization exmple 1-1");
    
    // 解の確認
    ofLog(OF_LOG_NOTICE,
          "ex1-1: param0: %lf, param1: %lf",
          optimizer.GetParamMat()(0),
          optimizer.GetParamMat()(1));
    
    // パラメータ行列の初期値を再設定
    KSMatrixXd param1(2,1);
    param1 << 5.0, 5.0;
    optimizer.SetParamMat(param1);
    
    // 計算開始(IRLS計算)
    TS_START("optimization exmple 1-2");
    for (int i = 0; i < numStep; ++i)
    {
        if (!optimizer.DoGaussNewtonStepIRLS())
        {
            ofLog(OF_LOG_ERROR, "ガウス-ニュートン計算ステップに失敗しました。");
            return false;
        }
    }
    TS_STOP("optimization exmple 1-2");
    
    // 解の確認
    ofLog(OF_LOG_NOTICE,
          "ex1-2: param0: %lf, param1: %lf",
          optimizer.GetParamMat()(0),
          optimizer.GetParamMat()(1));
    */
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
