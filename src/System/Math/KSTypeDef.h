//
//  KSTypeDef.h
//
//  KSMathライブラリの型を定義するヘッダファイル
//
//  Copyright (c) 2016年 Takahiro Kosaka. All rights reserved.
//  Created by Takahiro Kosaka on 2016/04/05.
//
//

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
    
    
} //namespace Kosakasakas {

#endif /* KSTypeDef_h */
