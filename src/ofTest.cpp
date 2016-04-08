//
//  ofTest.h
//
//  oFアプリでのテストクラス
//
//  Copyright (c) 2016年 Takahiro Kosaka. All rights reserved.
//  Created by Takahiro Kosaka on 2016/04/05.
//
//

#include "ofTest.h"
#include "System/Math/KSMath.h"
#include "System/Util/KSUtil.h"
#include "ofxTimeMeasurements.h"

using namespace std;
using namespace Kosakasakas;

ofTest::ofTest()
{}

ofTest::~ofTest()
{}

bool    ofTest::Initialize()
{
    return true;
}

bool    ofTest::DoTest()
{
    // 最適化テスト
    
    // 例題No.1
    {
        // タイマー開始
        TS_START("optimization exmple 1");
        
        // ==================================
        // y=2x+1 を解として y=ax+b を最適化する
        // ==================================
        
        // データセットを登録
        KSMatrixXd  data(2, 5);
        data << 0.01, 0.04, 0.08, 0.12, 0.16,
                1.02, 1.08, 1.16, 1.24, 1.32;
        
        // オプティマイザの宣言
        KSOptimizerForNLLS  optimizer;
        
        // 残差関数
        KSFunction  residual    = [&optimizer](const KSMatrixXd &x)->KSMatrixXd
        {
            const KSMatrixXd& data = optimizer.GetDataMat();
            KSMatrixXd r(data.cols(), 1);
            
            for(int i=0, n=r.rows(); i<n; ++i)
            {
                // r = y-(ax+b)
                r(i)    = data(1,i) - (x(0) * data(0, i) + x(1));
            }
            
            optimizer.SetLastResidualMat(r);
            
            KSMatrixXd w;
            optimizer.GetIRISWieghtMat(w);
            
            r = w * r;
            
            
            return r;
        };
        
        // 残差のヤコビアン
        KSFunction jacobian     = [&optimizer](const KSMatrixXd &x)->KSMatrixXd
        {
            const KSMatrixXd& data = optimizer.GetDataMat();
            KSMatrixXd d(data.cols(), x.rows());
            
            for(int i=0,n=d.rows(); i<n; ++i)
            {
                // d0 = -x
                // d1 = 1
                d(i, 0)         = -data(0,i);
                d(i, 1)         = -1;
            }
            return d;
        };
        
        // 正解値マトリックスの初期値を設定
        KSMatrixXd param(2,1);
        param << 5.0, 5.0;
        
        // オプティマイザの初期化
        optimizer.Initialize(residual, jacobian, param, data);
        
        // 計算ステップ5回
        std::vector<double> srsLog;
        int numStep = 5;
        for (int i = 0; i < numStep; ++i)
        {
            KSUtil::ofASSERT(optimizer.DoGaussNewtonStep(), "ガウス-ニュートン計算ステップに失敗しました。");
            srsLog.push_back(optimizer.GetSquaredResidualsSum());
        }
        
        //各ステップでの残差平方和
        ofLog(OF_LOG_NOTICE,
              "step0:%lf, step1:%lf, step2:%lf, step3:%lf, step4:%lf",
              srsLog[0], srsLog[1], srsLog[2], srsLog[3], srsLog[4]);
        
        // 残差平方和の確認。データ的には0になるはず。
        KSUtil::ofASSERT(optimizer.GetSquaredResidualsSum() < 0.004, "残差平方和の収束精度が低いです。");
        
        // 解の確認
        KSUtil::ofASSERT(fabs(optimizer.GetParamMat()(0) - 2.0) < 0.004, "パラメータ推定結果が異なります。");
        KSUtil::ofASSERT(fabs(optimizer.GetParamMat()(1) - 1.0) < 0.004, "パラメータ推定結果が異なります。");
        
        // タイマー終了
        TS_STOP("optimization exmple 1");
    }
    
    // 例題No.2
    {
        TS_START("optimization exmple 2");
        
        // ==================================
        // 以下の例題を解く
        // https://en.wikipedia.org/wiki/Gauss%E2%80%93Newton_algorithm
        // ==================================
    
        // データセットを登録
        KSMatrixXd  data(2, 7);
        data <<  0.038, 0.194, 0.425, 0.626,  1.253,  2.500,  3.740,
                    0.050, 0.127, 0.094, 0.2122, 0.2729, 0.2665, 0.3317;
        
        // オプティマイザの宣言
        KSOptimizerForNLLS  optimizer;
        
        // 残差関数
        KSFunction  residual    = [&optimizer](const KSMatrixXd &x)->KSMatrixXd
        {
            const KSMatrixXd& data = optimizer.GetDataMat();
            KSMatrixXd y(data.cols(), 1);
            
            for(int i=0, n=y.rows(); i<n; ++i)
            {
                y(i)    = data(1,i) - (x(0) * data(0, i)) / (x(1) + data(0,i));
            }
            return y;
        };
        
        // 残差のヤコビアン
        KSFunction jacobian     = [&optimizer](const KSMatrixXd &x)->KSMatrixXd
        {
            const KSMatrixXd& data = optimizer.GetDataMat();
            KSMatrixXd d(data.cols(), x.rows());
            
            for(int i=0,n=d.rows(); i<n; ++i)
            {
                double denom    = (x(1) + data(0,i)) * (x(1) + data(0,i));
                d(i, 0)         = -data(0,i) / (x(1) + data(0,i));
                d(i, 1)         = (x(1) * data(0, i)) / denom;
            }
            return d;
        };
        
        // 正解値マトリックスの初期値を設定
        KSMatrixXd param(2,1);
        param << 0.9, 0.2;
        
        // オプティマイザの初期化
        optimizer.Initialize(residual, jacobian, param, data);
        
        KSUtil::ofASSERT((optimizer.GetSquaredResidualsSum() - 1.445) < 0.01, "残差平方和の初期値が正解と異なります。");
        
        std::vector<double> srsLog;
        for (int i = 0; i < 5; ++i)
        {
            KSUtil::ofASSERT(optimizer.DoGaussNewtonStep(), "ガウス-ニュートン計算ステップに失敗しました。");
            srsLog.push_back(optimizer.GetSquaredResidualsSum());
        }
        
        //各ステップでの残差平方和
        ofLog(OF_LOG_NOTICE,
              "step0:%lf, step1:%lf, step2:%lf, step3:%lf, step4:%lf",
              srsLog[0], srsLog[1], srsLog[2], srsLog[3], srsLog[4]);
        
        KSUtil::ofASSERT((optimizer.GetSquaredResidualsSum() - 0.00784) < 0.01, "残差平方和の収束値が正解と異なります。");
        
        KSUtil::ofASSERT(fabs(optimizer.GetParamMat()(0) - 0.362) < 0.01, "パラメータ推定結果が異なります。");
        KSUtil::ofASSERT(fabs(optimizer.GetParamMat()(1) - 0.556) < 0.01, "パラメータ推定結果が異なります。");
        
        TS_STOP("optimization exmple 2");
    }
    
    return true;
}