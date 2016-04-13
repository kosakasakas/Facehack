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
    m_LastResidualMat   = KSMatrixXd::Ones(m_MatData.cols(), 1);
    m_IRISWeight        = KSMatrixXd::Ones(m_MatData.cols(), 1).asDiagonal();
    m_WeightResidual = KSMatrixXd::Ones(m_MatData.cols(), 1);
    m_WeightJacobian = KSMatrixXd::Ones(m_MatData.cols(), m_MatData.rows());

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

bool    KSOptimizerForNLLS::DoGaussNewtonStepIRIS()
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
    
    // IRIS用のweightを算出して乗算
    KSMatrixXd w    = y.cwiseAbs().cwiseMax(0.00001).cwiseInverse().asDiagonal();
    for (int i=0,n=j.cols(); i<n; ++i)
    {
        j.col(i) = w * j.col(i);
    }
    y = w * y;
    
    KSMatrixXd jt   = j.transpose();
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

void    KSOptimizerForNLLS::UpdateIRISWeightMat()
{
    m_WeightJacobian = m_FuncJacobian(m_MatParam);
    m_WeightResidual = m_FuncResidual(m_MatParam);

    // IRIS weight for jacobian
    for (int i=0, n=m_WeightJacobian.cols(); i<n; ++i)
    {
        m_WeightJacobian.col(i) = m_WeightJacobian.col(i).cwiseAbs().cwiseMax(0.00001).cwiseInverse();
    }
    m_WeightResidual = m_WeightResidual.cwiseAbs().cwiseMax(0.00001).cwiseInverse();
    
    std::string strP = SerializeMat((const Eigen::Matrix<double, 2, 1>)m_MatParam);
    std::string strJ = SerializeMat((const Eigen::Matrix<double, 5, 2>)m_WeightJacobian);
    std::string strR = SerializeMat((const Eigen::Matrix<double, 5, 1>)m_WeightResidual);
    int a = 0;
}


