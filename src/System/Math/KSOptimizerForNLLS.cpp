//
//  KSOptimizerForNLLS.h
//
//  非線形最小二乗問題のための最適化計算クラス
//
//  Copyright (c) 2016年 Takahiro Kosaka. All rights reserved.
//  Created by Takahiro Kosaka on 2016/04/05.
//
//  This Source Code Form is subject to the terms of the Mozilla
//  Public License v. 2.0. If a copy of the MPL was not distributed
//  with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

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
    
    if (!m_NESolverFactory.Initialize())
    {
        return false;
    }
    m_pNESolver     = m_NESolverFactory.Create(NESolverType::CHOLESKY);

    return true;
}

// 最適化ステップの実行（ガウス-ニュートン法）
bool    KSOptimizerForNLLS::DoGaussNewtonStep()
{
    if (!m_IsInitialized || !m_pNESolver)
    {
        return false;
    }
    
    KSMatrixXd j    = m_FuncJacobian(m_MatParam);
    if (j.rows() < j.cols())
    {
        return false;
    }
    
    KSMatrixXd y    = m_FuncResidual(m_MatParam);
    
    return m_pNESolver->Solve(m_MatParam, y, j);
}

// IRLS最適化ステップの実行（ガウス-ニュートン法）
bool    KSOptimizerForNLLS::DoGaussNewtonStepIRLS()
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
    KSMatrixXd y    = m_FuncResidual(m_MatParam);
    
    // IRLS用のweightを算出して乗算
    KSMatrixXd w    = y.cwiseAbs().cwiseMax(0.00001).cwiseInverse().asDiagonal();
    for (int i=0,n=j.cols(); i<n; ++i)
    {
        j.col(i) = w * j.col(i);
    }
    y = w * y;
    
    return m_pNESolver->Solve(m_MatParam, y, j);
}

// 残差平方和の取得
double KSOptimizerForNLLS::GetSquaredResidualsSum()
{
    return fabs((m_FuncResidual(m_MatParam).transpose() * m_FuncResidual(m_MatParam))(0));
}

// 正規方程式ソルバの変更
void    KSOptimizerForNLLS::SwitchNormalEquationSolver(NESolverType type)
{
    m_pNESolver = m_NESolverFactory.Create(type);
}

