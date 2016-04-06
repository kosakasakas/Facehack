//
//  KSOptimizerForNLLS.h
//
//  非線形最小二乗問題のための最適化計算クラス
//
//  Copyright (c) 2016年 Takahiro Kosaka. All rights reserved.
//  Created by Takahiro Kosaka on 2016/04/05.
//
//

#include "KSOptimizerForNLLS.h"

using namespace Kosakasakas;

// コンストラクタ
KSOptimizerForNLLS::KSOptimizerForNLLS()
: m_IsInitialized(false)
{}

// デストラクタ
KSOptimizerForNLLS::~KSOptimizerForNLLS()
{}

// 初期化
bool    KSOptimizerForNLLS::Initialize(KSFunction& residual,
                                       KSFunction& jaconian,
                                       KSMatrixXd& initParam,
                                       KSMatrixXd& data)
{
    m_FuncResidual  = std::move(residual);
    m_FuncJacobian  = std::move(jaconian);
    m_MatParam      = std::move(initParam);
    m_MatData       = std::move(data);
    m_IsInitialized = true;

    return true;
}

// 最適化ステップの実行（ガウス-ニュートン法）
bool    KSOptimizerForNLLS::DoGaussNewtonStep()
{
    if (!m_IsInitialized)
    {
        return false;
    }
    
    KSMatrixXd j    = m_FuncJacobian(m_MatParam);
    if (j.rows() < j.cols())
    {
        return false;
    }
    KSMatrixXd jt   = j.transpose();
    KSMatrixXd y    = m_FuncResidual(m_MatParam);
    
    auto llt = (jt * j).ldlt();
    if (llt.info()  != Eigen::Success)
    {
        return false;
    }

    KSMatrixXd s    = llt.solve(jt * y * -1.0);
    m_MatParam   = m_MatParam + s;
    
    return true;
}

// 残差平方和の取得
double KSOptimizerForNLLS::GetSquaredResidualsSum()
{
    return fabs((m_FuncResidual(m_MatParam).transpose() * m_FuncResidual(m_MatParam))(0));
}
