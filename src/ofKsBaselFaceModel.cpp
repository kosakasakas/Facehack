//
//  ofKsBaselFaceModel.cpp
//  Facehack
//
//  Created by Takahiro Kosaka on 2016/07/01.
//  Copyright (c) 2016年 Takahiro Kosaka. All rights reserved.
//
//  This Source Code Form is subject to the terms of the Mozilla
//  Public License v. 2.0. If a copy of the MPL was not distributed
//  with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "ofKsBaselFaceModel.hpp"
#include "System/Util/KSUtil.h"

#include <boost/scoped_ptr.hpp>
#include <vtkDirectory.h>
#include <vtkPolyDataReader.h>
#include <vtkStructuredPoints.h>
#include <vtkStructuredPointsReader.h>
#include <vtkPolyDataWriter.h>
#include <vtkVersion.h>
#include <vtkTriangle.h>

#include "DataManager.h"
#include "PCAModelBuilder.h"
#include "vtkStandardImageRepresenter.h"
#include "PosteriorModelBuilder.h"
#include "ConditionalModelBuilder.h"
#include "vtkStandardImageRepresenter.h"

using namespace Kosakasakas;
using namespace statismo;

//! コンストラクタ
ofKsBaselFaceModel::ofKsBaselFaceModel()
: m_pBaselModelVertices(nullptr)
, m_pBaselModelColors(nullptr)
{}

//! デストラクタ
ofKsBaselFaceModel::~ofKsBaselFaceModel()
{
    Finalize();
}

/**
 @brief 初期化
 
 HDF5モデルを使って初期化を行います.
 以下のモデル以外読めないと思います.
 http://faces.cs.unibas.ch/bfm/main.php?nav=1-1-0&id=details
 @param dirPath     oFのdataディレクトリからの相対でファイルが置いてあるパス
 @param fileName    Basel Face Modelのファイル名
 @return    成功可否
 */
bool    ofKsBaselFaceModel::Initialize(const char* dirPath, const char* fileName)
{
    m_DirPath   = dirPath;
    m_FileName  = fileName;
    return ofKsModel::Initialize();
}

/**
 @brief 終了処理
 */
void    ofKsBaselFaceModel::Finalize()
{
    m_DirPath.clear();
    m_FileName.clear();
    
    // 生ポインタは消す
    if (m_pBaselModelVertices)
    {
        m_pBaselModelVertices->Delete();
        m_pBaselModelVertices = nullptr;
    }
    if (m_pBaselModelColors)
    {
        m_pBaselModelColors->Delete();
        m_pBaselModelColors = nullptr;
    }
}

//! モデルの読み込み
bool    ofKsBaselFaceModel::LoadMesh()
{
    if (m_DirPath.empty() || m_FileName.empty())
    {
        ofLog(OF_LOG_ERROR, "ファイル名が空です。Initialize()を呼んで設定してください.");
        return false;
    }
    
    if (m_pBaselModelVertices || m_pBaselModelColors)
    {
        ofLog(OF_LOG_ERROR, "すでにモデルはロード済みです. 再読み込みする場合はFinalize()を行ってください.");
        return false;
    }
    
    ofDirectory dataDir(m_DirPath);
    string dataPath = dataDir.getAbsolutePath() + "/" + m_FileName;
    
    // HDF5ファイルの読み込み
    H5::H5File file;
    try {
        file = H5::H5File(dataPath.c_str(), H5F_ACC_RDONLY);
    } catch (H5::Exception& e) {
        std::string msg(std::string("could not open HDF5 file \n") + e.getCDetailMsg());
        
        ofLog(OF_LOG_ERROR, "%s", msg.c_str());
        ofLog(OF_LOG_ERROR, "ofKsBaselFaceModelで読み込めるのは現状、HDF5モデルのみです. 正しいファイルを指定してください.");
        return false;
    }
    
    // カラーグループを取得
    const H5::Group shapeRoot = file.openGroup("shape");
    const H5::Group colorRoot = file.openGroup("color");
    
    try {
        
        // To load a model, we call the static Load method, which returns (a pointer to) a
        // new StatisticalModel object
        RepresenterType* representer    = RepresenterType::Create();
        m_pBaselModelVertices           = StatisticalModelType::Load(representer, shapeRoot);
        m_pBaselModelColors             = StatisticalModelType::Load(representer, colorRoot);
        
        representer->Delete();
        representer = nullptr;
        
    } catch (StatisticalModelException& e) {
        std::cout << "Exception occured while building the shape model" << std::endl;
        std::cout << e.what() << std::endl;
        
        ofLog(OF_LOG_ERROR, "モデルの読み込みに失敗しました.");
        
        // データを消去しておく
        if(m_pBaselModelVertices)
        {
            m_pBaselModelVertices->Delete();
            m_pBaselModelVertices = nullptr;
        }
        if(m_pBaselModelColors)
        {
            m_pBaselModelColors->Delete();
            m_pBaselModelColors = nullptr;
        }
    
        return false;
    }
    
    return true;
}

