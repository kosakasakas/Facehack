//
//  ofTest.h
//
//  oFアプリでのテストクラス
//
//  Copyright (c) 2016年 Takahiro Kosaka. All rights reserved.
//  Created by Takahiro Kosaka on 2016/04/05.
//
//  This Source Code Form is subject to the terms of the Mozilla
//  Public License v. 2.0. If a copy of the MPL was not distributed
//  with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

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
        // ==================================
        // y=2x+1 を解として y=ax+b を最適化する
        // ==================================
        
        // データセットを登録。ただし、一つだけ5%の誤差を含んでいる
        KSMatrixXf  data(2, 5);
        data << 0.01, 0.04, 0.08, 0.12 * 1.005, 0.16,
                1.02, 1.08, 1.16, 1.24, 1.32;
        
        // オプティマイザの宣言
        KSDenseOptimizer  optimizer;
        
        // 残差関数
        KSFunction  residual    = [&optimizer](const KSVectorXf &x)->KSVectorXf
        {
            const KSMatrixXf& data = optimizer.GetDataMat();
            KSMatrixXf r(data.cols(), 1);
            
            for(int i=0, n=r.rows(); i<n; ++i)
            {
                // r = y-(ax+b)
                r(i)    = data(1,i) - (x(0) * data(0, i) + x(1));
            }
            
            return r;
        };
        
        // 残差のヤコビアン
        KSFunction jacobian     = [&optimizer](const KSVectorXf &x)->KSMatrixXf
        {
            const KSMatrixXf& data = optimizer.GetDataMat();
            KSMatrixXf d(data.cols(), x.rows());
            
            for(int i=0,n=d.rows(); i<n; ++i)
            {
                // d0 = -x
                // d1 = -1
                d(i, 0)         = -data(0,i);
                d(i, 1)         = -1;
            }
            
            return d;
        };
        
        // パラメータ行列の初期値を設定
        KSMatrixXf param0(2,1);
        param0 << 5.0, 5.0;
        
        // オプティマイザの初期化
        optimizer.Initialize(residual, jacobian, param0, data);
        
        // 計算ステップ5回
        int numStep = 5;
        
        // 計算開始(通常計算)
        TS_START("optimization exmple 1-1");
        for (int i = 0; i < numStep; ++i)
        {
            if (!optimizer.DoGaussNewtonStep())
            {
                ofLog(OF_LOG_ERROR, "ガウス-ニュートン計算ステップに失敗しました。");
                return false;
            }
        }
        TS_STOP("optimization exmple 1-1");
        
        // 解の確認
        ofLog(OF_LOG_NOTICE,
              "ex1-1: param0: %lf, param1: %lf",
              optimizer.GetParamMat()(0),
              optimizer.GetParamMat()(1));
        
        // パラメータ行列の初期値を再設定
        KSMatrixXf param1(2,1);
        param1 << 5.0, 5.0;
        optimizer.SetParamMat(param1);
        
        // 計算開始(IRLS計算)
        TS_START("optimization exmple 1-2");
        for (int i = 0; i < numStep; ++i)
        {
            if (!optimizer.DoGaussNewtonStepIRLS())
            {
                ofLog(OF_LOG_ERROR, "ガウス-ニュートン計算ステップに失敗しました。");
                return false;
            }
        }
        TS_STOP("optimization exmple 1-2");
        
        // 解の確認
        ofLog(OF_LOG_NOTICE,
              "ex1-2: param0: %lf, param1: %lf",
              optimizer.GetParamMat()(0),
              optimizer.GetParamMat()(1));
        
        // ================================
        // 結果:
        // [notice ] ex1-1: param0: 1.996821, param1: 1.000021
        // [notice ] ex1-2: param0: 2.000000, param1: 1.000000
        //
        // IRLSの方が誤差を含むデータに対して高精度な解が得られる。
        // ただし、0.1msほど計算が遅い。
        // ================================

    }
    
    // 例題No.2
    {
        // ==================================
        // wikiの例題を解く
        // https://en.wikipedia.org/wiki/Gauss%E2%80%93Newton_algorithm
        // ==================================
        
        // データセットを登録
        KSMatrixXf  data(2, 7);
        data <<  0.038, 0.194, 0.425, 0.626,  1.253,  2.500,  3.740,
        0.050, 0.127, 0.094, 0.2122, 0.2729, 0.2665, 0.3317;
        
        // オプティマイザの宣言
        KSDenseOptimizer  optimizer;
        
        // 残差関数
        KSFunction  residual    = [&optimizer](const KSVectorXf &x)->KSVectorXf
        {
            const KSMatrixXf& data = optimizer.GetDataMat();
            KSVectorXf y(data.cols());
            
            for(int i=0, n=y.rows(); i<n; ++i)
            {
                y(i)    = data(1,i) - (x(0) * data(0, i)) / (x(1) + data(0,i));
            }
            return y;
        };
        
        // 残差のヤコビアン
        KSFunction jacobian     = [&optimizer](const KSVectorXf &x)->KSMatrixXf
        {
            const KSMatrixXf& data = optimizer.GetDataMat();
            KSMatrixXf d(data.cols(), x.rows());
            
            for(int i=0,n=d.rows(); i<n; ++i)
            {
                double denom    = (x(1) + data(0,i)) * (x(1) + data(0,i));
                d(i, 0)         = -data(0,i) / (x(1) + data(0,i));
                d(i, 1)         = (x(1) * data(0, i)) / denom;
            }
            return d;
        };
        
        // 正解値マトリックスの初期値を設定
        KSMatrixXf param(2,1);
        param << 0.9, 0.2;
        
        // オプティマイザの初期化
        optimizer.Initialize(residual, jacobian, param, data);
        
        ofASSERT((optimizer.GetSquaredResidualsSum() - 1.445) < 0.01, "残差平方和の初期値が正解と異なります。");
        
        std::vector<double> srsLog;
        
        TS_START("optimization exmple 2");
        for (int i = 0; i < 5; ++i)
        {
            ofASSERT(optimizer.DoGaussNewtonStep(), "ガウス-ニュートン計算ステップに失敗しました。");
            srsLog.push_back(optimizer.GetSquaredResidualsSum());
        }
        TS_STOP("optimization exmple 2");
        
        //各ステップでの残差平方和
        ofLog(OF_LOG_NOTICE,
              "step0:%lf, step1:%lf, step2:%lf, step3:%lf, step4:%lf",
              srsLog[0], srsLog[1], srsLog[2], srsLog[3], srsLog[4]);
        
        ofASSERT((optimizer.GetSquaredResidualsSum() - 0.00784) < 0.01, "残差平方和の収束値が正解と異なります。");
        
        ofASSERT(fabs(optimizer.GetParamMat()(0) - 0.362) < 0.01, "パラメータ推定結果が異なります。");
        ofASSERT(fabs(optimizer.GetParamMat()(1) - 0.556) < 0.01, "パラメータ推定結果が異なります。");
        
        // ================================
        // 結果:
        // [notice ] step0:0.008561, step1:0.007904, step2:0.007855, step3:0.007846, step4:0.007844
        //
        // 残差平方和はステップごとに縮まっていて、
        // wikiの正解値である0.00784と同値(0.007844)が得られる。
        // ================================
    }
    
    // 例題No.3
    {
        // ==================================
        // 例題No.2と同じ問題をSparse行列を使い、
        // 前処理付き共役勾配法を使って解く
        // ==================================
    
        // データセットを登録
        KSMatrixSparsef  data(2, 7);
        data.insert(0, 0) = 0.038;
        data.insert(0, 1) = 0.194;
        data.insert(0, 2) = 0.425;
        data.insert(0, 3) = 0.626;
        data.insert(0, 4) = 1.253;
        data.insert(0, 5) = 2.500;
        data.insert(0, 6) = 3.740;
        data.insert(1, 0) = 0.050;
        data.insert(1, 1) = 0.127;
        data.insert(1, 2) = 0.094;
        data.insert(1, 3) = 0.2122;
        data.insert(1, 4) = 0.2729;
        data.insert(1, 5) = 0.2665;
        data.insert(1, 6) = 0.3317;
        
        // オプティマイザの宣言
        KSSparseOptimizer  optimizer;
        
        // ソルバを前処理付き共役勾配法(PGC)に変更
        optimizer.SwitchNormalEquationSolver(NESolverType::PCG);
        
        // PGCの試行回数のセット
        optimizer.SetMaxIterations(4);
        
        // 残差関数
        KSFunctionSparse  residual    = [&optimizer](const KSMatrixSparsef &x)->KSMatrixSparsef
        {
            const KSMatrixSparsef& data = optimizer.GetDataMat();
            KSMatrixSparsef y(data.cols(), 1);
            
            for(int i=0, n=y.rows(); i<n; ++i)
            {
                y.coeffRef(i, 0) = data.coeff(1, i) - (x.coeff(0, 0) * data.coeff(0, i)) / (x.coeff(1, 0) + data.coeff(0,i));
            }
            return y;
        };
        
        // 残差のヤコビアン
        KSFunctionSparse jacobian     = [&optimizer](const KSMatrixSparsef &x)->KSMatrixSparsef
        {
            const KSMatrixSparsef& data = optimizer.GetDataMat();
            KSMatrixSparsef d(data.cols(), x.rows());
            
            for(int i=0,n=d.rows(); i<n; ++i)
            {
                double denom    = (x.coeff(1, 0) + data.coeff(0,i)) * (x.coeff(1, 0) + data.coeff(0,i));
                d.coeffRef(i, 0)         = -data.coeff(0,i) / (x.coeff(1, 0) + data.coeff(0,i));
                d.coeffRef(i, 1)         = (x.coeff(1, 0) * data.coeff(0, i)) / denom;
            }
            return d;
        };
        
        // 正解値マトリックスの初期値を設定
        KSMatrixSparsef param(2,1);
        param.insert(0, 0) = 0.9;
        param.insert(1, 0) = 0.2;
        
        // オプティマイザの初期化
        optimizer.Initialize(residual, jacobian, param, data);
        
        ofASSERT((optimizer.GetSquaredResidualsSum() - 1.445) < 0.01, "残差平方和の初期値が正解と異なります。");
        
        std::vector<double> srsLog;
        
        TS_START("optimization exmple 3");
        for (int i = 0; i < 5; ++i)
        {
            ofASSERT(optimizer.DoGaussNewtonStep(), "ガウス-ニュートン計算ステップに失敗しました。");
            srsLog.push_back(optimizer.GetSquaredResidualsSum());
        }
        TS_STOP("optimization exmple 3");
        
        //各ステップでの残差平方和
        ofLog(OF_LOG_NOTICE,
              "step0:%lf, step1:%lf, step2:%lf, step3:%lf, step4:%lf",
              srsLog[0], srsLog[1], srsLog[2], srsLog[3], srsLog[4]);
        
        ofASSERT((optimizer.GetSquaredResidualsSum() - 0.00784) < 0.01, "残差平方和の収束値が正解と異なります。");
        
        ofASSERT(fabs(optimizer.GetParamMat().coeff(0, 0) - 0.362) < 0.01, "パラメータ推定結果が異なります。");
        ofASSERT(fabs(optimizer.GetParamMat().coeff(1, 0) - 0.556) < 0.01, "パラメータ推定結果が異なります。");
    }
    
    // 例題No.4
    {
        // 剛体変換のパラメータを解いてみる。
        //
        //                       | a, b, c |
        // Rt * v = | x, y, z| * | d, e, f | + |j, k, l|
        //                       | g, h, i |
        //
        // で表されるとすると、未知数は12個。 それ以上の残差があれば解けるはず
        
        const int paramNum = 6;
        float anser[paramNum];
        anser[0] = ofDegToRad(30.0f); // ロール（X軸回転）
        anser[1] = ofDegToRad(0.0f); // ピッチ（Y軸回転）
        anser[2] = ofDegToRad(0.0f); // ヨー（Z軸回転）
        anser[3] = 0.0f; // X軸平行移動
        anser[4] = 0.0f; // Y軸平行移動
        anser[5] = 0.0f;  // Z軸平行移動
        
        // 実験スペース
        {
            ofVec3f v = {1.0f, 1.0f, 0.0f};
            ofVec3f t = {0.0f, 0.0f, 0.0f};
            float a = ofDegToRad(30.0);
            float b = ofDegToRad(0.0);
            float c = ofDegToRad(0.0);
            ofMatrix4x4 m, mr, mp, my;
            m.makeRotationMatrix(ofRadToDeg(a), ofVec3f(1.0f, 0.0f, 0.0f),
                                 ofRadToDeg(b), ofVec3f(0.0f, 1.0f, 0.0f),
                                 ofRadToDeg(c), ofVec3f(0.0f, 0.0f, 1.0f));
            m.translate(t);
            ofVec3f ans = ofMatrix4x4::transform3x3(m, v);
            
            KSMatrixSparsef mt(4,4);
            mt.coeffRef(0, 0) = cosf(b) * cosf(c);
            mt.coeffRef(0, 1) = sinf(a) * sinf(b) * cosf(c) + cosf(a) * sinf(c);
            mt.coeffRef(0, 2) = -cosf(a) * sinf(b) * cosf(c) + sinf(a) * sinf(c);
            
            mt.coeffRef(1, 0) = -cosf(b) * sinf(c);
            mt.coeffRef(1, 1) = -sinf(a) * sinf(b) * sinf(c) + cosf(a) * cosf(c);
            mt.coeffRef(1, 2) = cosf(a) * sinf(b) * sinf(c) + sinf(a) * cosf(c);
            
            mt.coeffRef(2, 0) = sinf(b);
            mt.coeffRef(2, 1) = -sinf(a) * cosf(b);
            mt.coeffRef(2, 2) = cosf(a) * cosf(b);
            
            mt.coeffRef(3, 0) = t.x;
            mt.coeffRef(3, 1) = t.y;
            mt.coeffRef(3, 2) = t.z;

            KSVectorSparsef vt(4);
            vt.coeffRef(0) = v.x;
            vt.coeffRef(1) = v.y;
            vt.coeffRef(2) = v.z;
            vt.coeffRef(3) = 1;
            
            KSVectorSparsef at = vt.transpose() * mt;
            
            ofLog(OF_LOG_ERROR, "X: [my]%lf, [of]%lf", at.coeff(0), ans.x);
            ofLog(OF_LOG_ERROR, "Y: [my]%lf, [of]%lf", at.coeff(1), ans.y);
            ofLog(OF_LOG_ERROR, "Z: [my]%lf, [of]%lf", at.coeff(2), ans.z);
            
        }
        
        // ロール、ピッチ、ヨーの順で回転した後平行移動する行列を作成
        ofMatrix4x4 m, mr, mp, my;
        m.makeRotationMatrix(ofRadToDeg(anser[0]), ofVec3f(1.0f, 0.0f, 0.0f),
                             ofRadToDeg(anser[1]), ofVec3f(0.0f, 1.0f, 0.0f),
                             ofRadToDeg(anser[2]), ofVec3f(0.0f, 0.0f, 1.0f));
        m.translate(anser[3], anser[4], anser[5]);
        
        // 適当に入力を作る
        int sampleVecNum = 40;
        KSMatrixSparsef data(2, 3 * sampleVecNum);
        for (int i = 0; i < sampleVecNum; ++i)
        {
            ofVec4f v;
            v.x = ofRandom(-1.0f, 1.0f);
            v.y = ofRandom(-1.0f, 1.0f);
            v.z = ofRandom(-1.0f, 1.0f);
            v.w = 1.0f;
            
            ofVec4f a = m * v;
            
            data.insert(0, 3*i+0) = v.x;
            data.insert(0, 3*i+1) = v.y;
            data.insert(0, 3*i+2) = v.z;
            
            data.insert(1, 3*i+0) = a.x;
            data.insert(1, 3*i+1) = a.y;
            data.insert(1, 3*i+2) = a.z;
        }
        
        // オプティマイザの宣言
        KSSparseOptimizer  optimizer;
        
        // ソルバを前処理付き共役勾配法(PGC)に変更
        optimizer.SwitchNormalEquationSolver(NESolverType::PCG);
        
        // PGCの試行回数のセット
        optimizer.SetMaxIterations(8);
        
        // 残差関数
        KSFunctionSparse  residual    = [&optimizer](const KSMatrixSparsef &x)->KSMatrixSparsef
        {
            const KSMatrixSparsef& data = optimizer.GetDataMat();
            KSMatrixSparsef d(data.cols(), 1);
            
            float r = x.coeff(0, 0); // ロール
            float p = x.coeff(1, 0); // ピッチ
            float y = x.coeff(2, 0); // ヨー
            
            KSMatrixSparsef mt(4,4);
            mt.coeffRef(0, 0) = cosf(r) * cosf(p);
            mt.coeffRef(0, 1) = sinf(r) * cosf(p);
            mt.coeffRef(0, 2) = -sinf(p);
            mt.coeffRef(0, 3) = 0.0;
            mt.coeffRef(1, 0) = cosf(r) * sinf(p) * sinf(y) - sinf(r) * cosf(y);
            mt.coeffRef(1, 1) = sinf(r) * sinf(p) * sinf(y) + cosf(r) * cosf(y);
            mt.coeffRef(1, 2) = cosf(p) * sinf(y);
            mt.coeffRef(1, 3) = 0.0;
            mt.coeffRef(2, 0) = cosf(r) * sinf(p) * cosf(y) + sinf(r) * sinf(y);
            mt.coeffRef(2, 1) = sinf(r) * sinf(p) * cosf(y) - cosf(r) * sinf(y);
            mt.coeffRef(2, 2) = cosf(p) * cosf(y);
            mt.coeffRef(2, 3) = 0.0;
            mt.coeffRef(3, 0) = x.coeff(3, 0);
            mt.coeffRef(3, 1) = x.coeff(4, 0);
            mt.coeffRef(3, 2) = x.coeff(5, 0);
            mt.coeffRef(3, 3) = 1.0;
            
            // 実験
            ofMatrix4x4 mt2, mt2r, mt2p, mt2y;
            mt2.makeRotationMatrix(ofRadToDeg(r), ofVec3f(1.0f, 0.0f, 0.0f),
                                 ofRadToDeg(p), ofVec3f(0.0f, 1.0f, 0.0f),
                                 ofRadToDeg(y), ofVec3f(0.0f, 0.0f, 1.0f));
            mt2.translate(x.coeff(3, 0), x.coeff(4, 0), x.coeff(5, 0));
            
            KSVectorSparsef v(4);
            for(int i=0,n=d.rows()/3; i<n; ++i)
            {
                v.coeffRef(0) = data.coeff(0, 3*i);
                v.coeffRef(1) = data.coeff(0, 3*i+1);
                v.coeffRef(2) = data.coeff(0, 3*i+2);
                v.coeffRef(3) = 1.0f;
                
                // r = a - v * mt
                KSVectorSparsef vmt = v.transpose() * mt;
                d.coeffRef(3*i,0)    = data.coeff(1, 3*i) - vmt.coeff(0);
                d.coeffRef(3*i+1,0)  = data.coeff(1, 3*i+1) - vmt.coeff(1);
                d.coeffRef(3*i+2,0)  = data.coeff(1, 3*i+2) - vmt.coeff(2);
                
                ofVec4f v2;
                v2.x = data.coeff(0, 3*i);
                v2.y = data.coeff(0, 3*i+1);
                v2.z = data.coeff(0, 3*i+2);
                v2.w = 1.0f;
                ofVec4f vmt2 = mt2 * v2;
                
                ofLog(OF_LOG_ERROR, "r: %lf, p:%lf, y:%lf", r,p,y);
                ofLog(OF_LOG_ERROR, "X: [opt]%lf, [real]%lf", vmt.coeff(0), vmt2.x);
                ofLog(OF_LOG_ERROR, "Y: [opt]%lf, [real]%lf", vmt.coeff(1), vmt2.y);
                ofLog(OF_LOG_ERROR, "Z: [opt]%lf, [real]%lf", vmt.coeff(2), vmt2.z);
                ofLog(OF_LOG_ERROR, "W: [opt]%lf, [real]%lf", vmt.coeff(3), vmt2.w);
                
            }
            return d;
        };
        
        // 残差のヤコビアン
        KSFunctionSparse jacobian     = [&optimizer](const KSMatrixSparsef &x)->KSMatrixSparsef
        {
            const KSMatrixSparsef& data = optimizer.GetDataMat();
            KSMatrixSparsef d(data.cols(), x.rows());
            
            float r = x.coeff(0, 0);
            float p = x.coeff(1, 0);
            float y = x.coeff(2, 0);
            
            for(int i=0,n=d.rows()/3; i<n; ++i)
            {
                // 極めてΘが小さい場合は
                // cosΘ = 1, sinΘ = Θ
                // さらに、sin * sin = 0と近似できる。
                // これを踏まえて行列を変換すると、以下のようになる
                // (ロール角がr, ピッチ角がp, ヨー角がy, 平行移動が(tx,ty,tz))
                //      | 1   r   -p  0 |
                // RT = | -r  1   y   0 |
                //      | p   -y  1   0 |
                //      | tx  ty  tz  1 |
                // つまり、v(vx, vy, vz, 1)を変換すると、
                //          | vx    - r*vy + p*vz + tx |t
                // v * RT = | r*vx  + vy   - y*vz + ty |
                //          | -p*vx + y*vy + vz   + tz |
                //          | 1                        |
                // になる。
                // ということで、ヤコビアンは、、、
                
                float vx = data.coeff(0, 3*i+0);
                float vy = data.coeff(0, 3*i+1);
                float vz = data.coeff(0, 3*i+2);
                
                //6個入れる
                d.coeffRef(3*i, 0) = -(-vy);
                d.coeffRef(3*i, 1) = -(vz);
                d.coeffRef(3*i, 2) = 0.0;
                d.coeffRef(3*i, 3) = -1.0;
                d.coeffRef(3*i, 4) = 0.0;
                d.coeffRef(3*i, 5) = 0.0;
                
                //6個入れる
                d.coeffRef(3*i+1, 0) = -(vx);
                d.coeffRef(3*i+1, 1) = 0.0;
                d.coeffRef(3*i+1, 2) = -(-vz);
                d.coeffRef(3*i+1, 3) = 0.0;
                d.coeffRef(3*i+1, 4) = -1.0;
                d.coeffRef(3*i+1, 5) = 0.0;
                
                //6個入れる
                d.coeffRef(3*i+2, 0) = 0.0;
                d.coeffRef(3*i+2, 1) = -(-vx);
                d.coeffRef(3*i+2, 2) = -(vy);
                d.coeffRef(3*i+2, 3) = 0.0;
                d.coeffRef(3*i+2, 4) = 0.0;
                d.coeffRef(3*i+2, 5) = -1.0;
            }
            
            return d;
        };
        
        // 正解値マトリックスの初期値を設定
        KSMatrixSparsef param(paramNum,1);
        for (int i = 0; i < paramNum; ++i)
        {
            param.coeffRef(i, 0) = 0.0;
        }
        
        // オプティマイザの初期化
        optimizer.Initialize(residual, jacobian, param, data);
        
        std::vector<double> srsLog;
        
        const int gaussStepNum = 7;
        TS_START("optimization exmple 4");
        for (int i = 0; i < gaussStepNum; ++i)
        {
            ofASSERT(optimizer.DoGaussNewtonStepIRLS(), "ガウス-ニュートン計算ステップに失敗しました。");
            srsLog.push_back(optimizer.GetSquaredResidualsSum());
        }
        TS_STOP("optimization exmple 4");
        
        //各ステップでの残差平方和
        for(int i = 0; i < gaussStepNum; ++i)
        {
            ofLog(OF_LOG_NOTICE, "SRS: (%dth step)%lf", i, srsLog[i]);
        }
        
        for (int i =0; i < paramNum; ++i)
        {
            ofLog(OF_LOG_ERROR, "%dth param: [opt]%lf, [ans]%lf", i, optimizer.GetParamMat().coeff(i, 0), anser[i]);
            ofASSERT(fabs(optimizer.GetParamMat().coeff(i, 0) - anser[i]) < 0.01, "パラメータ推定結果が異なります。");
        }

    }
    return true;
}