//
//  KSTypeDef.h
//
//  KSMathライブラリの型を定義するヘッダファイル
//
//  Copyright (c) 2016年 Takahiro Kosaka. All rights reserved.
//  Created by Takahiro Kosaka on 2016/04/05.
//
//  This Source Code Form is subject to the terms of the Mozilla
//  Public License v. 2.0. If a copy of the MPL was not distributed
//  with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef KSTypeDef_h
#define KSTypeDef_h

#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/SparseCore>

namespace Kosakasakas {

    /**
     @brief double精度の任意要素マトリックス
            現状最適化処理専用で、Eigenラップしただけです。
     */
    typedef Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>   KSMatrixXd;
    
    /**
     @brief double精度の任意要素マトリックス
     現状最適化処理専用で、Eigenラップしただけです。
     */
    typedef Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic>   KSMatrixXf;
    
    /**
     @brief double精度の任意要素スパースマトリックス
     現状最適化処理専用で、Eigenラップしただけです。
     */
    typedef Eigen::SparseMatrix<double>                             KSMatrixSparsed;
    /**
     @brief float精度の任意要素スパースマトリックス
     現状最適化処理専用で、Eigenラップしただけです。
     */
    typedef Eigen::SparseMatrix<float>                             KSMatrixSparsef;
    
    /**
     @brief float精度の任意要素ベクトル
     Eigenラップしただけです。
     */
    typedef Eigen::Matrix<float, Eigen::Dynamic, 1>                 KSVectorXf;
    
    /**
     @brief double精度の任意要素ベクトル
     現状最適化処理専用で、Eigenラップしただけです。
     */
    typedef Eigen::Matrix<double, Eigen::Dynamic, 1>                KSVectorXd;
    
    /**
     @brief double精度の任意要素スパースベクトル
     現状最適化処理専用で、Eigenラップしただけです。
     */
    typedef Eigen::SparseVector<double>                             KSVectorSparsed;
    
    /**
     @brief float精度の任意要素スパースベクトル
     現状最適化処理専用で、Eigenラップしただけです。
     */
    typedef Eigen::SparseVector<float>                             KSVectorSparsef;
    
    /**
     @brief マトリックス引数の任意ファンクタ
     現状最適化処理専用です。
     */
    typedef std::function<KSMatrixXf(const KSMatrixXf &x)>              KSFunction;
    
    /**
     @brief スパースマトリックス引数の任意ファンクタ
     現状最適化処理専用です。
     */
    typedef std::function<KSMatrixSparsef(const KSMatrixSparsef &x)>    KSFunctionSparse;
    
    /**
     @brief 正規方程式のソルバータイプ
     正規方程式の直接解を解くソルバーで、このライブラリで指定できるソルバーです。
     */
    enum NESolverType
    {
        //! コレスキー分解
        CHOLESKY,
        //! 前処理付き共役勾配法
        PCG
    };
    
    
} //namespace Kosakasakas {

#endif /* KSTypeDef_h */