/**
 @brief MeanShapeをメッシュに書き込む
 
 PCAのMeanShapeをメッシュに書き込みます.
 必ずInitialize()を読んでから使用してください.
 @return 成功可否
 */
bool    ofKsBaselFaceModel::DrawMean()
{
    if (!m_pBaselModelVertices || !m_pBaselModelColors)
    {
        ofLog(OF_LOG_ERROR, "モデルがロードされていません.");
        return false;
    }
    
    // meanを取得
    vtkPolyData* pShape;
    {
        pShape = m_pBaselModelVertices->DrawMean();
        if (!pShape)
        {
            goto ERROR_LABEL;
            return false;
        }
    }
    vtkPolyData* pAlbedo;
    {
        pAlbedo = m_pBaselModelColors->DrawMean();
        if (!pAlbedo)
        {
            goto ERROR_LABEL;
            return false;
        }
    }
    
    // ofMeshを構築
    if (!SetupMesh(m_Mesh, pShape, pAlbedo))
    {
        ofLog(OF_LOG_ERROR, "メッシュの構築に失敗しました");
        goto ERROR_LABEL;
    }
    
    return true;
    
ERROR_LABEL:
    if (pShape)     pShape->Delete();
    if (pAlbedo)    pAlbedo->Delete();
    m_Mesh.clear();
    return false;
}

/**
 @brief ランダムサンプリングした結果をメッシュに書き込む
 
 PCAの主成分をランダムでサンプリングした結果をメッシュに書き込みます.
 必ずInitialize()を読んでから使用してください.
 @return 成功可否
 */
bool    ofKsBaselFaceModel::DrawRandomSample()
{
    if (!m_pBaselModelVertices || !m_pBaselModelColors)
    {
        ofLog(OF_LOG_ERROR, "モデルがロードされていません.");
        return false;
    }
    
    // ランダムサンプリング
    vtkPolyData* pShape;
    {
        pShape = m_pBaselModelVertices->DrawSample();
        if (!pShape)
        {
            goto ERROR_LABEL;
            return false;
        }
    }
    vtkPolyData* pAlbedo;
    {
        pAlbedo = m_pBaselModelColors->DrawSample();
        if (!pAlbedo)
        {
            goto ERROR_LABEL;
            return false;
        }
    }
    
    // ofMeshを構築
    if (!SetupMesh(m_Mesh, pShape, pAlbedo))
    {
        ofLog(OF_LOG_ERROR, "メッシュの構築に失敗しました");
        goto ERROR_LABEL;
    }
    
    return true;
    
ERROR_LABEL:
    if (pShape)     pShape->Delete();
    if (pAlbedo)    pAlbedo->Delete();
    m_Mesh.clear();
    return false;
}

/**
 @brief 指定のPCAの主成分値でサンプリングした結果をメッシュに書き込む
 
 指定したPCAの主成分値でサンプリングした結果をメッシュに書き込みます.
 指定されていない主成分の値は0.0で計算されます.
 必ずInitialize()を読んでから使用してください.
 @return 成功可否
 */
bool    ofKsBaselFaceModel::DrawSample(KSVectorXf& shapeCoeff, KSVectorXf& albedoCoeff)
{
    if (!m_pBaselModelVertices || !m_pBaselModelColors)
    {
        ofLog(OF_LOG_ERROR, "モデルがロードされていません.");
        return false;
    }
    
    // 主成分の数チェック
    int shapePrincipleNum   = m_pBaselModelVertices->GetNumberOfPrincipalComponents();
    int albedoPrincipleNum  = m_pBaselModelColors->GetNumberOfPrincipalComponents();
    if (shapeCoeff.size() > shapePrincipleNum)
    {
        ofLog(OF_LOG_ERROR, "シェイプの主成分の個数を超えています.");
        return false;
    }
    if (albedoCoeff.size() > albedoPrincipleNum)
    {
        ofLog(OF_LOG_ERROR, "アルベドの主成分の個数を超えています.");
        return false;
    }
    
    // 足りない分の要素は0.0を入れて拡張しておく
    shapeCoeff.conservativeResizeLike(KSVectorXf::Zero(shapePrincipleNum));
    albedoCoeff.conservativeResizeLike(KSVectorXf::Zero(albedoPrincipleNum));
    
    // サンプリング
    vtkPolyData* pShape;
    {
        pShape = m_pBaselModelVertices->DrawSample((VectorType)shapeCoeff);
        if (!pShape)
        {
            goto ERROR_LABEL;
            return false;
        }
    }
    vtkPolyData* pAlbedo;
    {
        pAlbedo = m_pBaselModelColors->DrawSample((VectorType)albedoCoeff);
        if (!pAlbedo)
        {
            goto ERROR_LABEL;
            return false;
        }
    }
    
    // ofMeshを構築
    if (!SetupMesh(m_Mesh, pShape, pAlbedo))
    {
        ofLog(OF_LOG_ERROR, "メッシュの構築に失敗しました");
        goto ERROR_LABEL;
    }
    
    return true;
    
ERROR_LABEL:
    if (pShape)     pShape->Delete();
    if (pAlbedo)    pAlbedo->Delete();
    m_Mesh.clear();
    return false;
}

