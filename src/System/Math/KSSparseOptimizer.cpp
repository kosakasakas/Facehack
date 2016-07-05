//
//  KSSparseOptimizer.cpp
//
//  スパース行列による非線形最小二乗問題のための最適化計算クラス
//
//  Copyright (c) 2016年 Takahiro Kosaka. All rights reserved.
//  Created by Takahiro Kosaka on 2016/05/23.
//
//  This Source Code Form is subject to the terms of the Mozilla
//  Public License v. 2.0. If a copy of the MPL was not distributed
//  with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "KSSparseOptimizer.h"

using namespace Kosakasakas;

// コンストラクタ
KSSparseOptimizer::KSSparseOptimizer()
: m_IsInitialized(false)
, m_pNESolver(nullptr)
, m_MaxIterations(4)
{}

// デストラクタ
KSSparseOptimizer::~KSSparseOptimizer()
{}

// 初期化
bool    KSSparseOptimizer::Initialize(KSFunctionSparse& residual,
                                     KSFunctionSparse& jaconian,
                                     KSMatrixSparsef& initParam,
                                     KSMatrixSparsef& data)
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
    
    if (!m_pNESolver)
    {
        m_pNESolver     = m_NESolverFactory.Create(NESolverType::CHOLESKY);
    }
    
    return true;
}

// 最適化ステップの実行（ガウス-ニュートン法）
bool    KSSparseOptimizer::DoGaussNewtonStep()
{
    if (!m_IsInitialized || !m_pNESolver)
    {
        return false;
    }
    
    KSMatrixSparsef j    = m_FuncJacobian(m_MatParam);
    if (j.rows() < j.cols())
    {
        return false;
    }
    
    KSMatrixSparsef y    = m_FuncResidual(m_MatParam);
    
    return m_pNESolver->Solve(m_MatParam, y, j, m_MaxIterations);
}

// IRLS最適化ステップの実行（ガウス-ニュートン法）
bool    KSSparseOptimizer::DoGaussNewtonStepIRLS()
{
    if (!m_IsInitialized)
    {
        return false;
    }
    
    KSMatrixSparsef j    = m_FuncJacobian(m_MatParam);
    if (j.rows() < j.cols())
    {
        return false;
    }
    KSMatrixSparsef y    = m_FuncResidual(m_MatParam);
    
    // IRLS用のweightを算出して乗算
    KSVectorXf v = y.col(0);
    auto w = v.cwiseAbs().cwiseMax(0.00001).cwiseInverse().asDiagonal();
    for (int i=0,n=j.cols(); i<n; ++i)
    {
        j.col(i) = w * j.col(i);
    }
    y = w * y;
    
    return m_pNESolver->Solve(m_MatParam, y, j, m_MaxIterations);
}

// 残差平方和の取得
double KSSparseOptimizer::GetSquaredResidualsSum()
{
    KSMatrixSparsef m   = m_FuncResidual(m_MatParam);
    KSMatrixSparsef mt  = m.transpose();
    KSMatrixSparsef res = mt * m;
    return fabs(res.coeff(0, 0));
}

// 正規方程式ソルバの変更
void    KSSparseOptimizer::SwitchNormalEquationSolver(NESolverType type)
{
    m_pNESolver = m_NESolverFactory.Create(type);
}


