//
//  ofKsBaselFaceModel.hpp
//  Facehack
//
//  Created by Takahiro Kosaka on 2016/07/01.
//  Copyright (c) 2016年 Takahiro Kosaka. All rights reserved.
//
//  This Source Code Form is subject to the terms of the Mozilla
//  Public License v. 2.0. If a copy of the MPL was not distributed
//  with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef ofKsBaselFaceModel_hpp
#define ofKsBaselFaceModel_hpp

#include "ofKsModel.hpp"
#include "vtkStandardMeshRepresenter.h"
#include "StatisticalModel.h"
#include "System/Math/KSMath.h"

#include <vector>
#include <vtkPolyData.h>
#include <vector>

namespace Kosakasakas
{
    /**
     @brief BaselFaceModelクラス
     
     以下のHDF5モデルを読み込み、PCAのサンプリングを行ったモデルを管理するクラス
     http://faces.cs.unibas.ch/bfm/main.php?nav=1-1-0&id=details
     このモデル以外の読み込みは保証しません.
     */
    class ofKsBaselFaceModel : public ofKsModel
    {
    public:
        //! コンストラクタ
        ofKsBaselFaceModel();
        //! デストラクタ
        virtual ~ofKsBaselFaceModel();
        
        //! 初期化
        bool    Initialize(const char* dirPath, const char* fileName);
        //! 終了
        void    Finalize();
        
        //! MeanShapeをメッシュに書き込む
        bool    DrawMean(bool cacheNormal = false);
        //! ランダムサンプリングした結果をメッシュに書き込む
        bool    DrawRandomSample(bool useCachedNormal = false);
        //! 指定のPCAの主成分値でサンプリングした結果をメッシュに書き込む
        bool    DrawSample(KSVectorXf& shapeCoeff, KSVectorXf& albedoCoeff, bool useCachedNormal = false);
        //! ミーンシェイプの法線をキャッシュしておく
        bool    CacheMeanShapeNormal();
        
    protected:
        //! モデルの読み込み
        bool    LoadMesh();
        //! データをメッシュに読み込む
        bool    SetupMesh(ofMesh& dstMesh,
                          vtkPolyData* pSrcVertices,
                          vtkPolyData* pSrcColors,
                          bool cacheNormal      = false,  // 法線をキャッシュするかどうか
                          bool useCachedNormal  = false); // キャッシュされた法線を使うかどうか
        
    protected:
        // All the statismo classes have to be parameterized with the RepresenterType.
        // For building a shape model with vtk, we use the vtkPolyDataRepresenter.
        typedef statismo::vtkStandardMeshRepresenter      RepresenterType;
        typedef statismo::StatisticalModel<vtkPolyData>   StatisticalModelType;
        
        //! ファイルの置いてあるディレクトリ
        std::string m_DirPath;
        //! HDF5ファイル名
        std::string m_FileName;

        //! basel face modelのキャッシュ(shape)
        StatisticalModelType* m_pBaselModelVertices;
        //! basel face modelのキャッシュ(color)
        StatisticalModelType* m_pBaselModelColors;
        
        //! 法線キャッシュ
        std::vector<ofVec3f>    m_aNormalCache;
    };
}

#endif /* ofKsBaselFaceModel_hpp */
