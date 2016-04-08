//
//  KSOptimizerForNLLS.h
//
//  非線形最小二乗問題のための最適化計算クラス
//
//  Copyright (c) 2016年 Takahiro Kosaka. All rights reserved.
//  Created by Takahiro Kosaka on 2016/04/05.
//
//

#ifndef KSOptimizerForNLLS_h
#define KSOptimizerForNLLS_h

#include "KSTypeDef.h"
#include "memory.h"
#include <functional>

namespace Kosakasakas {
    
    /**
     @brief 非線形最小二乗問題を扱うための最適化計算クラスです。
            線形問題も扱えます。
            ソルバにはガウス-ニュートン法を用います。
    */
    class KSOptimizerForNLLS
    {
    public:
        //! コンストラクタ
        KSOptimizerForNLLS();
        //! デストラクタ
        ~KSOptimizerForNLLS();
        
        /**
         @brief 初期化
         
         最適化計算クラスを初期化します.
         各パラメータは内部でstd::moveされ、所有権がこのクラスに渡ってしまう点に注意して下さい。
         @param residual    残差関数
         @param jacobian    残差関数のヤコビアン（一次微分）
         @param param       パラメータの初期値マトリックス
         @param data        サンプルデータマトリック
         @return 初期化の成否
         */
        bool    Initialize(KSFunction& residual,
                           KSFunction& jaconian,
                           KSMatrixXd& initParam,
                           KSMatrixXd& data);
        
        /**
         @brief 最適化ステップの実行（ガウス-ニュートン法）
         
         ガウス-ニュートン法による最適化計算を行います。
         内部では1回しか最適化計算を行わないため、アプリケーション側で複数回計算ステップを実行すると
         精度が上がって行きます。
         実行前に必ずInitializeを呼んでください。
         @return 初期化の成否
         */
        bool    DoGaussNewtonStep();
        
        /**
         @brief 残差平方和の取得
         
         現在のパラメータでのサンプルデータとの残差平方和を取得します。
         @return 残差平方和
         */
        double  GetSquaredResidualsSum();
        
        /**
         @brief パラメータマトリックスの取得
         
         現在のパラメータマトリックスを取得します。
         @return パラメータマトリックス
         */
        inline const KSMatrixXd&    GetParamMat() const
        {
            return m_MatParam;
        }
        
        /**
         @brief サンプルデータマトリックスの取得
         
         サンプルデータマトリックスを取得します。
         @return パラメータマトリックス
         */
        inline const KSMatrixXd&    GetDataMat() const
        {
            return m_MatData;
        }
        
        const KSMatrixXd&   GetLastResidualMat() const
        {
            return m_LastResidualMat;
        }
        
        void GetIRISWieghtMat(KSMatrixXd& dst);
        
        void    SetLastResidualMat(KSMatrixXd& mat)
        {
            m_LastResidualMat   = mat;
        }
        
        template <typename T, int R, int L> std::string SerializeMat(const Eigen::Matrix<T, R, L>& M){
            std::stringstream strm;
            
            // put the size of the serialized matrix
            //strm << R << "," << L << ",";
            
            // put the elements of the serialized matrix
            for(int i=0; i<R; i++){
                for(int j=0; j<L; j++){
                    strm << M(i,j);
                    
                    // separator
                    if(!(i == R-1 && j == L-1))
                        strm << ",";
                }
            }
            
            return strm.str();
        }
        
        template <typename T> Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> DeserializeMat(std::string str){
            Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> M;
            int R, L;
            
            // sed "s/,/ /g" str
            replace(str.begin(), str.end(), ',', ' ');
            
            std::stringstream strm(str);
            // get the size of the output matrix
            strm >> R >> L;
            
            // get the elements of the output matrix
            M = decltype(M)(R, L);
            for(int i=0; i<R; i++){
                for(int j=0; j<L; j++){
                    strm >> M(i, j);
                }
            }
            
            return M;
        }
        
    private:
        //! 内部初期化されているかどうか
        bool        m_IsInitialized;
        //! 残差の関数を保持するオブジェクト
        KSFunction  m_FuncResidual;
        //! 残差のヤコビアンを保持するオブジェクト
        KSFunction  m_FuncJacobian;
        //! パラメータマトリックスを保持するオブジェクト
        KSMatrixXd  m_MatParam;
        //! サンプルデータマトリックスを保持するオブジェクト
        KSMatrixXd  m_MatData;
        //! 前ステップでの残差行列
        KSMatrixXd  m_LastResidualMat;
    };
    
} //namespace Kosakasakas {

#endif /* KSOptimizerForNLLS_h */