/**
 @brief データをメッシュに読み込む
 
 書き込んだvtkPolyDataからofMeshにデータをセットアップします.
 @return 成功可否
 */
bool    ofKsBaselFaceModel::SetupMesh(ofMesh& dstMesh, vtkPolyData* pSrcVertices, vtkPolyData* pSrcColors)
{
    if (!pSrcVertices || !pSrcColors)
    {
        ofLog(OF_LOG_ERROR, "引数が空です.");
        return false;
    }
    
    int numVertices = pSrcVertices->GetNumberOfCells();
    int numColors   = pSrcColors->GetNumberOfCells();
    
    if (numColors != numVertices)
    {
        ofLog(OF_LOG_ERROR, "頂点数とカラー数の対応が取れません.");
        return false;
    }
    
    // メッシュをクリア
    m_Mesh.clear();
    
    // メッシュ用のバッファ
    ofVec3f         vtx[3];
    ofFloatColor    col[3];
    ofVec3f         nol;
    
    // 頂点用バッファ
    double v0[3];
    double v1[3];
    double v2[3];
    // カラー用バッファ
    double c0[3];
    double c1[3];
    double c2[3];
    // 法線用バッファ
    double n[3];

    // プリミティブモードの指定
    m_Mesh.setMode(OF_PRIMITIVE_TRIANGLES);
    
    // 読み込み
    for (int i=0; i<numVertices; ++i)
    {
        vtkCell* pCellVertex = pSrcVertices->GetCell(i);
        vtkCell* pCellColor  = pSrcColors->GetCell(i);
        
        vtkTriangle* pTriangleVertex = dynamic_cast<vtkTriangle*>(pCellVertex);
        vtkTriangle* pTriangleColor  = dynamic_cast<vtkTriangle*>(pCellColor);
        if (!pTriangleVertex || !pTriangleColor)
        {
            ofLog(OF_LOG_ERROR, "TriangleでないCellが見つかりました.");
            m_Mesh.clear();
            return false;
        }
        
        // 頂点をバッファにロード
        pTriangleVertex->GetPoints()->GetPoint(0, v0);
        pTriangleVertex->GetPoints()->GetPoint(1, v1);
        pTriangleVertex->GetPoints()->GetPoint(2, v2);
        // カラーをバッファにロード
        pTriangleColor->GetPoints()->GetPoint(0, c0);
        pTriangleColor->GetPoints()->GetPoint(1, c1);
        pTriangleColor->GetPoints()->GetPoint(2, c2);
        // 法線計算
        KSUtil::CalcFaceNormal(n, v0, v1, v2);
        
        // ベクトルオブジェクトに格納
        vtx[0].x = v0[0];
        vtx[0].y = v0[1];
        vtx[0].z = v0[2];
        
        vtx[1].x = v1[0];
        vtx[1].y = v1[1];
        vtx[1].z = v1[2];
        
        vtx[2].x = v2[0];
        vtx[2].y = v2[1];
        vtx[2].z = v2[2];
        
        col[0].r = c0[0];
        col[0].g = c0[1];
        col[0].b = c0[2];
        col[0].a = 1.0f;
        
        col[1].r = c1[0];
        col[1].g = c1[1];
        col[1].b = c1[2];
        col[1].a = 1.0f;
        
        col[2].r = c2[0];
        col[2].g = c2[1];
        col[2].b = c2[2];
        col[2].a = 1.0f;
        
        nol.x    = n[0];
        nol.y    = n[1];
        nol.z    = n[2];
        
        // メッシュを構築
        m_Mesh.addVertex(vtx[0]);
        m_Mesh.addVertex(vtx[1]);
        m_Mesh.addVertex(vtx[2]);
        
        m_Mesh.addColor(col[0]);
        m_Mesh.addColor(col[1]);
        m_Mesh.addColor(col[2]);
        
        m_Mesh.addNormal(nol);
        m_Mesh.addNormal(nol);
        m_Mesh.addNormal(nol);
    }

    return true;
}

