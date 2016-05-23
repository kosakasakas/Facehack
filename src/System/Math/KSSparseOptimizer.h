//
//  KSSparseOptimizer.h
//  Face2Face
//
//  Created by 小坂 昂大 on 2016/05/23.
//
//

#ifndef __Face2Face__KSSparseOptimizer__
#define __Face2Face__KSSparseOptimizer__

#include "KSTypeDef.h"
#include "memory.h"
#include <functional>
#include "KSNormalEquationSolver.h"
#include "KSNESolverFactory.h"

namespace Kosakasakas {
    
    /**
     @brief 非線形最小二乗問題を扱うための最適化計算クラスです。
     線形問題も扱えます。
     ソルバにはガウス-ニュートン法を用います。
     */
    class KSSparseOptimizer
    {
    public:
        //! コンストラクタ
        KSSparseOptimizer();
        //! デストラクタ
        ~KSSparseOptimizer();
        
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
        bool    Initialize(KSFunctionSparse& residual,
                           KSFunctionSparse& jaconian,
                           KSMatrixSparsed& initParam,
                           KSMatrixSparsed& data);
        
        /**
         @brief 最適化ステップの実行（ガウス-ニュートン法）
         
         ガウス-ニュートン法による最適化計算を行います。
         内部では1回しか最適化計算を行わないため、アプリケーション側で複数回計算ステップを実行してください。
         実行前に必ずInitializeを呼んでください。
         @return 初期化の成否
         */
        bool    DoGaussNewtonStep();
        
        /**
         @brief IRLS最適化ステップの実行（ガウス-ニュートン法）
         
         Iteratively reweighted least squaresをガウス-ニュートン法により最適化計算します。
         重みの計算は、前ステップの解による残差の逆数を絶対値で使っています。
         内部では1回しか最適化計算を行わないため、アプリケーション側で複数回計算ステップを実行してください。
         実行前に必ずInitializeを呼んでください。
         @return 初期化の成否
         */
        bool    DoGaussNewtonStepIRLS();
        
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
        inline const KSMatrixSparsed&    GetParamMat() const
        {
            return m_MatParam;
        }
        
        /**
         @brief サンプルデータマトリックスの取得
         
         サンプルデータマトリックスを取得します。
         @return パラメータマトリックス
         */
        inline const KSMatrixSparsed&    GetDataMat() const
        {
            return m_MatData;
        }
        
        /**
         @brief 正規方程式ソルバの変更
         
         内部で使う正規方程式のソルバを変更します.
         デフォルトではコレスキー分解が指定されています.
         @param type        ソルバー列挙型
         */
        void    SwitchNormalEquationSolver(NESolverType type);
        
        /**
         @brief パラメータ行列のセット
         
         最適化するパラメータ行列の初期値をセットします.
         各パラメータは内部でstd::moveされ、所有権がこのクラスに渡ってしまう点に注意して下さい.
         @param paramMat    パラメータ行列
         */
        inline void SetParamMat(KSMatrixSparsed& paramMat)
        {
            m_MatParam  = std::move(paramMat);
        }
        
    private:
        //! 正規方程式ソルバへのシェアードポインタ
        typedef std::shared_ptr<KSNormalEquationSolver> NESolverPtr;
        
        //! 内部初期化されているかどうか
        bool        m_IsInitialized;
        //! 残差の関数を保持するオブジェクト
        KSFunctionSparse  m_FuncResidual;
        //! 残差のヤコビアンを保持するオブジェクト
        KSFunctionSparse  m_FuncJacobian;
        //! パラメータマトリックスを保持するオブジェクト
        KSMatrixSparsed  m_MatParam;
        //! サンプルデータマトリックスを保持するオブジェクト
        KSMatrixSparsed  m_MatData;
        
        //! 正規方程式ソルバのファクトリオブジェクト
        KSNESolverFactory   m_NESolverFactory;
        //! 正規方程式ソルバのポインタ
        NESolverPtr         m_pNESolver;
    };
    
} //namespace Kosakasakas {


#endif /* defined(__Face2Face__KSSparseOptimizer__) */
