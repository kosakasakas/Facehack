//
//  KSCholeskyDecomposition.h
//
//  正規方程式のコレスキー分解によるソルバ
//
//  Copyright (c) 2016年 Takahiro Kosaka. All rights reserved.
//  Created by Takahiro Kosaka on 2016/04/14.
//
//

#ifndef KSCholeskyDecomposition_h
#define KSCholeskyDecomposition_h

#include "KSNormalEquationSolver.h"

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
    };
    
} //namespace Kosakasakas {


#endif /* KSCholeskyDecomposition_h */
