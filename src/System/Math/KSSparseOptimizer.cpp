//
//  KSSparseOptimizer.cpp
//  Face2Face
//
//  Created by 小坂 昂大 on 2016/05/23.
//
//

#include "KSSparseOptimizer.h"

using namespace Kosakasakas;

// コンストラクタ
KSSparseOptimizer::KSSparseOptimizer()
: m_IsInitialized(false)
{}

// デストラクタ
KSSparseOptimizer::~KSSparseOptimizer()
{}

// 初期化
bool    KSSparseOptimizer::Initialize(KSFunctionSparse& residual,
                                     KSFunctionSparse& jaconian,
                                     KSMatrixSparsed& initParam,
                                     KSMatrixSparsed& data)
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
bool    KSSparseOptimizer::DoGaussNewtonStep()
{
    if (!m_IsInitialized || !m_pNESolver)
    {
        return false;
    }
    
    KSMatrixSparsed j    = m_FuncJacobian(m_MatParam);
    if (j.rows() < j.cols())
    {
        return false;
    }
    
    KSMatrixSparsed y    = m_FuncResidual(m_MatParam);
    
    return m_pNESolver->Solve(m_MatParam, y, j);
}

// IRLS最適化ステップの実行（ガウス-ニュートン法）
bool    KSSparseOptimizer::DoGaussNewtonStepIRLS()
{
    if (!m_IsInitialized)
    {
        return false;
    }
    
    KSMatrixSparsed j    = m_FuncJacobian(m_MatParam);
    if (j.rows() < j.cols())
    {
        return false;
    }
    KSMatrixSparsed y    = m_FuncResidual(m_MatParam);
    
    // IRLS用のweightを算出して乗算
    KSVectorXd v = y.col(0);
    auto w = v.cwiseAbs().cwiseMax(0.00001).cwiseInverse().asDiagonal();
    for (int i=0,n=j.cols(); i<n; ++i)
    {
        j.col(i) = w * j.col(i);
    }
    y = w * y;
    
    return m_pNESolver->Solve(m_MatParam, y, j);
}

// 残差平方和の取得
double KSSparseOptimizer::GetSquaredResidualsSum()
{
    KSMatrixSparsed m   = m_FuncResidual(m_MatParam);
    KSMatrixSparsed mt  = m.transpose();
    KSMatrixSparsed res = mt * m;
    return fabs(res.coeff(0, 0));
}

// 正規方程式ソルバの変更
void    KSSparseOptimizer::SwitchNormalEquationSolver(NESolverType type)
{
    m_pNESolver = m_NESolverFactory.Create(type);
}


