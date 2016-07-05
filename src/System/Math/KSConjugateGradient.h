//
//  KSConjugateGradient.h
//
//  正規方程式の共役勾配法によるソルバ
//
//  Copyright (c) 2016年 Takahiro Kosaka. All rights reserved.
//  Created by Takahiro Kosaka on 2016/05/24.
//
//  This Source Code Form is subject to the terms of the Mozilla
//  Public License v. 2.0. If a copy of the MPL was not distributed
//  with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef KSConjugateGradient_h
#define KSConjugateGradient_h

#include "KSNormalEquationSolver.h"
#include <eigen3/Eigen/SparseCholesky>
#include <eigen3/Eigen/IterativeLinearSolvers>


namespace Kosakasakas {
    
    /**
     @brief 正規方程式のコレスキー分解によるソルバ
     正規方程式をコレスキー分解により解くクラスです.
     */
    class KSConjugateGradient : public KSNormalEquationSolver
    {
    public:
        //! コンストラクタ
        KSConjugateGradient()
        {};
        
        //! デストラクタ
        virtual ~KSConjugateGradient()
        {};
        
        //! 初期化
        inline bool Initialize()
        {
            return true;
        };
        
        //! 終了処理
        inline void Finalize()
        {};
        
        /**
         @brief 計算実行
         
         実際に計算を行う関数です.
         コレスキー分解により正規方程式を解きます.
         @param dst     出力パラメータ行列
         @param y       残差関数
         @param j       残差関数のヤコビアン
         @return 計算の成否
         */
        inline bool Solve(KSMatrixXf& dst, KSMatrixXf& y, KSMatrixXf& j, int maxIterations)
        {
            KSMatrixXf jt  = j.transpose();
            KSMatrixXf A   = jt * j;
            KSMatrixXf b   = jt * y * -1.0;
            Eigen::ConjugateGradient<KSMatrixXf> solver;
            solver.compute(A);
            if(solver.info()!=Eigen::Success)
            {
                return false;
            }
            solver.setMaxIterations(maxIterations);
            KSMatrixXf s   = solver.solve(b);
            dst                 = dst + s;
            return true;
        };
        
        /**
         @brief 計算実行
         
         実際に計算を行う関数です.
         コレスキー分解により正規方程式を解きます.
         @param dst     出力パラメータ行列
         @param y       残差関数
         @param j       残差関数のヤコビアン
         @return 計算の成否
         */
        inline bool Solve(KSMatrixSparsef& dst, KSMatrixSparsef& y, KSMatrixSparsef& j, int maxIterations)
        {
            KSMatrixSparsef jt  = j.transpose();
            KSMatrixSparsef A   = jt * j;
            KSMatrixSparsef b   = jt * y * -1.0;
            Eigen::ConjugateGradient<KSMatrixSparsef> solver;
            solver.compute(A);
            if(solver.info()!=Eigen::Success)
            {
                return false;
            }
            solver.setMaxIterations(maxIterations);
            KSMatrixSparsef s   = solver.solve(b);
            dst                 = dst + s;
            return true;
        };
        
    };
    
} //namespace Kosakasakas {



#endif /* KSConjugateGradient_h */
