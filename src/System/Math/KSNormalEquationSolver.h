//
//  KSNormalEquationSolver.h
//
//  正規方程式のソルバクラスインターフェース
//
//  Copyright (c) 2016年 Takahiro Kosaka. All rights reserved.
//  Created by Takahiro Kosaka on 2016/04/14.
//
//

#ifndef KSNormalEquationSolver_h
#define KSNormalEquationSolver_h

#include "KSTypeDef.h"

namespace Kosakasakas {
    
    /**
     @brief 正規方程式のソルバクラスインターフェース
     正規方程式のソルバを定義するためのインターフェースです.
     */
    class KSNormalEquationSolver
    {
    public:
        //! コンストラクタ
        KSNormalEquationSolver()
        {};
        
        //! デストラクタ
        virtual ~KSNormalEquationSolver()
        {};
        
        /**
         @brief 計算実行
         
         実際に計算を行う関数です.
         実装は継承先で定義して下さい.
         @param dst     出力パラメータ行列
         @param y       残差関数
         @param j       残差関数のヤコビアン
         @return 計算の成否
         */
        virtual bool    Solve(KSMatrixXd& dst, KSMatrixXd& y, KSMatrixXd& j) = 0;
        
        /**
         @brief 計算実行
         
         実際に計算を行う関数です.
         実装は継承先で定義して下さい.
         @param dst     出力パラメータ行列
         @param y       残差関数
         @param j       残差関数のヤコビアン
         @return 計算の成否
         */
        virtual bool    Solve(KSMatrixSparsed& dst, KSMatrixSparsed& y, KSMatrixSparsed& j) = 0;
    };
    
} //namespace Kosakasakas {

#endif /* KSNormalEquationSolver_h */
