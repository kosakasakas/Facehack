//
//  KSCholeskyDecomposition.h
//
//  正規方程式のコレスキー分解によるソルバ
//
//  Copyright (c) 2016年 Takahiro Kosaka. All rights reserved.
//  Created by Takahiro Kosaka on 2016/04/14.
//
//  This Source Code Form is subject to the terms of the Mozilla
//  Public License v. 2.0. If a copy of the MPL was not distributed
//  with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef KSCholeskyDecomposition_h
#define KSCholeskyDecomposition_h

#include "KSNormalEquationSolver.h"
#include "../../../extAddons/Eigen/SparseCholesky"

namespace Kosakasakas {
    
    /**
     @brief 正規方程式のコレスキー分解によるソルバ
     正規方程式をコレスキー分解により解くクラスです.
     */
    class KSCholeskyDecomposition : public KSNormalEquationSolver
    {
    public:
        //! コンストラクタ
        KSCholeskyDecomposition()
        {};
        
        //! デストラクタ
        virtual ~KSCholeskyDecomposition()
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
        inline bool Solve(KSMatrixXd& dst, KSMatrixXd& y, KSMatrixXd& j)
        {
            KSMatrixXd jt   = j.transpose();
            auto llt        = (jt * j).ldlt();
            if (llt.info()  != Eigen::Success)
            {
                return false;
            }
            
            KSMatrixXd s    = llt.solve(jt * y * -1.0);
            dst             = dst + s;
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
        inline bool Solve(KSMatrixSparsed& dst, KSMatrixSparsed& y, KSMatrixSparsed& j)
        {
            KSMatrixSparsed jt  = j.transpose();
            KSMatrixSparsed A   = jt * j;
            KSMatrixSparsed b   = jt * y * -1.0;
            Eigen::SimplicialLLT<KSMatrixSparsed> solver;
            solver.compute(A);
            if(solver.info()!=Eigen::Success)
            {
                return false;
            }
            
            KSMatrixSparsed s   = solver.solve(b);
            dst                 = dst + s;
            return true;
        };

    };
    
} //namespace Kosakasakas {


#endif /* KSCholeskyDecomposition_h */
