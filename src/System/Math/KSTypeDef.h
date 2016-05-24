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

#include "../../../extAddons/Eigen/Dense"

namespace Kosakasakas {

    /**
     @brief double精度の任意要素マトリックス
            現状最適化処理専用で、Eigenラップしただけです。
     */
    typedef Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>   KSMatrixXd;
    
    /**
     @brief double精度の任意要素ベクトル
     現状最適化処理専用で、Eigenラップしただけです。
     */
    typedef Eigen::Matrix<double, Eigen::Dynamic, 1>                KSVectorXd;
    
    /**
     @brief マトリックス引数の任意ファンクタ
     現状最適化処理専用です。
     */
    typedef std::function<KSMatrixXd(const KSMatrixXd &x)>          KSFunction;
    
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
